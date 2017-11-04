//===----------------------------------------------------------------------===//
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
#include <gtest/gtest.h>

#include <string>
using std::string;

#include "util/Hexadecimal.h"
using util::binaryCharsToHexChar;
using util::binaryStringToHexString;
using util::hexToByte;
using util::hexPairToByte;

TEST(Hexadecimal, binaryCharsToHexChar) {
  EXPECT_EQ('0', binaryCharsToHexChar('0','0','0','0'));
  EXPECT_EQ('1', binaryCharsToHexChar('0','0','0','1'));
  EXPECT_EQ('2', binaryCharsToHexChar('0','0','1','0'));
  EXPECT_EQ('3', binaryCharsToHexChar('0','0','1','1'));
  EXPECT_EQ('4', binaryCharsToHexChar('0','1','0','0'));
  EXPECT_EQ('5', binaryCharsToHexChar('0','1','0','1'));
  EXPECT_EQ('6', binaryCharsToHexChar('0','1','1','0'));
  EXPECT_EQ('7', binaryCharsToHexChar('0','1','1','1'));
  EXPECT_EQ('8', binaryCharsToHexChar('1','0','0','0'));
  EXPECT_EQ('9', binaryCharsToHexChar('1','0','0','1'));
  EXPECT_EQ('A', binaryCharsToHexChar('1','0','1','0'));
  EXPECT_EQ('B', binaryCharsToHexChar('1','0','1','1'));
  EXPECT_EQ('C', binaryCharsToHexChar('1','1','0','0'));
  EXPECT_EQ('D', binaryCharsToHexChar('1','1','0','1'));
  EXPECT_EQ('E', binaryCharsToHexChar('1','1','1','0'));
  EXPECT_EQ('F', binaryCharsToHexChar('1','1','1','1'));
}

TEST(Hexadecimal, binaryStringToHexString) {
  string str = "0000";
  EXPECT_EQ("0", binaryStringToHexString(str.begin(), str.end()));
  str = "111100001010";
  EXPECT_EQ("F0A", binaryStringToHexString(str.begin(), str.end()));
}

TEST(Hexadecimal, hexToByte) {
  EXPECT_EQ(0, hexToByte('0'));
  EXPECT_EQ(1, hexToByte('1'));
  EXPECT_EQ(2, hexToByte('2'));
  EXPECT_EQ(3, hexToByte('3'));
  EXPECT_EQ(4, hexToByte('4'));
  EXPECT_EQ(5, hexToByte('5'));
  EXPECT_EQ(6, hexToByte('6'));
  EXPECT_EQ(7, hexToByte('7'));
  EXPECT_EQ(8, hexToByte('8'));
  EXPECT_EQ(9, hexToByte('9'));
  EXPECT_EQ(10, hexToByte('A'));
  EXPECT_EQ(10, hexToByte('a'));
  EXPECT_EQ(11, hexToByte('B'));
  EXPECT_EQ(11, hexToByte('b'));
  EXPECT_EQ(12, hexToByte('C'));
  EXPECT_EQ(12, hexToByte('c'));
  EXPECT_EQ(13, hexToByte('D'));
  EXPECT_EQ(13, hexToByte('d'));
  EXPECT_EQ(14, hexToByte('E'));
  EXPECT_EQ(14, hexToByte('e'));
  EXPECT_EQ(15, hexToByte('F'));
  EXPECT_EQ(15, hexToByte('f'));
}

TEST(Hexadecimal, hexPairToByte) {
  // Uses hexToByte so case checking not needed

  EXPECT_EQ(0, hexPairToByte('0', '0'));
  EXPECT_EQ(255, hexPairToByte('F', 'F'));

  // TODO: add more
}


