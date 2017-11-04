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
/// \brief This file defines basic routines for bit manipulation
///
//===----------------------------------------------------------------------===//
#ifndef COMMON_BITS_H_INCLUDED
#define COMMON_BITS_H_INCLUDED

#include <cassert>

namespace common {
template <typename T>
typename std::enable_if<
    std::is_integral<T>::value && std::is_unsigned<T>::value, T>::type
  popCount(T in) __attribute__((const));
} //namespace common

// Implementation
//------------------------------------------------------------------------------
namespace common {
template <typename T>
typename std::enable_if<
    std::is_integral<T>::value && std::is_unsigned<T>::value, T>::type
popCount(T in) {
  // TODO, use __builtin_popcount() when available (gcc/clang)
  // or __popcnt16(), __popcnt(), __popcnt64() (MSVC)
  T count = 0;

  for (; in != 0; ++count)
    in &= static_cast<T>(in - 1);

  return count;
}

} // namespace common

#endif
