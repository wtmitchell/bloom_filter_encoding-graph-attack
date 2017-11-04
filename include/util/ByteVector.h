//===-- util/ByteVector.h - ByteVector convenience methods ------*- C++ -*-===//
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
///
/// \file
/// \brief This file contains the declaration of the HashFunction class and
/// IncrementalHashFunction class, which are the base classes for all hash
/// functions
///
//===----------------------------------------------------------------------===//
#ifndef UTIL_BYTEVECTOR_H_INCLUDED
#define UTIL_BYTEVECTOR_H_INCLUDED

#include <ostream>
#include <string>
#include <vector>

#include "util/Types.h"

std::ostream &operator<<(std::ostream &out, const std::vector<byte> &v);

// Arithmetic operators
std::vector<byte> operator+(const std::vector<byte> &lhs,
                            const std::vector<byte> &rhs);
std::vector<byte> operator-(const std::vector<byte> &minuend,
                            const std::vector<byte> &subtrahend);
std::vector<byte> operator*(const std::vector<byte> &multiplicand,
                            const byte multiplier);
std::vector<byte> operator*(const std::vector<byte> &multiplicand,
                            const unsigned int multiplier);
std::vector<byte> operator*(const std::vector<byte> &multiplicand,
                            const std::vector<byte> &multiplier);
std::vector<byte> operator%(const std::vector<byte> &number,
                            const std::vector<byte> &modulus);
unsigned int operator%(const std::vector<byte> &number,
                       const unsigned int modulus);

// Bitwise operators
std::vector<byte> operator&(const std::vector<byte> &lhs,
                            const std::vector<byte> &rhs);
std::vector<byte> operator|(const std::vector<byte> &lhs,
                            const std::vector<byte> &rhs);
std::vector<byte> operator^(const std::vector<byte> &lhs,
                            const std::vector<byte> &rhs);

// Comparison operators
bool operator<(const std::vector<byte> &lhs, const std::vector<byte> &rhs);
bool operator<=(const std::vector<byte> &lhs, const std::vector<byte> &rhs);
bool operator==(const std::vector<byte> &lhs, const std::vector<byte> &rhs);
bool operator!=(const std::vector<byte> &lhs, const std::vector<byte> &rhs);

namespace util {
namespace ByteVector {
std::string toString(const std::vector<byte> &in);
bool isHexString(const std::string in);

std::vector<byte> toByteVector(const std::string &in);
std::vector<byte> toByteVector(const unsigned in);

unsigned int toUnsignedInt(const std::vector<byte> &in);

void removeLeadingZeros(std::vector<byte> &in);
}
}
#endif
