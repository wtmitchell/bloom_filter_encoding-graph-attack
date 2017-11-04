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

#include "util/ByteVector.h"
using util::ByteVector::toString;
using util::ByteVector::toByteVector;
using util::ByteVector::isHexString;

#include "util/Types.h"

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;
#include <vector>
using std::vector;

TEST(ByteVector, Conversion) {
  vector<byte> vb;
  vb.push_back(0x11);
  vb.push_back(0x22);
  vb.push_back(0x33);
  vb.push_back(0x44);
  vb.push_back(0x55);
  vb.push_back(0x66);
  vb.push_back(0x77);
  vb.push_back(0x88);
  vb.push_back(0x99);
  vb.push_back(0x00);
  vb.push_back(0xAA);
  vb.push_back(0xBB);
  vb.push_back(0xCC);
  vb.push_back(0xDD);
  vb.push_back(0xEE);
  vb.push_back(0xFF);

  string expected = "11223344556677889900AABBCCDDEEFF";

  EXPECT_EQ(expected, toString(vb));
  EXPECT_EQ(toByteVector(expected), vb);
  EXPECT_EQ(expected, toString(toByteVector(expected)));

  EXPECT_TRUE(isHexString(expected));
  EXPECT_FALSE(isHexString("GG"));

  // TODO: add tests for toByteVector(unsigned)
  // TODO: add test for toUnsignedInt(vector<byte>)
}

TEST(ByteVector, Addition) {
  vector<byte> lhs;
  vector<byte> rhs;
  vector<byte> sum;

  lhs.push_back(0x80);
  lhs.push_back(0x00);
  lhs.push_back(0x00);

  rhs.push_back(0x80);
  rhs.push_back(0x00);

  sum.push_back(0x80);
  sum.push_back(0x80);
  sum.push_back(0x00);

  EXPECT_EQ(sum, lhs + rhs);
  EXPECT_EQ(sum, rhs + lhs);

  lhs.clear();
  rhs.clear();
  sum.clear();

  lhs.push_back(0x80);
  lhs.push_back(0x00);

  rhs.push_back(0x80);
  rhs.push_back(0x00);

  sum.push_back(0x01);
  sum.push_back(0x00);
  sum.push_back(0x00);

  EXPECT_EQ(sum, lhs + rhs);
  EXPECT_EQ(sum, rhs + lhs);

  lhs.clear();
  rhs.clear();
  sum.clear();

  lhs.push_back(0xFF);
  lhs.push_back(0xFF);

  rhs.push_back(0xFF);
  rhs.push_back(0xFF);

  sum.push_back(0x01);
  sum.push_back(0xFF);
  sum.push_back(0xFE);

  EXPECT_EQ(sum, lhs + rhs);
  EXPECT_EQ(sum, rhs + lhs);
}

TEST(ByteVector, Subtraction) {
  vector<byte> lhs;
  vector<byte> rhs;
  vector<byte> diff;

  lhs.push_back(0x80);
  lhs.push_back(0x00);

  rhs.push_back(0x80);
  rhs.push_back(0x00);

  diff.push_back(0x00);

  EXPECT_EQ(diff, lhs - rhs);

  lhs.clear();
  rhs.clear();
  diff.clear();

  lhs.push_back(0xFF);
  lhs.push_back(0xFF);

  rhs.push_back(0x7F);
  rhs.push_back(0xFF);

  diff.push_back(0x80);
  diff.push_back(0x00);

  EXPECT_EQ(diff, lhs - rhs);

  lhs.clear();
  rhs.clear();
  diff.clear();

  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  lhs.push_back(0x01);
  lhs.push_back(0x00);
  lhs.push_back(0x00);
  lhs.push_back(0x00);
  EXPECT_EQ(lhs, toByteVector("FFFF01000000"));

  rhs.push_back(0x7F);
  rhs.push_back(0xFF);
  EXPECT_EQ(rhs, toByteVector("7FFF"));

  diff.push_back(0xFF);
  diff.push_back(0xFF);
  diff.push_back(0x00);
  diff.push_back(0xFF);
  diff.push_back(0x80);
  diff.push_back(0x01);
  EXPECT_EQ(diff, toByteVector("FFFF00FF8001"));

  EXPECT_EQ(diff, lhs - rhs);
}

TEST(ByteVector, MultiplicationWithByte) {
  vector<byte> multiplicand;

  multiplicand.push_back(0xFF);
  multiplicand.push_back(0xFF);

  byte multiplier = 0x01;
  EXPECT_EQ(multiplicand, multiplicand * multiplier);

  multiplier = 0x02;
  EXPECT_EQ(multiplicand + multiplicand, multiplicand * multiplier);

  multiplier = 0x03;
  EXPECT_EQ(multiplicand + multiplicand + multiplicand,
            multiplicand * multiplier);

  multiplicand.clear();
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);
  multiplicand.push_back(0xAB);

  multiplier = 0x01;
  EXPECT_EQ(multiplicand, multiplicand * multiplier);

  multiplier = 0x02;
  EXPECT_EQ(multiplicand + multiplicand, multiplicand * multiplier);

  multiplier = 0x03;
  EXPECT_EQ(multiplicand + multiplicand + multiplicand,
            multiplicand * multiplier);
}

TEST(ByteVector, MultiplicationWithByteVector) {
  vector<byte> lhs;
  vector<byte> rhs;
  vector<byte> product;

  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  rhs.push_back(0x00);
  rhs.push_back(0x02);

  product.push_back(0x01);
  product.push_back(0xFF);
  product.push_back(0xFE);

  EXPECT_EQ(product, lhs * rhs);
  EXPECT_EQ(product, rhs * lhs);

  lhs.clear();
  rhs.clear();
  product.clear();

  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);

  rhs.push_back(0x00);

  product.push_back(0x00);

  EXPECT_EQ(product, lhs * rhs);
  EXPECT_EQ(product, rhs * lhs);
}

TEST(ByteVector, Modulus) {
  vector<byte> number;
  vector<byte> modulus;
  vector<byte> residue;

  number.push_back(0xFF);
  number.push_back(0xFF);
  number.push_back(0xFF);

  modulus.push_back(0x01);
  modulus.push_back(0xFF);

  residue.push_back(0x3F);

  EXPECT_EQ(residue, number % modulus);

  number.clear();
  modulus.clear();
  residue.clear();

  // number =
  // toByteVector("c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
  number = toByteVector("a470");
  modulus = toByteVector("10");
  residue.push_back(0x00);

  EXPECT_EQ(residue, number % modulus);
}

TEST(ByteVector, Equality) {
  vector<byte> lhs;
  vector<byte> rhs;

  lhs.push_back(0xFF);
  rhs.push_back(0xFF);

  EXPECT_TRUE(lhs == rhs);
  EXPECT_TRUE(rhs == lhs);
  EXPECT_TRUE(lhs == lhs);
  EXPECT_TRUE(rhs == rhs);

  lhs.clear();
  lhs.push_back(0x00);
  lhs.push_back(0xFF);

  EXPECT_TRUE(lhs == rhs);
  EXPECT_TRUE(rhs == lhs);
  EXPECT_TRUE(lhs == lhs);

  lhs.clear();
  rhs.clear();
  lhs.push_back(0x00);
  rhs.push_back(0x00);

  EXPECT_TRUE(lhs == rhs);
  EXPECT_TRUE(rhs == lhs);
  EXPECT_TRUE(lhs == lhs);
  EXPECT_TRUE(rhs == rhs);

  lhs.push_back(0x70);

  EXPECT_FALSE(lhs == rhs);
  EXPECT_FALSE(rhs == lhs);
  EXPECT_TRUE(lhs == lhs);
}

TEST(ByteVector, LessThan) {
  vector<byte> lhs;
  vector<byte> rhs;

  lhs.push_back(0xFF);
  rhs.push_back(0xFF);

  EXPECT_FALSE(lhs < rhs);
  EXPECT_FALSE(rhs < lhs);
  EXPECT_FALSE(lhs < lhs);
  EXPECT_FALSE(rhs < rhs);
  EXPECT_TRUE(lhs <= rhs);
  EXPECT_TRUE(lhs <= lhs);
  EXPECT_TRUE(rhs <= rhs);
  EXPECT_TRUE(rhs <= lhs);

  lhs.push_back(0x00);

  EXPECT_FALSE(lhs < rhs);
  EXPECT_FALSE(lhs < lhs);
  EXPECT_TRUE(rhs < lhs);
  EXPECT_TRUE(rhs <= lhs);

  lhs.clear();
  rhs.clear();

  lhs.push_back(0x00);
  lhs.push_back(0xFF);
  rhs.push_back(0xFF);

  EXPECT_FALSE(lhs < rhs);
  EXPECT_FALSE(rhs < lhs);
  EXPECT_FALSE(lhs < lhs);
  EXPECT_FALSE(rhs < rhs);
  EXPECT_TRUE(lhs <= rhs);
  EXPECT_TRUE(lhs <= lhs);
  EXPECT_TRUE(rhs <= rhs);
  EXPECT_TRUE(rhs <= lhs);

  lhs.clear();
  rhs.clear();

  lhs.push_back(0x00);
  rhs.push_back(0x00);

  EXPECT_FALSE(lhs < rhs);
  EXPECT_FALSE(rhs < lhs);
  EXPECT_FALSE(lhs < lhs);
  EXPECT_FALSE(rhs < rhs);
  EXPECT_TRUE(lhs <= rhs);
  EXPECT_TRUE(lhs <= lhs);
  EXPECT_TRUE(rhs <= rhs);
  EXPECT_TRUE(rhs <= lhs);

  lhs.clear();
  rhs.clear();

  lhs.push_back(0x00);
  lhs.push_back(0x00);
  lhs.push_back(0x00);
  lhs.push_back(0x00);
  lhs.push_back(0x0F);
  rhs.push_back(0xFF);

  EXPECT_TRUE(lhs < rhs);
  EXPECT_FALSE(rhs < lhs);
  EXPECT_FALSE(lhs < lhs);
  EXPECT_FALSE(rhs < rhs);
  EXPECT_TRUE(lhs <= rhs);
  EXPECT_FALSE(rhs <= lhs);
  EXPECT_TRUE(lhs <= lhs);
  EXPECT_TRUE(rhs <= rhs);

  lhs.clear();
  rhs.clear();

  lhs.push_back(0x01);
  lhs.push_back(0xFF);
  rhs.push_back(0x3F);

  EXPECT_FALSE(lhs < rhs);
  EXPECT_TRUE(rhs < lhs);
  EXPECT_FALSE(lhs < lhs);
  EXPECT_FALSE(rhs < rhs);
  EXPECT_FALSE(lhs <= rhs);
  EXPECT_TRUE(rhs <= lhs);
  EXPECT_TRUE(lhs <= lhs);
  EXPECT_TRUE(rhs <= rhs);
}

TEST(ByteVector, BitwiseAnd) {
  vector<byte> lhs;
  vector<byte> rhs;
  vector<byte> result;

  lhs.push_back(0xFF);
  rhs.push_back(0xFF);
  result.push_back(0xFF);

  EXPECT_EQ(result, lhs & rhs);
  EXPECT_EQ(result, rhs & lhs);

  EXPECT_EQ(lhs, lhs & lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0xF0);
  rhs.push_back(0x0F);
  result.push_back(0x00);
  EXPECT_EQ(result, lhs & rhs);
  EXPECT_EQ(result, rhs & lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0xFF);
  rhs.push_back(0x0F);
  result.push_back(0x0F);
  EXPECT_EQ(result, lhs & rhs);
  EXPECT_EQ(result, rhs & lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  rhs.push_back(0x0F);
  rhs.push_back(0x0F);
  result.push_back(0x0F);
  result.push_back(0x0F);
  EXPECT_EQ(result, lhs & rhs);
  EXPECT_EQ(result, rhs & lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0x81);
  lhs.push_back(0x45);
  rhs.push_back(0xFF);
  rhs.push_back(0xFF);
  result.push_back(0x81);
  result.push_back(0x45);
  EXPECT_EQ(result, lhs & rhs);
  EXPECT_EQ(result, rhs & lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0x81);
  lhs.push_back(0x45);
  rhs.push_back(0x00);
  rhs.push_back(0xFF);
  result.push_back(0x00);
  result.push_back(0x45);
  EXPECT_EQ(result, lhs & rhs);
  EXPECT_EQ(result, rhs & lhs);
}

TEST(ByteVector, BitwiseXor) {
  vector<byte> lhs;
  vector<byte> rhs;
  vector<byte> result;

  lhs.push_back(0xFF);
  rhs.push_back(0xFF);
  result.push_back(0x00);

  EXPECT_EQ(result, lhs ^ rhs);
  EXPECT_EQ(result, rhs ^ lhs);

  EXPECT_EQ(result, lhs ^ lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0xF0);
  rhs.push_back(0x0F);
  result.push_back(0xFF);
  EXPECT_EQ(result, lhs ^ rhs);
  EXPECT_EQ(result, rhs ^ lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0xFF);
  rhs.push_back(0x0F);
  result.push_back(0xF0);
  EXPECT_EQ(result, lhs ^ rhs);
  EXPECT_EQ(result, rhs ^ lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  rhs.push_back(0x0F);
  rhs.push_back(0x0F);
  result.push_back(0xF0);
  result.push_back(0xF0);
  EXPECT_EQ(result, lhs ^ rhs);
  EXPECT_EQ(result, rhs ^ lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0x81);
  lhs.push_back(0x45);
  rhs.push_back(0xFF);
  rhs.push_back(0xFF);
  result.push_back(0x7E);
  result.push_back(0xBA);
  EXPECT_EQ(result, lhs ^ rhs);
  EXPECT_EQ(result, rhs ^ lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0x81);
  lhs.push_back(0x45);
  rhs.push_back(0x00);
  rhs.push_back(0xFF);
  result.push_back(0x81);
  result.push_back(0xBA);
  EXPECT_EQ(result, lhs ^ rhs);
  EXPECT_EQ(result, rhs ^ lhs);
}

TEST(ByteVector, BitwiseOr) {
  vector<byte> lhs;
  vector<byte> rhs;
  vector<byte> result;

  lhs.push_back(0xFF);
  rhs.push_back(0xFF);
  result.push_back(0xFF);

  EXPECT_EQ(result, lhs | rhs);
  EXPECT_EQ(result, rhs | lhs);

  EXPECT_EQ(lhs, lhs | lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0xF0);
  rhs.push_back(0x0F);
  result.push_back(0xFF);
  EXPECT_EQ(result, lhs | rhs);
  EXPECT_EQ(result, rhs | lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0xFF);
  rhs.push_back(0x0F);
  result.push_back(0xFF);
  EXPECT_EQ(result, lhs | rhs);
  EXPECT_EQ(result, rhs | lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0xFF);
  lhs.push_back(0xFF);
  rhs.push_back(0x0F);
  rhs.push_back(0x0F);
  result.push_back(0xFF);
  result.push_back(0xFF);
  EXPECT_EQ(result, lhs | rhs);
  EXPECT_EQ(result, rhs | lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0x81);
  lhs.push_back(0x45);
  rhs.push_back(0xFF);
  rhs.push_back(0xFF);
  result.push_back(0xFF);
  result.push_back(0xFF);
  EXPECT_EQ(result, lhs | rhs);
  EXPECT_EQ(result, rhs | lhs);

  lhs.clear();
  rhs.clear();
  result.clear();
  lhs.push_back(0x81);
  lhs.push_back(0x45);
  rhs.push_back(0x7E);
  rhs.push_back(0xBA);
  result.push_back(0xFF);
  result.push_back(0xFF);
  EXPECT_EQ(result, lhs | rhs);
  EXPECT_EQ(result, rhs | lhs);
}
