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

#include <iostream>
using std::cout;
using std::endl;
#include <set>
using std::set;
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
#include <utility>
using std::pair;
#include <vector>
using std::vector;

#include "bloomfilter/BloomFilter.h"
using bloomfilter::BloomFilter;
using bloomfilter::BloomFilterStandard;
#include "bloomfilter/HashSet.h"
using bloomfilter::HashSetSimple;
using bloomfilter::HashSetPair;
#include "bloomfilter/InsertionPolicy.h"
using bloomfilter::InsertionBigramWithSentinel;
using bloomfilter::InsertionTrigramWithSentinel;
#include "hash/HashFactory.h"
#include "util/ByteVector.h"
using util::ByteVector::toByteVector;

TEST(BloomSetFilter, SimpleHashesBigramsWithSentinel) {
  HashSetSimple hs;

  hs.add(hash::MD5).add(hash::SHA3_256).add(hash::SHA3_224);

  BloomFilter<HashSetSimple, InsertionBigramWithSentinel, true> bf(20, hs);

  string expected_contents = "00000000000000000000";
  string contents;
  boost::to_string(bf.raw(), contents);
  EXPECT_EQ(expected_contents, contents);
  EXPECT_EQ(0, bf.count());

  string string_form = "BloomFilter:\n"
                       "Size (m) = 20\n"
                       "Hashes: Simple (k = 3) {MD5, SHA3-256, SHA3-224}\n"
                       "Insertion policy: 2-gram with start sentinel '^' and "
                       "with stop sentinel '$'\n"
                       "Contents: 00000000000000000000\n"
                       "Actually Inserted(0): {}\n"
                       "Actual Members(0): {}\n"
                       "All: ? No alphabet specified\n"
                       "False positives: ?";

  stringstream ss;
  ss << bf;
  EXPECT_EQ(string_form, ss.str());

  EXPECT_FALSE(bf.contains("test"));
  EXPECT_FALSE(bf.contains_exactly("test"));
  bf.insert("test");
  EXPECT_TRUE(bf.contains("test"));
  EXPECT_TRUE(bf.contains_exactly("test"));

  expected_contents = "10001010011010011101";
  boost::to_string(bf.raw(), contents);
  EXPECT_EQ(expected_contents, contents);
  EXPECT_EQ(10, bf.count());

  string_form = "BloomFilter:\n"
                "Size (m) = 20\n"
                "Hashes: Simple (k = 3) {MD5, SHA3-256, SHA3-224}\n"
                "Insertion policy: 2-gram with start sentinel '^' and with "
                "stop sentinel '$'\n"
                "Contents: 10111001011001010001\n"
                "Actually Inserted(1): {test}\n"
                "Actual Members(5): {^t, es, st, t$, te}\n"
                "All: ? No alphabet specified\n"
                "False positives: ?";

  stringstream ss2;
  ss2 << bf;
  EXPECT_EQ(string_form, ss2.str());
}

TEST(BloomSetFilter, PairHashesBigramsWithSentinel) {
  HashSetPair hs(4);

  hs.add(hash::MD5).add(hash::SHA3_256);

  BloomFilter<HashSetPair, InsertionBigramWithSentinel, true> bf(20, hs);

  string expected_contents = "00000000000000000000";
  string contents;
  boost::to_string(bf.raw(), contents);
  EXPECT_EQ(expected_contents, contents);
  EXPECT_EQ(0, bf.count());

  string string_form = "BloomFilter:\n"
                       "Size (m) = 20\n"
                       "Hashes: Pair (k = 4) {MD5, SHA3-256}\n"
                       "Insertion policy: 2-gram with start sentinel '^' and "
                       "with stop sentinel '$'\n"
                       "Contents: 00000000000000000000\n"
                       "Actually Inserted(0): {}\n"
                       "Actual Members(0): {}\n"
                       "All: ? No alphabet specified\n"
                       "False positives: ?";

  stringstream ss;
  ss << bf;
  EXPECT_EQ(string_form, ss.str());

  EXPECT_FALSE(bf.contains("test"));
  EXPECT_FALSE(bf.contains_exactly("test"));
  bf.insert("test");
  EXPECT_TRUE(bf.contains("test"));
  EXPECT_TRUE(bf.contains_exactly("test"));

  expected_contents = "01001011111111010001";
  boost::to_string(bf.raw(), contents);
  EXPECT_EQ(expected_contents, contents);
  EXPECT_EQ(12, bf.count());

  string_form = "BloomFilter:\n"
                "Size (m) = 20\n"
                "Hashes: Pair (k = 4) {MD5, SHA3-256}\n"
                "Insertion policy: 2-gram with start sentinel '^' and with "
                "stop sentinel '$'\n"
                "Contents: 10001011111111010010\n"
                "Actually Inserted(1): {test}\n"
                "Actual Members(5): {^t, es, st, t$, te}\n"
                "All: ? No alphabet specified\n"
                "False positives: ?";

  stringstream ss2;
  ss2 << bf;
  EXPECT_EQ(string_form, ss2.str());
}

TEST(BloomSetFilter, PairHashesHMACBigramsWithSentinel) {
  HashSetPair hs(4);

  hs.addHMAC(hash::SHA_256, toByteVector("010101"))
      .addHMAC(hash::SHA_256, toByteVector("101010"));

  BloomFilter<HashSetPair, InsertionBigramWithSentinel, true> bf(20, hs);

  string expected_contents = "00000000000000000000";
  string contents;
  boost::to_string(bf.raw(), contents);
  EXPECT_EQ(expected_contents, contents);
  EXPECT_EQ(0, bf.count());

  string string_form = "BloomFilter:\n"
                       "Size (m) = 20\n"
                       "Hashes: Pair (k = 4) {HMAC(SHA-256) key: 010101, "
                       "HMAC(SHA-256) key: 101010}\n"
                       "Insertion policy: 2-gram with start sentinel '^' and "
                       "with stop sentinel '$'\n"
                       "Contents: 00000000000000000000\n"
                       "Actually Inserted(0): {}\n"
                       "Actual Members(0): {}\n"
                       "All: ? No alphabet specified\n"
                       "False positives: ?";

  stringstream ss;
  ss << bf;
  EXPECT_EQ(string_form, ss.str());

  EXPECT_FALSE(bf.contains("test"));
  EXPECT_FALSE(bf.contains_exactly("test"));
  bf.insert("test");
  EXPECT_TRUE(bf.contains("test"));
  EXPECT_TRUE(bf.contains_exactly("test"));

  expected_contents = "11111010100100101001";
  boost::to_string(bf.raw(), contents);
  EXPECT_EQ(expected_contents, contents);
  EXPECT_EQ(11, bf.count());

  string_form = "BloomFilter:\n"
                "Size (m) = 20\n"
                "Hashes: Pair (k = 4) {HMAC(SHA-256) key: 010101, "
                "HMAC(SHA-256) key: 101010}\n"
                "Insertion policy: 2-gram with start sentinel '^' and with "
                "stop sentinel '$'\n"
                "Contents: 10010100100101011111\n"
                "Actually Inserted(1): {test}\n"
                "Actual Members(5): {^t, es, st, t$, te}\n"
                "All: ? No alphabet specified\n"
                "False positives: ?";

  stringstream ss2;
  ss2 << bf;
  EXPECT_EQ(string_form, ss2.str());
}

TEST(BloomFilter, PotentialMembersPair) {
  HashSetPair hs(10);

  hs.add(hash::MD5).add(hash::SHA3_256);

  BloomFilter<HashSetPair, InsertionBigramWithSentinel, true> bf(256, hs);

  EXPECT_FALSE(bf.contains_exactly("test"));
  bf.insert("test");
  EXPECT_TRUE(bf.contains_exactly("test"));
  bf.insert("foo");
  EXPECT_FALSE(bf.contains_exactly("test"));

  string expected_contents =
      "100000001010000011010011010010100010011010100010000111000010001011011000"
      "100010000000000011010000000000000010011001100000000000000100000011001100"
      "000010010001000010000011000100000000000100111101110011100001011001000000"
      "0001101010000101000100010011000001001101";

  string contents;
  boost::to_string(bf.raw(), contents);
  EXPECT_EQ(expected_contents, contents);
  EXPECT_EQ(77, bf.count());

  // Return ignored since it is cached inside the bf. Needs to be called prior
  // to the printout
  bf.potential_members("abcdefghijklmnopqrstuvwxyz");

  string string_form =
      "BloomFilter:\n"
      "Size (m) = 256\n"
      "Hashes: Pair (k = 10) {MD5, SHA3-256}\n"
      "Insertion policy: 2-gram with start sentinel '^' and with stop sentinel "
      "'$'\n"
      "Contents: "
      "101100100000110010001000101000010101100000000010011010000111001110111100"
      "100000000000100011000001000010001001000000110011000000100000000000000110"
      "011001000000000000001011000000000001000100011011010001000011100001000101"
      "0110010001010010110010110000010100000001\n"
      "Actually Inserted(2): {test, foo}\n"
      "Actual Members(9): {^f, ^t, es, fo, o$, oo, st, t$, te}\n"
      "All(10): {^f, ^t, bg, es, fo, o$, oo, st, t$, te}\n"
      "False positives(1): {bg}";

  stringstream ss;
  ss << bf;
  EXPECT_EQ(string_form, ss.str());
  EXPECT_EQ(77, bf.count());
}

TEST(BloomFilter, PotentialMembersTriple) {
  HashSetPair hs(10);

  hs.add(hash::MD5).add(hash::SHA3_256);

  BloomFilter<HashSetPair, InsertionTrigramWithSentinel, true> bf(256, hs);

  EXPECT_FALSE(bf.contains_exactly("test"));
  bf.insert("test");
  EXPECT_TRUE(bf.contains_exactly("test"));
  bf.insert("foo");
  EXPECT_FALSE(bf.contains_exactly("test"));

  bf.potential_members("abcdefghijklmnopqrstuvwxyz");

  string string_form =
      "BloomFilter:\n"
      "Size (m) = 256\n"
      "Hashes: Pair (k = 10) {MD5, SHA3-256}\n"
      "Insertion policy: 3-gram with start sentinel '^' and with stop sentinel "
      "'$'\n"
      "Contents: "
      "010101000001000000001001001110000100100001010000010010000001100000001101"
      "001101101000011000001010101100011001111001011000001100011011110001000000"
      "000110110000000000000100010101010001101000010000100010000000011100001011"
      "1111111111011101101000010010000010000000\n"
      "Actually Inserted(2): {test, foo}\n"
      "Actual Members(11): {^^f, ^^t, ^fo, ^te, est, foo, o$$, oo$, st$, t$$, "
      "tes}\n"
      "All(64): {^^f, ^^t, ^cp, ^eu, ^fo, ^so, ^te, ^tn, bme, bnj, bte, bzd, "
      "deo, dex, epb, est, fat, foo, fru, gkz, ijk, iwn, jhb, kgs, kld, kze, "
      "lbn, lrv, ltt, meg, msm, ndx, nxn, o$$, odb, old, oo$, pda, psa, qvv, "
      "qwa, rpm, soo, st$, sua, szc, t$$, tea, tes, tfy, tiy, top, uby, uce, "
      "uea, ujr, vrh, vw$, wex, wgf, xuf, xyh, ypl, zlt}\nFalse positives(53): "
      "{^cp, ^eu, ^so, ^tn, bme, bnj, bte, bzd, deo, dex, epb, fat, fru, gkz, "
      "ijk, iwn, jhb, kgs, kld, kze, lbn, lrv, ltt, meg, msm, ndx, nxn, odb, "
      "old, pda, psa, qvv, qwa, rpm, soo, sua, szc, tea, tfy, tiy, top, uby, "
      "uce, uea, ujr, vrh, vw$, wex, wgf, xuf, xyh, ypl, zlt}";

  stringstream ss;
  ss << bf;
  EXPECT_EQ(string_form, ss.str());
  EXPECT_EQ(90, bf.count());
}

TEST(BloomFilter, DiceCoefficient) {
  HashSetPair hs(2);

  hs.add(hash::MD5).add(hash::SHA3_256);

  BloomFilter<HashSetPair, InsertionBigramWithSentinel, true> bf1(10, hs);
  BloomFilter<HashSetPair, InsertionBigramWithSentinel, true> bf2(10, hs);

  // Manipulate the bitset directly to get a known Dice Coeff
  boost::dynamic_bitset<> &bf1_raw =
      const_cast<boost::dynamic_bitset<> &>(bf1.raw());
  boost::dynamic_bitset<> &bf2_raw =
      const_cast<boost::dynamic_bitset<> &>(bf2.raw());

  bf1_raw.set(0);
  bf1_raw.set(2);
  bf1_raw.set(4);
  bf1_raw.set(6);
  bf1_raw.set(8);

  EXPECT_EQ(5, bf1.count());

  bf2_raw.set(0);
  bf2_raw.set(1);
  bf2_raw.set(2);
  bf2_raw.set(4);
  bf2_raw.set(5);
  bf2_raw.set(6);
  bf2_raw.set(8);
  bf2_raw.set(9);

  EXPECT_EQ(8, bf2.count());

  typedef pair<boost::dynamic_bitset<>::size_type,
               boost::dynamic_bitset<>::size_type> dice_coeff;
  dice_coeff dice = bloomfilter::dice_coefficient(bf1, bf2);

  EXPECT_EQ(dice.first, 10);
  EXPECT_EQ(dice.second, 13);
}

TEST(BloomFilter, TREncodingExample) {
  const auto data = "SMITH";
  const auto k = 3u;
  const auto m = 35u;

  auto key1 = toByteVector("1111111111111111111111111111111111111111111111111111111111111111");
  auto key2 = toByteVector("2222222222222222222222222222222222222222222222222222222222222222");

  // Print out hashes of individual bigrams
  auto hash1 = hash::getHMACHash(hash::SHA_256, key1);
  auto hash2 = hash::getHMACHash(hash::SHA_256, key2);

  EXPECT_EQ("HMAC(SHA-256) key: 1111111111111111111111111111111111111111111111111111111111111111",
	    hash1->name());
  EXPECT_EQ("HMAC(SHA-256) key: 2222222222222222222222222222222222222222222222222222222222222222",
	    hash2->name());

  InsertionBigramWithSentinel ip;
  auto bigrams = ip.process(data);

  auto i = bigrams.begin(), e = bigrams.end();
  EXPECT_EQ("^S", *i);
  EXPECT_NE(i, e);
  EXPECT_EQ(21, hash1->calculate(*i) % m);
  EXPECT_EQ(10, hash2->calculate(*i) % m);
  ++i;
  EXPECT_EQ("SM", *i);
  EXPECT_NE(i, e);
  EXPECT_EQ(23, hash1->calculate(*i) % m);
  EXPECT_EQ(2, hash2->calculate(*i) % m);
  ++i;
  EXPECT_EQ("MI", *i);
  EXPECT_NE(i, e);
  EXPECT_EQ(25, hash1->calculate(*i) % m);
  EXPECT_EQ(26, hash2->calculate(*i) % m);
  ++i;
  EXPECT_EQ("IT", *i);
  EXPECT_NE(i, e);
  EXPECT_EQ(29, hash1->calculate(*i) % m);
  EXPECT_EQ(14, hash2->calculate(*i) % m);
  ++i;
  EXPECT_EQ("TH", *i);
  EXPECT_NE(i, e);
  EXPECT_EQ(4, hash1->calculate(*i) % m);
  EXPECT_EQ(19, hash2->calculate(*i) % m);
  ++i;
  EXPECT_EQ("H$", *i);
  EXPECT_NE(i, e);
  EXPECT_EQ(12, hash1->calculate(*i) % m);
  EXPECT_EQ(31, hash2->calculate(*i) % m);
  ++i;
  EXPECT_EQ(i, e);

  // Construct the filter
  HashSetPair hs(k);
  hs.addHMAC(hash::SHA_256, key1).addHMAC(hash::SHA_256, key2);
  BloomFilterStandard filter(m, hs);

  auto contents =
      "BloomFilter:\n"
      "Size (m) = 35\n"
      "Hashes: Pair (k = 3) {HMAC(SHA-256) key: "
      "1111111111111111111111111111111111111111111111111111111111111111, "
      "HMAC(SHA-256) key: "
      "2222222222222222222222222222222222222222222222222222222222222222}\n"
      "Insertion policy: 2-gram with start sentinel '^' and with stop sentinel "
      "'$'\n"
      "Contents: 00000000000000000000000000000000000\n"
      "Actually Inserted(0): {}\n"
      "Actual Members(0): {}\n"
      "All: ? No alphabet specified\n"
      "False positives: ?";

  stringstream ss;
  ss << filter;
  EXPECT_EQ(contents, ss.str());

  // Insert data
  filter.insert(data);

  // Verify contents
  auto contents2 =
      "BloomFilter:\n"
      "Size (m) = 35\n"
      "Hashes: Pair (k = 3) {HMAC(SHA-256) key: "
      "1111111111111111111111111111111111111111111111111111111111111111, "
      "HMAC(SHA-256) key: "
      "2222222222222222222222222222222222222222222222222222222222222222}\n"
      "Insertion policy: 2-gram with start sentinel '^' and with stop sentinel "
      "'$'\n"
      "Contents: 00001011100010001000011101010101000\n"
      "Actually Inserted(1): {SMITH}\n"
      "Actual Members(6): {H$, IT, MI, SM, TH, ^S}\n"
      "All: ? No alphabet specified\n"
      "False positives: ?";

  stringstream ss2;
  ss2 << filter;
  EXPECT_EQ(contents2, ss2.str());
}
