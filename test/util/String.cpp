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

#include "util/String.h"

TEST(String, stripNonAlpha) {
  EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", util::stripNonAlpha("abcdefghijklmnopqrstuvwxyz"));
  EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", util::stripNonAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  EXPECT_EQ("", util::stripNonAlpha("`~!@#$%^&*()-_=+[{]}\\|;:'\",<.>/?"));
}

TEST(String, toLowerCase) {
  EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", util::toLowerCase("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", util::toLowerCase("abcdefghijklmnopqrstuvwxyz"));
}
