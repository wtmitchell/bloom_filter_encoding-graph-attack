//===-- test/hash/HashCommon.cpp - Common testing for hashes --------------===//
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
/// \brief This file contains common testing routines for hash functions
///
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

#include "HashCommon.h"

#include "hash/HashFunction.h"
using ::hash::HashFunction;
using ::hash::IncrementalHashFunction;
#include "util/ByteVector.h"
using util::ByteVector::isHexString;
using util::ByteVector::toByteVector;
#include "util/Types.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

void test::hash::testHash(const std::unique_ptr<IncrementalHashFunction> &hash,
                          const string data, const string expected_hash) {
  // Validate expected_hash as mush as possible
  EXPECT_TRUE(isHexString(expected_hash));
  // Expected hash divided by two because it should be in hex and thus two
  // chars per byte of output
  EXPECT_EQ(hash->output_length(), expected_hash.size() / 2);
  // Convert to vector<byte> to match output of HashFunction
  vector<byte> expected = toByteVector(expected_hash);

  // TODO: Test other forms of calculate
  vector<byte> calculated = hash->calculate(data);

  EXPECT_EQ(expected, calculated);
}

void test::hash::testHashBytes(const std::unique_ptr<IncrementalHashFunction> &hash,
                          const string data, const string expected_hash) {
  // Validate expected_hash as mush as possible
  EXPECT_TRUE(isHexString(expected_hash));
  // Expected hash divided by two because it should be in hex and thus two
  // chars per byte of output
  EXPECT_EQ(hash->output_length(), expected_hash.size() / 2);
  // Convert to vector<byte> to match output of HashFunction
  vector<byte> expected = toByteVector(expected_hash);

  // TODO: Test other forms of calculate
  vector<byte> calculated = hash->calculate(toByteVector(data));

  EXPECT_EQ(expected, calculated);
}

void test::hash::testHashIterated(
    const std::unique_ptr<IncrementalHashFunction> &hash, const string data,
    const int repetitions, const string expected_hash) {
  // Validate expected_hash as mush as possible
  EXPECT_TRUE(isHexString(expected_hash));
  // Expected hash divided by two because it should be in hex and thus two
  // chars per byte of output
  EXPECT_EQ(hash->output_length(), expected_hash.size() / 2);
  // Convert to vector<byte> to match output of HashFunction
  vector<byte> expected = toByteVector(expected_hash);

  // TODO: Test other forms of calculate

  for (auto i = 1; i < repetitions; ++i) {
    hash->update(data);
  }

  vector<byte> calculated = hash->calculate(data);

  EXPECT_EQ(expected, calculated);
}
