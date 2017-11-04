//===-- util/Hexadecimal.h - Common hex manipulation routines ---*- C++ -*-===//
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
/// \brief This file defines basic routines for manipulating things in hex
///
//===----------------------------------------------------------------------===//
#ifndef UTIL_HEXADECIMAL_H_INCLUDED
#define UTIL_HEXADECIMAL_H_INCLUDED

#include <string>
#include "util/Types.h"

namespace util {
char binaryCharsToHexChar(const char a, const char b, const char c, const char d);
template <typename InputIterator>
  std::string binaryStringToHexString(InputIterator begin, InputIterator end);
byte hexToByte(const char a);
byte hexPairToByte(const char a, const char b);
}

namespace util {
template <typename InputIterator>
std::string binaryStringToHexString(InputIterator begin, InputIterator end) {
  char a, b, c, d;
  std::string out = "";

  while (begin != end) {
    a = *begin;
    ++begin;

    if (begin != end) {
      b = *begin;
      ++begin;
    } else {
      b = '0';
    }

    if (begin != end) {
      c = *begin;
      ++begin;
    } else {
      c = '0';
    }

    if (begin != end) {
      d = *begin;
      ++begin;
    } else {
      d = '0';
    }

    out += binaryCharsToHexChar(a, b, c, d);
  }
  return out;
}
}
#endif
