//===------------------------------------------------------------*- C++ -*-===//
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
/// \brief BitTuple - n-tuple of bits, packed, optimized for small sizes
///
//===----------------------------------------------------------------------===//
/*
 * Copyright 2017 Will Mitchell
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef ADT_BITTUPLE_H_INCLUDED
#define ADT_BITTUPLE_H_INCLUDED

#include <cassert>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <type_traits>

#include "common/BitTwiddle.h"

namespace adt {
/// BitTuple - n-tuple of bits, packed, with optimizations for small sizes
///
/// This is similar to a llvm::SmallBitVector. It contains one pointer-sized
/// field which is used to store the bits when possible, otherwise it is a
/// pointer to a larger array which contains both the bits and the sizes needed
/// to track everything. To tell the difference between the two situations, the
/// lsb of the pointer is checked. Therefore we are assuming that we will not
/// be allocated memory starting at an odd address.
template <typename Storage = uint64_t> class BitTuple {
  // Parts of this implementation is based heavily upon LLVM's SmallBitVector
public:
  template <typename S>
  friend
  bool operator==(const BitTuple<S> &lhs, const BitTuple<S> &rhs);

  typedef size_t size_type;

  // Wraps a single bit
  class reference {
    BitTuple<Storage> &bt;
    size_type pos;

  public:
    reference(BitTuple<Storage> &b, size_type i);
    reference(const reference &) = default;
    reference(reference &&) = default;
    reference &operator=(reference b);
    reference &operator=(bool b);
    operator bool() const __attribute__((pure));
  };

  /// default ctor - create an empty small bit tuple
  BitTuple();
  /// ctor - create a bit tuple of the specified size. All bits are initialized
  /// to the specified value
  explicit BitTuple(size_type newSize, bool defaultVal = false);
  /// copy ctor
  BitTuple(const BitTuple<Storage> &rhs);
  /// move ctor
  BitTuple(BitTuple<Storage> &&rhs);
  /// dtor
  ~BitTuple();

  /// returns the number of bits
  size_type size() const __attribute__((pure));
  /// returns true iff size() is 0
  bool empty() const __attribute__((pure));
  /// population count
  size_type count() const __attribute__((pure));
  /// returns true if any bit is set
  bool any() const __attribute__((pure));
  /// return true is all bits are set
  bool all() const __attribute__((pure));
  /// returns true if no bit is set
  bool none() const __attribute__((pure));
  /// sets to pristine state
  void clear();
  /// grow or shrink, filling new values as specified
  void resize(size_type newSize, bool defaultVal = false);
  /// sets all bits
  BitTuple<Storage> &set();
  /// sets a specific bit
  BitTuple<Storage> &set(size_type i);
  /// sets a range of bits [a, b)
  BitTuple<Storage> &set(size_type a, size_type b);
  /// resets all bits
  BitTuple<Storage> &reset();
  /// resets a specific bit
  BitTuple<Storage> &reset(size_type i);
  /// resets a range of bits [a, b)
  BitTuple<Storage> &reset(size_type a, size_type b);
  /// flips all bits
  BitTuple<Storage> &flip();
  /// flips a specific bit
  BitTuple<Storage> &flip(size_type i);

  /// index
  reference operator[](size_type i);
  /// index
  bool operator[](size_type i) const __attribute__((pure));
  /// returns true iff specified bit is true
  bool test(size_type i) const __attribute__((pure));
  /// intersection
  BitTuple<Storage> &operator&=(const BitTuple<Storage> &rhs);
  /// difference. Same as *this &= ~rhs
  BitTuple<Storage> &reset(const BitTuple<Storage> &rhs);
  /// difference. Same as *this &= ~rhs
  BitTuple<Storage> &operator-=(const BitTuple<Storage> &rhs);
  /// intersection non-empty. Same as reset(rhs) and any()
  bool test(const BitTuple<Storage> &rhs) const __attribute__((pure));
  /// union
  BitTuple<Storage> &operator|=(const BitTuple<Storage> &rhs);
  /// symmetric difference
  BitTuple<Storage> &operator^=(const BitTuple<Storage> &rhs);
  /// copy assignment
  BitTuple<Storage> &operator=(const BitTuple<Storage> &rhs);
  /// move assignment
  BitTuple<Storage> &operator=(BitTuple<Storage> &&rhs);
  /// swap
  void swap(BitTuple<Storage> &rhs);

private:
  enum {
    // The number of bits in the pointer
    NumPointerBits = sizeof(Storage *) * CHAR_BIT,
    // One bit is used to differentiate between small and large mode
    SmallNumTotalBits = NumPointerBits - 1,
    // How many bits needed to represent the length in small mode.
    // = ceil(log2(NumPointerBits))
    SmallNumSizeBits =
        (NumPointerBits == 32 ? 5 : NumPointerBits == 64 ? 6 : 0),
    // How many bits used for storage
    SmallNumDataBits = SmallNumTotalBits - SmallNumSizeBits,
    // The number of bits in storage
    NumStorageBits = sizeof(Storage) * CHAR_BIT
  };
  // Only 32 / 64 bit pointers supported due to how SmallNumSizeBits is
  // calculated
  static_assert(NumPointerBits == 64 || NumPointerBits == 32,
                "Unsupported word size");
  // Trivially copyable needed since we use std::malloc/std::realloc
  // FIXME: libstdc++ doesn't yet support this
  // static_assert(std::is_trivially_copyable<Storage>::value,
  //              "Storage not trivially copyable");
  static_assert(std::is_integral<Storage>::value, "Storage not integral");
  // Since doing bit manipulations, require unsigned to avoid undefined behavior
  static_assert(std::is_unsigned<Storage>::value, "Storage not unsigned");

  // When in small mode, X has format "size | bits | 1"
  // When in large mode, X is a pointer to an array of Storage with X[0] being
  // the length of the array and X[1] being the bits stored
  uintptr_t X;

  bool isSmall() const __attribute__((pure));
  void switchToSmall(size_type newSize, uintptr_t bits);
  uintptr_t getSmallRawBits() const __attribute__((pure));
  void setSmallRawBits(uintptr_t bits);
  uintptr_t getSmallBits() const __attribute__((pure));
  void setSmallBits(uintptr_t bits);
  size_type getSmallSize() const __attribute__((pure));
  void setSmallSize(size_type s);

  void switchToLarge(Storage *);
  Storage *getPointer() const __attribute__((pure));
  void clearLargeUnusedBits();
};

/// returns a flipped copy
template <typename Storage>
BitTuple<Storage> operator~(const BitTuple<Storage> &arg) __attribute__((pure));

/// returns a flipped copy
template <typename Storage>
BitTuple<Storage> &operator~(BitTuple<Storage> &&arg) __attribute__((pure));

/// equality
template <typename Storage>
bool operator==(const BitTuple<Storage> &lhs,
                const BitTuple<Storage> &rhs) __attribute__((pure));

/// inequality
template <typename Storage>
bool operator!=(const BitTuple<Storage> &lhs,
                const BitTuple<Storage> &rhs) __attribute__((pure));

/// intersection
template <typename Storage>
BitTuple<Storage> operator&(const BitTuple<Storage> &lhs,
                            const BitTuple<Storage> &rhs) __attribute__((pure));

/// union
template <typename Storage>
BitTuple<Storage> operator|(const BitTuple<Storage> &lhs,
                            const BitTuple<Storage> &rhs) __attribute__((pure));

/// difference
template <typename Storage>
BitTuple<Storage> operator-(const BitTuple<Storage> &lhs,
                            const BitTuple<Storage> &rhs) __attribute__((pure));

/// symmetric difference
template <typename Storage>
BitTuple<Storage> operator^(const BitTuple<Storage> &lhs,
                            const BitTuple<Storage> &rhs) __attribute__((pure));
} // namespace adt

namespace std {
template <typename Storage>
void swap(adt::BitTuple<Storage> &lhs, adt::BitTuple<Storage> &rhs) {
  lhs.swap(rhs);
}
} // namespace std

// Implementation
//------------------------------------------------------------------------------
namespace adt {
// create empty small
template <typename Storage> BitTuple<Storage>::BitTuple() : X(1) {}

// create empty small and resize
template <typename Storage>
BitTuple<Storage>::BitTuple(size_type newSize, bool defaultVal)
    : X(1) {
  resize(newSize, defaultVal);
}

template <typename Storage> BitTuple<Storage>::BitTuple(const BitTuple &rhs) {
  if (rhs.isSmall()) {
    X = rhs.X;
  } else {
    const size_t len = rhs.getPointer()[0] * sizeof(Storage);
    Storage *data = static_cast<Storage *>(std::malloc(len));
    std::memcpy(data, rhs.getPointer(), len);
    switchToLarge(data);
  }
}

template <typename Storage>
BitTuple<Storage>::BitTuple(BitTuple &&rhs)
    : X(rhs.X) {
  rhs.X = 1;
}

template <typename Storage> BitTuple<Storage>::~BitTuple() {
  if (!isSmall())
    std::free(getPointer());
}

template <typename Storage>
typename BitTuple<Storage>::size_type BitTuple<Storage>::size() const {
  if (isSmall())
    return getSmallSize();
  return static_cast<size_type>(getPointer()[1]);
}

template <typename Storage> bool BitTuple<Storage>::empty() const {
  if (isSmall())
    return getSmallSize() == 0;
  return getPointer()[1] == Storage(0);
}

// population count
template <typename Storage>
typename BitTuple<Storage>::size_type BitTuple<Storage>::count() const {
  if (isSmall())
    return common::popCount(getSmallBits());

  size_type pop = 0;
  const size_type arrayLen = static_cast<size_type>(getPointer()[0]);
  for (size_t i = 2; i < arrayLen; ++i)
    pop += common::popCount(getPointer()[i]);

  return pop;
}

/// returns true if any bit is set
template <typename Storage>
bool BitTuple<Storage>::any() const {
  if (isSmall())
    return getSmallBits() > 0;

  // Large
  const size_type arrayLen = static_cast<size_type>(getPointer()[0]);
  for (size_t i = 2; i < arrayLen; ++i) {
    if (getPointer()[i] > 0)
      return true;
  }

  return false;
}

/// return true is all bits are set
template <typename Storage>
bool BitTuple<Storage>::all() const {
  if (isSmall()) {
    const uintptr_t mask = (static_cast<uintptr_t>(1) << getSmallSize()) - 1;
    return (getSmallBits() & mask) == mask;
  }

  // Large
  // Note -1 is added because the last element is handled separately, and
  // for common subexpression usage
  const size_type arrayLen = static_cast<size_type>(getPointer()[0]) - 1;
  Storage mask = static_cast<Storage>(~static_cast<Storage>(0));
  for (size_t i = 2; i < arrayLen; ++i) {
    if ((getPointer()[i] & mask) != mask)
      return false;
  }
  // num bits stored is even multiple so last Storage is fully used
  const Storage remainder = getPointer()[1] % NumStorageBits;
  if (remainder == 0)
    return (getPointer()[arrayLen] & mask) == mask;

  // Have only some bits in the last Storage entry, adjust mask
  mask >>= NumStorageBits - remainder;
  return (getPointer()[arrayLen] & mask) == mask;
}

/// returns true if no bit is set
template <typename Storage>
bool BitTuple<Storage>::none() const {
  return !any();
}

// sets to pristine state
template <typename Storage> void BitTuple<Storage>::clear() {
  if (!isSmall())
    std::free(getPointer());
  X = 1;
}

// grow or shrink, setting new values with defaultVal
template <typename Storage>
void BitTuple<Storage>::resize(size_type newSize, bool defaultVal) {
  assert(newSize <= std::numeric_limits<Storage>::max() &&
         "Size can be at most max value of Storage");
  if (isSmall()) {
    if (newSize <= SmallNumDataBits) {
      // small -> small
      uintptr_t newBits = defaultVal ? ~uintptr_t(0) << getSmallSize() : 0;
      setSmallSize(newSize);
      setSmallBits(newBits | getSmallBits());
    } else {
      // small -> large
      // numStorage = ceil(newSize / NumStorageBits) + 2
      // The + 2 are for numStorage and newSize
      const Storage numStorage = static_cast<Storage>(
          (newSize + NumStorageBits - 1) / NumStorageBits + 2);
      Storage *large =
          static_cast<Storage *>(std::malloc(sizeof(Storage) * numStorage));

      // Store the length of this array and the number of bits separately
      large[0] = numStorage;
      large[1] = static_cast<Storage>(newSize);

      // Default value to use
      const Storage defaultBits =
          defaultVal ? static_cast<Storage>(~Storage(0)) : Storage(0);

      // Copy the existing bits from small uintptr_t storage to the large array
      const Storage full =
          static_cast<Storage>(getSmallSize() / NumStorageBits);
      const Storage remainder =
          static_cast<Storage>(getSmallSize() % NumStorageBits);

      // From only the small uintptr_t
      const uintptr_t smallBits = getSmallBits();
      for (Storage i = 0; i < full; ++i)
        large[2 + i] =
            static_cast<Storage>((smallBits >> (i * NumStorageBits))) &
            ~Storage(0);

      // Last of bits from small uintptr_t filled with default values
      large[2 + full] =
          static_cast<Storage>((defaultBits << remainder)) |
          static_cast<Storage>((smallBits >> (full * NumStorageBits)));

      // Fill up the rest with just default values
      for (Storage i = static_cast<Storage>(3 + full); i < numStorage; ++i)
        large[i] = defaultBits;

      switchToLarge(large);
      clearLargeUnusedBits();
    }
  } else {
    if (getPointer()[1] == newSize) {
      // large, no change of size
      return;
    }
    if (newSize <= SmallNumDataBits) {
      // large -> small
      uintptr_t newBits = getPointer()[2];
      size_t additional = NumPointerBits / NumStorageBits;
      for (size_t i = 0; i < additional; ++i)
        newBits |= static_cast<uintptr_t>(getPointer()[2 + i])
                   << (i * NumStorageBits);

      std::free(getPointer());

      switchToSmall(newSize, newBits);
    } else {
      // large -> large
      // numStorage = ceil(newSize / NumStorageBits) + 2
      // The + 2 are for numStorage and newSize
      const Storage numStorage = static_cast<Storage>(
          (newSize + NumStorageBits - 1) / NumStorageBits + 2);

      X = reinterpret_cast<uintptr_t>(
          std::realloc(getPointer(), sizeof(Storage) * numStorage));

      // Fill default values if increased size
      if (getPointer()[1] < newSize) {
        // Default value to use
        const Storage defaultBits =
            defaultVal ? static_cast<Storage>(~Storage(0)) : Storage(0);

        const size_type remainder = getPointer()[1] % NumStorageBits;
        getPointer()[getPointer()[0] - 1] |=
            static_cast<Storage>(defaultBits << remainder);

        for (size_t i = getPointer()[0]; i < numStorage; ++i)
          getPointer()[i] = defaultBits;
      }
      getPointer()[0] = numStorage;
      getPointer()[1] = static_cast<Storage>(newSize);
      clearLargeUnusedBits();
    }
  }
}

// sets all bits
template <typename Storage> BitTuple<Storage> &BitTuple<Storage>::set() {
  if (isSmall()) {
    setSmallBits(~uintptr_t(0));
    return *this;
  }
  for (size_t i = 2; i < getPointer()[0]; ++i)
    getPointer()[i] = static_cast<Storage>(~Storage(0));
  clearLargeUnusedBits();

  return *this;
}

// sets a specific bit
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::set(size_type i) {
  assert(i < size() && "Out-of-bounds bit access");
  if (isSmall()) {
    setSmallBits((uintptr_t(1) << i) | getSmallBits());
  } else {
    const size_t idx = i / NumStorageBits + 2;
    const size_t off = i % NumStorageBits;
    getPointer()[idx] |= static_cast<Storage>(Storage(1) << off);
  }
  return *this;
}

// sets a range of bits [a, b)
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::set(size_type a, size_type b) {
  assert(a <= b && "Cannot set bits in range [a,b) if a > b");
  assert(b <= size() && "Cannot set bits in range [a,b) if b > size()");

  if (a == b) {
    // No effect since [a,b) specifies no bits to change
    return *this;
  }

  if (isSmall()) {
    uintptr_t mask = ((static_cast<uintptr_t>(1) << (b - a)) - 1) << a;
    setSmallBits(getSmallBits() | mask);
    return *this;
  }

  const size_type divA = a / NumStorageBits;
  const size_type remA = a % NumStorageBits;
  const size_type divB = b / NumStorageBits;
  const size_type remB = b % NumStorageBits;

  if (divA == divB || (divA == divB + 1 && remB == 0)) {
    // Within one unit of Storage

    if (remA == 0 && divA == divB + 1) {
      // One whole Storage
      const Storage mask = static_cast<Storage>(~static_cast<Storage>(0));
      getPointer()[2 + divA] = mask;
      return *this;
    }

    // Partial amount of storage
    const Storage mask = static_cast<Storage>(
        ((static_cast<Storage>(1) << (b - a)) - 1) << remA);
    getPointer()[2 + divA] |= mask;

    return *this;
  }

  // Leading portion
  if (remA == 0) {
    // One whole storage
    const Storage mask = static_cast<Storage>(~static_cast<Storage>(0));
    getPointer()[2 + divA] = mask;
  } else {
    // Portion of a storage
    const Storage mask = static_cast<Storage>(
        ((static_cast<Storage>(1) << (NumStorageBits - remA)) - 1) << remA);
    getPointer()[2 + divA] |= mask;
  }

  // Middle portion which becomes all 1s
  for (size_type i = divA + 1; i < divB; ++i) {
    const Storage mask = static_cast<Storage>(~static_cast<Storage>(0));
    getPointer()[2 + i] = mask;
  }

  // Trailing Portion. If remB is 0, then no trailing portion the since interval
  // is open at b
  if (remB != 0) {
    const Storage mask = static_cast<Storage>((static_cast<Storage>(1) << remB) - 1);
    getPointer()[2 + divB] |= mask;
  }

  return *this;
}

// resets all bits
template <typename Storage> BitTuple<Storage> &BitTuple<Storage>::reset() {
  if (isSmall())
    setSmallBits(uintptr_t(0));
  else
    std::memset(getPointer() + 2, 0,
                static_cast<size_t>(getPointer()[0] - 2) * sizeof(Storage));
  return *this;
}

// resets a specific bit
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::reset(size_type i) {
  assert(i < size() && "Out-of-bounds bit access");
  if (isSmall()) {
    setSmallBits(~(uintptr_t(1) << i) & getSmallBits());
  } else {
    const size_t idx = i / NumStorageBits + 2;
    const size_t off = i % NumStorageBits;
    getPointer()[idx] &= static_cast<Storage>(~(Storage(1) << off));
  }
  return *this;
}

// resets a range of bits [a, b)
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::reset(size_type a, size_type b) {
  assert(a <= b && "Cannot reset bits in range [a,b) if a > b");
  assert(b <= size() && "Cannot reset bits in range [a,b) if b > size()");

  if (a == b) {
    // No effect since [a,b) specifies no bits to change
    return *this;
  }

  if (isSmall()) {
    uintptr_t mask = ~(((static_cast<uintptr_t>(1) << (b - a)) - 1) << a);
    setSmallBits(getSmallBits() & mask);
    return *this;
  }

  const size_type divA = a / NumStorageBits;
  const size_type remA = a % NumStorageBits;
  const size_type divB = b / NumStorageBits;
  const size_type remB = b % NumStorageBits;

  if (divA == divB || (divA == divB + 1 && remB == 0)) {
    // Within one unit of Storage

    if (remA == 0 && divA == divB + 1) {
      // One whole Storage
      const Storage mask = static_cast<Storage>(0);
      getPointer()[2 + divA] = mask;
      return *this;
    }

    // Partial amount of storage
    const Storage mask = static_cast<Storage>(
        ~(((static_cast<Storage>(1) << (b - a)) - 1) << remA));
    getPointer()[2 + divA] &= mask;

    return *this;
  }

  // Leading portion
  if (remA == 0) {
    // One whole storage
    const Storage mask = static_cast<Storage>(0);
    getPointer()[2 + divA] = mask;
  } else {
    // Portion of a storage
    const Storage mask = static_cast<Storage>(
        ~(((static_cast<Storage>(1) << (NumStorageBits - remA)) - 1) << remA));
    getPointer()[2 + divA] &= mask;
  }

  // Middle portion which becomes all 0s
  for (size_type i = divA + 1; i < divB; ++i) {
    const Storage mask = static_cast<Storage>(0);
    getPointer()[2 + i] = mask;
  }

  // Trailing Portion. If remB is 0, then no trailing portion the since interval
  // is open at b
  if (remB != 0) {
    const Storage mask = static_cast<Storage>(~((static_cast<Storage>(1) << remB) - 1));
    getPointer()[2 + divB] &= mask;
  }

  return *this;
}

// flips all bits
template <typename Storage> BitTuple<Storage> &BitTuple<Storage>::flip() {
  if (isSmall()) {
    setSmallBits(~getSmallBits());
    return *this;
  }

  for (size_t i = 2; i < getPointer()[0]; ++i)
    getPointer()[i] = static_cast<Storage>(~getPointer()[i]);
  clearLargeUnusedBits();

  return *this;
}

// flips a specific bit
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::flip(size_type i) {
  assert(i < size() && "Out-of-bounds bit access");
  if (isSmall()) {
    setSmallBits((uintptr_t(1) << i) ^ getSmallBits());
  } else {
    const size_t idx = i / NumStorageBits + 2;
    const size_t off = i % NumStorageBits;
    getPointer()[idx] ^= static_cast<Storage>(Storage(1) << off);
  }
  return *this;
}

// returns a flipped copy
template <typename Storage>
BitTuple<Storage> operator~(const BitTuple<Storage> &arg) {
  BitTuple<Storage> copy(arg);
  copy.flip();
  return copy;
}

// returns a flipped copy
template <typename Storage>
BitTuple<Storage> &operator~(BitTuple<Storage> &&arg) {
  arg.flip();
  return arg;
}

// index
template <typename Storage>
typename BitTuple<Storage>::reference BitTuple<Storage>::
operator[](size_type i) {
  assert(i < size() && "Out-of-bounds bit access");
  return reference(*this, i);
}

// index
template <typename Storage>
bool BitTuple<Storage>::operator[](size_type i) const {
  return test(i);
}

// returns true iff specified bit is true
template <typename Storage> bool BitTuple<Storage>::test(size_type i) const {
  assert(i < size() && "Out-of-bounds bit access");
  if (isSmall())
    return ((getSmallBits() >> i) & 1) != 0;
  const size_t idx = i / NumStorageBits + 2;
  const size_t off = i % NumStorageBits;
  return ((getPointer()[idx] >> off) & 1) != 0;
}

// intersection
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::operator&=(const BitTuple<Storage> &rhs) {
  assert(
      size() == rhs.size() &&
      "Cannot perform intersection (op&=) unless size (universe) is the same");

  if (isSmall()) {
    setSmallBits(getSmallBits() & rhs.getSmallBits());
    return *this;
  }

  for (Storage i = 2, e = getPointer()[0]; i < e; ++i)
    getPointer()[i] &= rhs.getPointer()[i];

  return *this;
}

// difference. Same as *this &= ~rhs
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::reset(const BitTuple<Storage> &rhs) {
  assert(
      size() == rhs.size() &&
      "Cannot perform difference (reset) unless size (universe) is the same");

  if (isSmall()) {
    setSmallBits(getSmallBits() & ~rhs.getSmallBits());
    return *this;
  }

  for (Storage i = 2, e = getPointer()[0]; i < e; ++i)
    getPointer()[i] &= static_cast<Storage>(~rhs.getPointer()[i]);

  return *this;
}

// difference. Same as *this &= ~rhs
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::operator-=(const BitTuple<Storage> &rhs) {
  return reset(rhs);
}

// intersection non-empty. Same as reset(rhs) and any()
template <typename Storage>
bool BitTuple<Storage>::test(const BitTuple<Storage> &rhs) const {
  assert(size() == rhs.size() && "Cannot perform intersection non-empty "
                                 "(test(rhs)) unless size (universe) is the "
                                 "same");
  if (isSmall())
    return getSmallBits() & rhs.getSmallBits();

  for (Storage i = 2, e = getPointer()[0]; i < e; ++i) {
    if (getPointer()[i] & rhs.getPointer()[i])
      return true;
  }
  return false;
}

// union
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::operator|=(const BitTuple<Storage> &rhs) {
  assert(size() == rhs.size() &&
         "Cannot perform union (op|=) unless size (universe) is the same");

  if (isSmall()) {
    setSmallBits(getSmallBits() | rhs.getSmallBits());
    return *this;
  }

  for (Storage i = 2, e = getPointer()[0]; i < e; ++i)
    getPointer()[i] |= rhs.getPointer()[i];

  return *this;
}

// symmetric difference
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::operator^=(const BitTuple<Storage> &rhs) {
  assert(size() == rhs.size() && "Cannot perform symmetric difference (op^=) "
                                 "unless size (universe) is the same");

  if (isSmall()) {
    setSmallBits(getSmallBits() ^ rhs.getSmallBits());
    return *this;
  }

  for (Storage i = 2, e = getPointer()[0]; i < e; ++i)
    getPointer()[i] ^= rhs.getPointer()[i];

  return *this;
}

// copy assignment
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::operator=(const BitTuple<Storage> &rhs) {
  if (!isSmall())
    std::free(getPointer());

  if (rhs.isSmall()) {
    X = rhs.X;
    return *this;
  }

  // Copy what rhs.X points to
  const size_t len = rhs.getPointer()[0] * sizeof(Storage);
  Storage *data = static_cast<Storage *>(std::malloc(len));
  std::memcpy(data, rhs.getPointer(), len);
  switchToLarge(data);

  return *this;
}

// move assignment
template <typename Storage>
BitTuple<Storage> &BitTuple<Storage>::operator=(BitTuple &&rhs) {
  // Clean up out storage so rhs gets emptied
  if (!isSmall())
    std::free(getPointer());
  X = 1;

  std::swap(X, rhs.X);

  return *this;
}

// swap
template <typename Storage>
void BitTuple<Storage>::swap(BitTuple &rhs) {
  std::swap(X, rhs.X);
}

template <typename Storage> bool BitTuple<Storage>::isSmall() const {
  return X & uintptr_t(1);
}

template <typename Storage>
void BitTuple<Storage>::switchToSmall(size_type s, uintptr_t bits) {
  X = 1;
  setSmallSize(s);
  setSmallBits(bits);
}

template <typename Storage>
uintptr_t BitTuple<Storage>::getSmallRawBits() const {
  assert(isSmall());
  return X >> 1;
}

template <typename Storage>
void BitTuple<Storage>::setSmallRawBits(uintptr_t bits) {
  assert(isSmall());
  X = (bits << 1) | uintptr_t(1);
}

template <typename Storage> uintptr_t BitTuple<Storage>::getSmallBits() const {
  return getSmallRawBits() & ~(~uintptr_t(0) << getSmallSize());
}

template <typename Storage>
void BitTuple<Storage>::setSmallBits(uintptr_t bits) {
  // Note: Need to call setSmallSize before setSmallBits
  setSmallRawBits((bits & ~(~uintptr_t(0) << getSmallSize())) |
                  (getSmallSize() << SmallNumDataBits));
}

template <typename Storage>
typename BitTuple<Storage>::size_type BitTuple<Storage>::getSmallSize() const {
  return getSmallRawBits() >> SmallNumDataBits;
}

template <typename Storage> void BitTuple<Storage>::setSmallSize(size_type s) {
  setSmallRawBits(getSmallBits() | (s << SmallNumDataBits));
}

template <typename Storage> void BitTuple<Storage>::switchToLarge(Storage *p) {
  X = reinterpret_cast<uintptr_t>(p);
  assert(!isSmall() && "Unaligned pointer");
}

template <typename Storage> Storage *BitTuple<Storage>::getPointer() const {
  assert(!isSmall());
  return reinterpret_cast<Storage *>(X);
}

template <typename Storage> void BitTuple<Storage>::clearLargeUnusedBits() {
  assert(!isSmall() && "clearing large unused bits in small mode");
  const size_type remainder = getPointer()[1] % NumStorageBits;
  if (remainder == 0)
    return;
  getPointer()[getPointer()[0] - 1] &=
      static_cast<Storage>((Storage(1) << remainder) - 1);
}

template <typename Storage>
BitTuple<Storage>::reference::reference(BitTuple<Storage> &b, size_type i)
    : bt(b), pos(i) {}

template <typename Storage>
typename BitTuple<Storage>::reference &BitTuple<Storage>::reference::
operator=(typename BitTuple<Storage>::reference b) {
  *this = bool(b);
  return *this;
}

template <typename Storage>
typename BitTuple<Storage>::reference &BitTuple<Storage>::reference::
operator=(bool b) {
  if (b)
    bt.set(pos);
  else
    bt.reset(pos);
  return *this;
}

template <typename Storage>
BitTuple<Storage>::reference::operator bool() const {
  return const_cast<const BitTuple<Storage> &>(bt).operator[](pos);
}

// equality
template <typename Storage>
bool operator==(const BitTuple<Storage> &lhs, const BitTuple<Storage> &rhs) {
  if (lhs.size() != rhs.size())
    return false;

  if (lhs.isSmall())
    return lhs.getSmallRawBits() == rhs.getSmallRawBits();

  if (lhs.getPointer()[0] != rhs.getPointer()[0])
    return false;

  for (Storage i = 1, e = lhs.getPointer()[0]; i < e; ++i) {
    if (lhs.getPointer()[i] != rhs.getPointer()[i])
      return false;
  }

  return true;
}

// inequality
template <typename Storage>
bool operator!=(const BitTuple<Storage> &lhs, const BitTuple<Storage> &rhs) {
  return !(lhs == rhs);
}


// intersection
template <typename Storage>
BitTuple<Storage> operator&(const BitTuple<Storage> &lhs,
                            const BitTuple<Storage> &rhs) {
  BitTuple<Storage> res(lhs);
  res &= rhs;
  return res;
}

// union
template <typename Storage>
BitTuple<Storage> operator|(const BitTuple<Storage> &lhs,
                            const BitTuple<Storage> &rhs) {
  BitTuple<Storage> res(lhs);
  res |= rhs;
  return res;
}


// difference
template <typename Storage>
BitTuple<Storage> operator-(const BitTuple<Storage> &lhs,
                            const BitTuple<Storage> &rhs) {
  BitTuple<Storage> res(lhs);
  res.reset(rhs);
  return res;
}

// symmetric difference
template <typename Storage>
BitTuple<Storage> operator^(const BitTuple<Storage> &lhs,
                            const BitTuple<Storage> &rhs) {
  BitTuple<Storage> res(lhs);
  res ^= rhs;
  return res;
}
} // namespace adt
#endif
