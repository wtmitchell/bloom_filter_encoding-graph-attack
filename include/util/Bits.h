//===-- util/Bits.h - Common bit manipulation routines ----------*- C++ -*-===//
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
/// \brief This file defines basic routines for bit manipulation
///
//===----------------------------------------------------------------------===//
#ifndef UTIL_BITS_H_INCLUDED
#define UTIL_BITS_H_INCLUDED

#include <cassert>

namespace util {
// snoob - same number of one bits
// (adapted from Fig 2-1 p 14 Hackers Delight 1st ed)
// This increments index to the next larger integer containing the same
// number of ones as index. When through all possible combinations, then
// increase the number of one bits set
// Uses N bits of in to express this
//
// This really should only exist for std::is_integral and std::is_unsigned
// but those require C++11
template <typename T> T snoob(T in, T N) {
  // Can't do this with in == 0, since would trigger division by zero
  assert(in != 0);

  T smallest = in & -in;
  T ripple = in + smallest;
  T ones = in ^ ripple;
  ones = (ones >> 2) / smallest;

  if (ripple >= (1 << N)) {
    // Add in another bit
    ones = (ones << 2) | 3;
    ripple = 0;
  }
  return ripple | ones;
}
}

#endif
