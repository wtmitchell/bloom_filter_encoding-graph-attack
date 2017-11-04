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

#include <sstream>
using std::stringstream;
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "bloomfilter/HashSet.h"
using bloomfilter::HashSetSimple;
using bloomfilter::HashSetPair;
#include "hash/HashFactory.h"
#include "util/ByteVector.h"
using util::ByteVector::toByteVector;

#include <iostream>
using std::cout;
using std::endl;

TEST(HashSet, Simple) {
  HashSetSimple hs;

  hs.add(hash::MD5).add(hash::SHA3_256).add(hash::SHA3_224);

  vector<string> names = hs.names();

  EXPECT_EQ(names.size(), 3);

  EXPECT_EQ(names[0], "MD5");
  EXPECT_EQ(names[1], "SHA3-256");
  EXPECT_EQ(names[2], "SHA3-224");

  stringstream ss;
  ss << hs;
  EXPECT_EQ("Simple (k = 3) {MD5, SHA3-256, SHA3-224}", ss.str());

  HashSetSimple::processor p = hs.process("", 64);
  HashSetSimple::processor::iterator i = p.begin();

  // Should return d41d8cd98f00b204e9800998ecf8427e mod 64
  EXPECT_EQ(0x3eu, *i);
  i++;
  // Should return
  // a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a mod 64
  EXPECT_EQ(0xau, *i);
  ++i;
  // Should return 6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7 mod 64
  EXPECT_EQ(0x7u, *i);
  ++i;
  EXPECT_EQ(p.end(), i);
}

TEST(HashSet, SimpleHMAC) {
  HashSetSimple hs;

  hs.addHMAC(hash::SHA_256, toByteVector("4A656665"))
      .addHMAC(hash::SHA_256, toByteVector("B0B0B0"))
      .addHMAC(hash::SHA_256, toByteVector("01010101"));

  vector<string> names = hs.names();

  EXPECT_EQ(names.size(), 3);

  EXPECT_EQ(names[0], "HMAC(SHA-256) key: 4A656665");
  EXPECT_EQ(names[1], "HMAC(SHA-256) key: B0B0B0");
  EXPECT_EQ(names[2], "HMAC(SHA-256) key: 01010101");

  stringstream ss;
  ss << hs;
  EXPECT_EQ("Simple (k = 3) {HMAC(SHA-256) key: 4A656665, HMAC(SHA-256) key: "
            "B0B0B0, HMAC(SHA-256) key: 01010101}",
            ss.str());

  HashSetSimple::processor p = hs.process("", 64);
  HashSetSimple::processor::iterator i = p.begin();

  // Should return ?? mod 64
  EXPECT_EQ(48, *i);
  i++;
  // Should return ??  mod 64
  EXPECT_EQ(47, *i);
  ++i;
  // Should return ?? mod 64
  EXPECT_EQ(49, *i);
  ++i;
  EXPECT_EQ(p.end(), i);
}

TEST(HashSet, Pair) {
  HashSetPair hs(4);

  hs.add(hash::MD5).add(hash::SHA3_256);

  vector<string> names = hs.names();

  EXPECT_EQ(names.size(), 2);

  EXPECT_EQ(names[0], "MD5");
  EXPECT_EQ(names[1], "SHA3-256");

  stringstream ss;
  ss << hs;
  EXPECT_EQ("Pair (k = 4) {MD5, SHA3-256}", ss.str());

  HashSetPair::processor p = hs.process("", 64);
  HashSetPair::processor::iterator i = p.begin();

  EXPECT_NE(p.end(), i);
  EXPECT_EQ(*i, 0x3eu);
  ++i;
  EXPECT_NE(p.end(), i);
  EXPECT_EQ(*i, 0x8u);
  ++i;
  EXPECT_NE(p.end(), i);
  EXPECT_EQ(*i, 0x12u);
  ++i;
  EXPECT_NE(p.end(), i);
  EXPECT_EQ(*i, 0x1cu);
  ++i;
  EXPECT_EQ(p.end(), i);
}
