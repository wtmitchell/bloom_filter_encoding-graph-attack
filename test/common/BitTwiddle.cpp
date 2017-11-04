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
///
/// \file
/// \brief This file contains unit tests for BitTwiddle routines
///
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

#include <climits>

#include "common/BitTwiddle.h"

using namespace common;

// Test fixture for type parameterized tests
template <typename T>
class BitTwiddleTest : public ::testing::Test { };

// Test BitTuple with multiple Storage types
typedef ::testing::Types<uint8_t, uint16_t, uint32_t, uint64_t> BitTwiddleTestTypes;
TYPED_TEST_CASE(BitTwiddleTest, BitTwiddleTestTypes);


TYPED_TEST(BitTwiddleTest, popCount) {
  EXPECT_EQ(0, popCount(TypeParam(0)));
  EXPECT_EQ(1, popCount(TypeParam(1)));
  EXPECT_EQ(1, popCount(TypeParam(2)));
  EXPECT_EQ(2, popCount(TypeParam(3)));
  EXPECT_EQ(sizeof(TypeParam) * CHAR_BIT, popCount(TypeParam(-1)));
}
