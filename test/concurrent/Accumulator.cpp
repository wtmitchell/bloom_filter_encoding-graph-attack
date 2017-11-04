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

#include "concurrent/Accumulator.h"

TEST(Accumulator, Basics) {
  concurrent::Accumulator<int> acc;

  EXPECT_EQ(0, acc.count(1));
  acc.add(1);
  EXPECT_EQ(1, acc.count(1));
  acc.add(1);
  EXPECT_EQ(2, acc.count(1));
  EXPECT_EQ(2, acc.count());

  concurrent::Accumulator<int> acc2;
  acc2.add(1);
  EXPECT_EQ(1, acc2.count(1));
  acc2.add(2);
  EXPECT_EQ(1, acc2.count(2));
  acc2.add(3);
  EXPECT_EQ(1, acc2.count(3));
  EXPECT_EQ(3, acc2.count());


  EXPECT_EQ(2, acc.count(1));
  EXPECT_EQ(0, acc.count(2));
  EXPECT_EQ(0, acc.count(3));
  EXPECT_EQ(2, acc.count());
  acc.combine(acc2);
  EXPECT_EQ(3, acc.count(1));
  EXPECT_EQ(1, acc.count(2));
  EXPECT_EQ(1, acc.count(3));
  EXPECT_EQ(5, acc.count());
  acc += acc2;
  EXPECT_EQ(4, acc.count(1));
  EXPECT_EQ(2, acc.count(2));
  EXPECT_EQ(2, acc.count(3));
  EXPECT_EQ(8, acc.count());

}
