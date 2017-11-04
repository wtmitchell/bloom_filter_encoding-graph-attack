//===-- util/ByteVector.cpp - ByteVector convenience methods --------------===//
//
// Copyright 2017 Will Mitchell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//
#include "util/ByteVector.h"

#include <algorithm>
using std::copy;
using std::max;
using std::min;
#include <cassert>
#include <cctype>
using std::isxdigit;
#include <cstddef>
using std::free;
#include <cstdio>
using std::scanf;
#include <cstdlib>
using std::strtol;
#include <iterator>
using std::advance;
#include <iomanip>
using std::hex;
using std::setfill;
using std::setw;
using std::uppercase;
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
#include <utility>
using std::pair;
#include <vector>
using std::vector;

#ifdef USE_GMP
#include <gmp.h>
#endif

#include "util/Hexadecimal.h"
#include "util/Types.h"

string util::ByteVector::toString(const vector<byte> &in) {
  stringstream ss;
  ss << hex << uppercase << setfill('0');
  for (vector<byte>::size_type i = 0; i < in.size(); ++i)
    ss << setw(2) << +in[i];

  return ss.str();
}

bool util::ByteVector::isHexString(const string in) {
  for (string::size_type i = 0; i < in.size(); ++i)
    if (!isxdigit(in[i]))
      return false;

  return true;
}

vector<byte> util::ByteVector::toByteVector(const std::string &in) {
  vector<byte> out;

  const string::size_type len = in.length();
  string::size_type offset = 0;
  out.reserve((len + 1) / 2);

  // Add leading zero if needed
  if (len % 2 == 1) {
    out.push_back(util::hexToByte(in[0]));
    ++offset;
  }

  // Parse remainder of string in pairs
  while (offset < len) {
    out.push_back(util::hexPairToByte(in[offset], in[offset + 1]));
    offset += 2;
  }

  return out;
}

// This is declared here instead of the header since no one except the
// overloaded function toByteVector should be calling this template
// To limit its visibility even more, put it in an anonymous namespace

namespace {
// We should only allow this function to be defined if T is integral
// Should be using C++11's std::enable_if and std::is_integral here
// but since its visibility is small enough, we will skip it for now
template <typename T>
// typename enable_if<is_integral<T>::value, std::vector<byte> >::type
    vector<byte> toByteVectorIntegral(const T in) {
  vector<byte> result;

  // Store result byte by byte
  for (int i = sizeof(T); i > 0; --i) {
    byte b = static_cast<byte>(in >> (8 * (i - 1)));

    // Don't add leading zeros
    if (b != 0x00 || result.size() > 0)
      result.push_back(b);
  }

  // Ensure there is at least one byte
  if (result.size() == 0)
    result.push_back(0x00);

  return result;
}
}

vector<byte> util::ByteVector::toByteVector(const unsigned in) {
  return toByteVectorIntegral<unsigned>(in);
}

unsigned int util::ByteVector::toUnsignedInt(const std::vector<byte> &in) {
  // assert(in.size() <= sizeof(unsigned int));

  unsigned int ret = 0;

  for (size_t i = 0; i < in.size(); ++i)
    ret |= static_cast<unsigned>(in[in.size() - i - 1]) << (i * 8u);

  return ret;
}

void util::ByteVector::removeLeadingZeros(std::vector<byte> &in) {
  if (*in.begin() == 0x00 && in.size() >= 2) {
    // Count the leading zero bytes
    vector<byte>::iterator i = in.begin();
    int count = 0;
    for (vector<byte>::iterator e = in.end(); i != e && *i == 0x00;
         ++i, ++count)
      ;

    // Copy them to the beginning of the vector
    copy(i, in.end(), in.begin());

    // Remove the excess bytes at the end
    for (; count > 0; --count)
      in.pop_back();

    // Ensure we haven't made the vector totally empty
    if (in.size() == 0)
      in.push_back(0x00);
  }
}

std::ostream &operator<<(std::ostream &out, const std::vector<byte> &v) {
  out << util::ByteVector::toString(v);

  return out;
}

vector<byte> operator+(const vector<byte> &lhs, const vector<byte> &rhs) {
#ifdef USE_GMP
  // Convert the arguments into mpz_t
  mpz_t gmp_lhs;
  mpz_init(gmp_lhs);
  mpz_import(gmp_lhs, lhs.size(), 1, sizeof(byte), 0, 0, lhs.data());
  mpz_t gmp_rhs;
  mpz_init(gmp_rhs);
  mpz_import(gmp_rhs, rhs.size(), 1, sizeof(byte), 0, 0, rhs.data());

  // Do the addition
  mpz_t gmp_sum;
  mpz_init(gmp_sum);
  mpz_add(gmp_sum, gmp_lhs, gmp_rhs);

  // Free the arguments
  mpz_clear(gmp_lhs);
  mpz_clear(gmp_rhs);

  // Convert to a vector<byte>
  char *result_str = mpz_get_str(NULL, 16, gmp_sum);
  vector<byte> result = util::ByteVector::toByteVector(string(result_str));

  // Free the sum
  mpz_clear(gmp_sum);
  free(result_str);

  return result;
#else // No GMP
  const vector<byte>::size_type szlhs = lhs.size();
  const vector<byte>::size_type szrhs = rhs.size();

  const vector<byte>::size_type szmin = min(szlhs, szrhs);
  const vector<byte>::size_type szmax = max(szlhs, szrhs);

  const vector<byte> *const larger = (szlhs > szrhs) ? &lhs : &rhs;

  vector<byte> sum_rev;
  sum_rev.reserve(szmax + 1);
  byte carry = 0;

  // Add from both lhs and rhs where possible
  for (vector<byte>::size_type i = 1; i <= szmin; ++i) {
    unsigned partial_sum = static_cast<unsigned>(lhs[szlhs - i]) +
                           static_cast<unsigned>(rhs[szrhs - i]) +
                           static_cast<unsigned>(carry);

    if (partial_sum >= (1 << 8))
      carry = static_cast<byte>(partial_sum >> 8);
    else
      carry = 0;

    sum_rev.push_back(static_cast<byte>(partial_sum));
  }

  // Add from larger of lhs and rhs if needed
  for (vector<byte>::size_type i = szmin + 1; i <= szmax; ++i) {
    unsigned partial_sum = static_cast<unsigned>((*larger)[szmax - i]) +
                           static_cast<unsigned>(carry);

    if (partial_sum >= (1 << 8))
      carry = static_cast<byte>(partial_sum >> 8);
    else
      carry = 0;

    sum_rev.push_back(static_cast<byte>(partial_sum));
  }

  if (carry != 0)
    sum_rev.push_back(carry);

  vector<byte> sum(sum_rev.rbegin(), sum_rev.rend());

  return sum;
#endif
}

vector<byte> operator-(const vector<byte> &minuend,
                       const vector<byte> &subtrahend) {
#ifdef USE_GMP
  // Convert the arguments into mpz_t
  mpz_t gmp_lhs;
  mpz_init(gmp_lhs);
  mpz_import(gmp_lhs, minuend.size(), 1, sizeof(byte), 0, 0, minuend.data());
  mpz_t gmp_rhs;
  mpz_init(gmp_rhs);
  mpz_import(gmp_rhs, subtrahend.size(), 1, sizeof(byte), 0, 0,
             subtrahend.data());

  // Do the addition
  mpz_t gmp_diff;
  mpz_init(gmp_diff);
  mpz_sub(gmp_diff, gmp_lhs, gmp_rhs);

  // Free the arguments
  mpz_clear(gmp_lhs);
  mpz_clear(gmp_rhs);

  // Convert to a vector<byte>
  char *result_str = mpz_get_str(NULL, 16, gmp_diff);
  vector<byte> result = util::ByteVector::toByteVector(string(result_str));

  // Free the sum
  mpz_clear(gmp_diff);
  free(result_str);

  return result;
#else // No GMP
  const vector<byte>::size_type szminuend = minuend.size();
  const vector<byte>::size_type szsubtrahend = subtrahend.size();

  // We don't allow negative results
  assert(szminuend >= szsubtrahend);

  vector<byte> difference_rev;
  difference_rev.reserve(szminuend);
  byte borrow = 0;

  // Subtract for every byte in the subtrahend
  for (vector<byte>::size_type i = 1; i <= szsubtrahend; ++i) {
    int partial_diff = static_cast<int>(minuend[szminuend - i]) -
                       static_cast<int>(subtrahend[szsubtrahend - i]) - borrow;

    if (partial_diff < 0) {
      partial_diff += 1 << 8;
      borrow = 1;
    } else
      borrow = 0;

    difference_rev.push_back(static_cast<byte>(partial_diff));
  }

  // Ripple any borrows through
  for (vector<byte>::size_type i = szsubtrahend + 1; i <= szminuend; ++i) {
    int partial_diff = static_cast<int>(minuend[szminuend - i]) - borrow;

    if (partial_diff < 0) {
      partial_diff += 1 << 8;
      borrow = 1;
    } else {
      // Done rippling, straight copy from here
      borrow = 0;
      difference_rev.push_back(static_cast<byte>(partial_diff));
      for (vector<byte>::size_type j = i + 1; j <= szminuend; ++j)
        difference_rev.push_back(static_cast<byte>(minuend[szminuend - j]));
      break;
    }

    difference_rev.push_back(static_cast<byte>(partial_diff));
  }

  // Means we borrowed too much, leading to a negative number
  assert(borrow == 0);

  // Clear out any leading zeros
  while (difference_rev.size() >= 2 && difference_rev.back() == 0x00)
    difference_rev.pop_back();

  vector<byte> difference(difference_rev.rbegin(), difference_rev.rend());

  return difference;
#endif
}

vector<byte> operator*(const vector<byte> &multiplicand,
                       const byte multiplier) {
  const vector<byte>::size_type szmultiplicand = multiplicand.size();

  vector<byte> product_rev;
  product_rev.reserve(szmultiplicand + 1);
  byte carry = 0;

  for (vector<byte>::size_type i = 1; i <= szmultiplicand; ++i) {
    unsigned partial_product =
        static_cast<unsigned>(multiplicand[szmultiplicand - i]) * multiplier +
        carry;

    if (partial_product >= (1 << 8))
      carry = static_cast<byte>(partial_product >> 8);
    else
      carry = 0;

    product_rev.push_back(static_cast<byte>(partial_product));
  }

  if (carry != 0)
    product_rev.push_back(carry);

  vector<byte> product(product_rev.rbegin(), product_rev.rend());

  return product;
}

vector<byte> operator*(const vector<byte> &multiplicand,
                       const unsigned int multiplier) {
  return multiplicand * util::ByteVector::toByteVector(multiplier);
}

vector<byte> operator*(const vector<byte> &multiplicand,
                       const vector<byte> &multiplier) {
#ifdef USE_GMP
  // Convert the arguments into mpz_t
  mpz_t gmp_lhs;
  mpz_init(gmp_lhs);
  mpz_import(gmp_lhs, multiplicand.size(), 1, sizeof(byte), 0, 0,
             multiplicand.data());
  mpz_t gmp_rhs;
  mpz_init(gmp_rhs);
  mpz_import(gmp_rhs, multiplier.size(), 1, sizeof(byte), 0, 0,
             multiplier.data());

  // Do the operation
  mpz_t gmp_result;
  mpz_init(gmp_result);
  mpz_mul(gmp_result, gmp_lhs, gmp_rhs);

  // Free the arguments
  mpz_clear(gmp_lhs);
  mpz_clear(gmp_rhs);

  // Convert to a vector<byte>
  char *result_str = mpz_get_str(NULL, 16, gmp_result);
  vector<byte> result = util::ByteVector::toByteVector(string(result_str));

  // Free the sum
  mpz_clear(gmp_result);
  free(result_str);

  return result;
#else // No GMP
  const vector<byte>::size_type szmultiplier = multiplier.size();

  vector<byte> product;
  product.reserve(szmultiplier + multiplicand.size());

  for (vector<byte>::size_type i = 1; i <= szmultiplier; ++i) {
    vector<byte> partial_product = multiplicand * multiplier[szmultiplier - i];

    for (vector<byte>::size_type j = 1; j < i; ++j)
      partial_product.push_back(0x00);

    product = product + partial_product;
  }

  util::ByteVector::removeLeadingZeros(product);

  return product;
#endif
}

vector<byte> operator%(const vector<byte> &number,
                       const vector<byte> &modulus) {
#ifdef USE_GMP
  // Convert the arguments into mpz_t
  mpz_t gmp_lhs;
  mpz_init(gmp_lhs);
  mpz_import(gmp_lhs, number.size(), 1, sizeof(byte), 0, 0, number.data());
  mpz_t gmp_rhs;
  mpz_init(gmp_rhs);
  mpz_import(gmp_rhs, modulus.size(), 1, sizeof(byte), 0, 0, modulus.data());

  // Do the operation
  mpz_t gmp_result;
  mpz_init(gmp_result);
  mpz_mod(gmp_result, gmp_lhs, gmp_rhs);

  // Free the arguments
  mpz_clear(gmp_lhs);
  mpz_clear(gmp_rhs);

  // Convert to a vector<byte>
  char *result_str = mpz_get_str(NULL, 16, gmp_result);
  vector<byte> result = util::ByteVector::toByteVector(string(result_str));

  // Free the result intermediaries
  mpz_clear(gmp_result);
  free(result_str);

  return result;
#else // No GMP
  assert(modulus != util::ByteVector::toByteVector(0));

  vector<byte> partial_remainder = number;
  vector<byte> divisor = modulus;

  vector<byte>::size_type divisor_padding = number.size() - modulus.size();
  divisor.reserve(divisor.size() + divisor_padding);
  for (vector<byte>::size_type i = 0; i < divisor_padding;
       ++i, divisor.push_back(0x00))
    ;

  while (modulus <= partial_remainder) {
    util::ByteVector::removeLeadingZeros(partial_remainder);

    // Remove padding on divisor as needed
    for (; partial_remainder < divisor && divisor_padding > 0;
         --divisor_padding, divisor.pop_back())
      ;

    // Remove a multiple of the divisor from the partial_remainder
    unsigned estimated_quotient_digit;

    if (partial_remainder.size() > divisor.size())
      estimated_quotient_digit =
          (static_cast<unsigned>(partial_remainder[0]) << 8) +
          static_cast<unsigned>(partial_remainder[1]);
    else
      estimated_quotient_digit = static_cast<unsigned>(partial_remainder[0]);

    estimated_quotient_digit += 1;
    estimated_quotient_digit /= static_cast<unsigned>(divisor[0]) + 1;
    estimated_quotient_digit = max(estimated_quotient_digit, 1u);

    vector<byte> multiple =
        divisor * util::ByteVector::toByteVector(estimated_quotient_digit);

    partial_remainder = partial_remainder - multiple;
  }

  return partial_remainder;
#endif
}

unsigned int operator%(const vector<byte> &number, const unsigned int modulus) {
#ifdef USE_GMP
  // Convert number to mpz_t
  mpz_t gmp_number;
  mpz_init(gmp_number);
  mpz_import(gmp_number, number.size(), 1, sizeof(byte), 0, 0, number.data());

  // Perform the calculation
  unsigned int residue =
      static_cast<unsigned int>(mpz_fdiv_ui(gmp_number, modulus));

  // Clean up intermediaries
  mpz_clear(gmp_number);

  return residue;
#else
  vector<byte> residue = number % util::ByteVector::toByteVector(modulus);
  return util::ByteVector::toUnsignedInt(residue);
#endif
}

std::vector<byte> operator&(const std::vector<byte> &lhs,
                            const std::vector<byte> &rhs) {
  std::vector<byte> result;
  result.reserve(lhs.size());

  for (std::vector<byte>::size_type i = 0, e = lhs.size(); i < e; ++i)
    result.push_back(lhs[i] & rhs[i]);

  return result;
}

std::vector<byte> operator|(const std::vector<byte> &lhs,
                            const std::vector<byte> &rhs) {
  std::vector<byte> result;
  result.reserve(lhs.size());

  for (std::vector<byte>::size_type i = 0, e = lhs.size(); i < e; ++i)
    result.push_back(lhs[i] | rhs[i]);

  return result;
}

std::vector<byte> operator^(const std::vector<byte> &lhs,
                            const std::vector<byte> &rhs) {
  std::vector<byte> result;
  result.reserve(lhs.size());

  for (std::vector<byte>::size_type i = 0, e = lhs.size(); i < e; ++i)
    result.push_back(lhs[i] ^ rhs[i]);

  return result;
}

bool operator<(const vector<byte> &lhs, const vector<byte> &rhs) {
  const vector<byte>::size_type szlhs = lhs.size();
  const vector<byte>::size_type szrhs = rhs.size();

  // First check based on number of bytes ignoring leading zero bytes
  vector<byte>::size_type leadinglhszeros = 0;
  vector<byte>::size_type leadingrhszeros = 0;

  // Count leading zeros of lhs
  vector<byte>::const_iterator ilhs = lhs.begin();
  const vector<byte>::const_iterator elhs = lhs.end();
  for (; ilhs != elhs && *ilhs == 0x00; ++ilhs, ++leadinglhszeros)
    ;

  // Count leading zeros of rhs
  vector<byte>::const_iterator irhs = rhs.begin();
  const vector<byte>::const_iterator erhs = rhs.end();
  for (; irhs != erhs && *irhs == 0x00; ++irhs, ++leadingrhszeros)
    ;

  // Check if we can answer based on magnitude alone
  if (szlhs - leadinglhszeros > szrhs - leadingrhszeros)
    return false;
  if (szlhs - leadinglhszeros < szrhs - leadingrhszeros)
    return true;

  // Can lexicographically compare from here
  // Move ilhs / irhs to either where they differ or the end of the number
  for (; ilhs != elhs && irhs != erhs && *ilhs == *irhs; ++ilhs, ++irhs)
    ;

  // Actually equal
  if (ilhs == elhs && irhs == erhs)
    return false;

  // ilhs and irhs point to the byte where they differ
  return *ilhs < *irhs;
}

bool operator<=(const vector<byte> &lhs, const vector<byte> &rhs) {
  const vector<byte>::size_type szlhs = lhs.size();
  const vector<byte>::size_type szrhs = rhs.size();

  // First check based on number of bytes ignoring leading zero bytes
  vector<byte>::size_type leadinglhszeros = 0;
  vector<byte>::size_type leadingrhszeros = 0;

  // Count leading zeros of lhs
  vector<byte>::const_iterator ilhs = lhs.begin();
  const vector<byte>::const_iterator elhs = lhs.end();
  for (; ilhs != elhs && *ilhs == 0x00; ++ilhs, ++leadinglhszeros)
    ;

  // Count leading zeros of rhs
  vector<byte>::const_iterator irhs = rhs.begin();
  const vector<byte>::const_iterator erhs = rhs.end();
  for (; irhs != erhs && *irhs == 0x00; ++irhs, ++leadingrhszeros)
    ;

  // Check if we can answer based on magnitude alone
  if (szlhs - leadinglhszeros > szrhs - leadingrhszeros)
    return false;
  if (szlhs - leadinglhszeros < szrhs - leadingrhszeros)
    return true;

  // Can lexicographically compare from here
  // Move ilhs / irhs to either where they differ or the end of the number
  for (; ilhs != elhs && irhs != erhs && *ilhs == *irhs; ++ilhs, ++irhs)
    ;

  // Actually equal
  if (ilhs == elhs && irhs == erhs)
    return true;

  // ilhs and irhs point to the byte where they differ
  return *ilhs < *irhs;
}

bool operator==(const vector<byte> &lhs, const vector<byte> &rhs) {
  const vector<byte>::size_type szlhs = lhs.size();
  const vector<byte>::size_type szrhs = rhs.size();

  // First check based on number of bytes ignoring leading zero bytes
  vector<byte>::size_type leadinglhszeros = 0;
  vector<byte>::size_type leadingrhszeros = 0;

  // Count leading zeros of lhs
  vector<byte>::const_iterator ilhs = lhs.begin();
  const vector<byte>::const_iterator elhs = lhs.end();
  for (; ilhs != elhs && *ilhs == 0x00; ++ilhs, ++leadinglhszeros)
    ;

  // Count leading zeros of rhs
  vector<byte>::const_iterator irhs = rhs.begin();
  const vector<byte>::const_iterator erhs = rhs.end();
  for (; irhs != erhs && *irhs == 0x00; ++irhs, ++leadingrhszeros)
    ;

  // If not equal length ignoring leading zeros, then can't possibly be equal
  if (szlhs - leadinglhszeros != szrhs - leadingrhszeros)
    return false;

  // Can lexicographically compare from here
  // Move ilhs / irhs to either where they differ or the end of the number
  for (; ilhs != elhs && irhs != erhs && *ilhs == *irhs; ++ilhs, ++irhs)
    ;

  // Actually equal
  if (ilhs == elhs && irhs == erhs)
    return true;

  return false;
}

bool operator!=(const vector<byte> &lhs, const vector<byte> &rhs) {
  return !(lhs == rhs);
}
