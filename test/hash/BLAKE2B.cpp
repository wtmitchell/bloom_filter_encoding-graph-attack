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

#include "hash/HashFactory.h"

#include "HashCommon.h"
using test::hash::testHash;
using test::hash::testHashBytes;

// Source of these test vectors is
// Botan's src/tests/data/hash/blake2b.vec

TEST(BLAKE2B_224, BotanTestVectors) {
  auto blake2b = hash::getHash(hash::BLAKE2B_224);

  EXPECT_EQ(blake2b->name(), "Blake2b(224)");

  testHashBytes(blake2b, "",
                "836cc68931c2e4e3e838602eca1902591d216837bafddfe6f0c8cb07");
}

TEST(BLAKE2B_256, BotanTestVectors) {
  auto blake2b = hash::getHash(hash::BLAKE2B_256);

  EXPECT_EQ(blake2b->name(), "Blake2b(256)");

  testHashBytes(
      blake2b, "",
      "0e5751c026e543b2e8ab2eb06099daa1d1e5df47778f7787faab45cdf12fe3a8");
}

TEST(BLAKE2B_384, BotanTestVectors) {
  auto blake2b = hash::getHash(hash::BLAKE2B_384);

  EXPECT_EQ(blake2b->name(), "Blake2b(384)");

  testHashBytes(blake2b, "", "b32811423377f52d7862286ee1a72ee540524380fda1724a6"
                             "f25d7978c6fd3244a6caf0498812673c5e05ef583825100");
}

TEST(BLAKE2B_512, BotanTestVectors) {
  auto blake2b = hash::getHash(hash::BLAKE2B_512);

  EXPECT_EQ(blake2b->name(), "Blake2b(512)");

  testHashBytes(blake2b, "", "786a02f742015903c6c6fd852552d272912f4740e15847618"
                             "a86e217f71f5419d25e1031afee585313896444934eb04b90"
                             "3a685b1448b755d56f701afe9be2ce");
}
