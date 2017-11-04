//===-- util/Hexadecimal.cpp - Hexadecimal convenience methods --------------===//
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
#include "util/Hexadecimal.h"

#include <cassert>

char util::binaryCharsToHexChar(const char a, const char b, const char c, const char d) {
  assert(a == '0' || a == '1');
  assert(b == '0' || b == '1');
  assert(c == '0' || c == '1');
  assert(d == '0' || d == '1');

  int combined  = ((a == '0') ? 0 : 1) * 8 + ((b == '0') ? 0 : 1) * 4
    + ((c == '0') ? 0 : 1) * 2 + ((d == '0') ? 0 : 1);

  if (combined < 10)
    return static_cast<char>(combined + '0');

  return static_cast<char>('A' + combined - 10);
}


byte util::hexToByte(const char a) {
  switch (a) {
  case '0':
    return 0;
  case '1':
    return 1;
  case '2':
    return 2;
  case '3':
    return 3;
  case '4':
    return 4;
  case '5':
    return 5;
  case '6':
    return 6;
  case '7':
    return 7;
  case '8':
    return 8;
  case '9':
    return 9;
  case 'A':
  case 'a':
    return 10;
  case 'B':
  case 'b':
    return 11;
  case 'C':
  case 'c':
    return 12;
  case 'D':
  case 'd':
    return 13;
  case 'E':
  case 'e':
    return 14;
  case 'F':
  case 'f':
    return 15;
  }
  return 0;
}

byte util::hexPairToByte(const char a, const char b) {
  return (static_cast<byte>(hexToByte(a) << 4)) + hexToByte(b);
}
