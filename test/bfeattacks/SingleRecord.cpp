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

#include <array>
using std::array;
#include <iostream>
using std::cout;
using std::endl;
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
#include <utility>
using std::pair;
#include <vector>
using std::vector;

#include "bfeattacks/SingleRecord.h"

#include "bloomfilter/BloomFilter.h"
#include "bloomfilter/HashSet.h"
#include "graph/Traversals.h"

TEST(SingleRecord, NoFiltersMississippi) {
  // Construct the record
  bloomfilter::HashSetPair hs(10);
  hs.add(hash::MD5).add(hash::SHA3_256);
  bfeattacks::SingleRecord<bloomfilter::BloomFilterStandard> rec(
      bloomfilter::BloomFilterStandard(256, hs));

  // Populate the bloom filter
  rec.bf.insert("mississippi");

  // Construct the graph
  rec.construct_graph("abcdefghijklmnopqrstuvwxyz");

  const string expected_bf =
      "BloomFilter:\n"
      "Size (m) = 256\n"
      "Hashes: Pair (k = 10) {MD5, SHA3-256}\n"
      "Insertion policy: 2-gram with start sentinel '^' and with stop sentinel "
      "'$'\n"
      "Contents: "
      "000010010010110100101010000100001010100001101010000001101110000010000100"
      "101100001001100110000011010000111000000001100100000011100101100000000000"
      "100010000000110010100100000011010000000001001000000011010000010010010100"
      "0111001000000100010000000110010000000100\n"
      "Actually Inserted(1): {mississippi}\n"
      "Actual Members(9): {^m, i$, ip, is, mi, pi, pp, si, ss}\n"
      "All(10): {^m, i$, ip, is, mi, pi, pp, si, sq, ss}\n"
      "False positives(1): {sq}";

  stringstream ss;
  ss << rec.bf;
  EXPECT_EQ(expected_bf, ss.str());

  // Run the traversals
  rec.setup_traversals({ { graph::Traversal::depth_first_search,
                           graph::Traversal::all_simple_paths,
                           graph::Traversal::all_edge_disjoint_paths } });
  rec.run_traversal(graph::Traversal::depth_first_search);
  rec.run_traversal(graph::Traversal::all_simple_paths);
  rec.run_traversal(graph::Traversal::all_edge_disjoint_paths);
  rec.simplify_paths();

  // Verify traversal results

  // This really could be any of the simple paths, but at the moment just
  // returns this one
  vector<string> dfs_paths = { { "mi" } };
  EXPECT_EQ(dfs_paths,
            rec.get_simplified_paths(graph::Traversal::depth_first_search));

  vector<string> simple_paths = {
    { "mi",   "mipi",   "mipisi",  "mipissi", "mippi",   "mippisi", "mippissi",
      "misi", "misipi", "misippi", "missi",   "missipi", "missippi" }
  };
  EXPECT_EQ(simple_paths,
            rec.get_simplified_paths(graph::Traversal::all_simple_paths));

  vector<string> edge_disjoint = {
    { "mi",             "mipi",           "mipippi",        "mipippisi",
      "mipippisissi",   "mipippisisssi",  "mipippissi",     "mipippissisi",
      "mipippisssi",    "mipippisssisi",  "mipipppi",       "mipipppisi",
      "mipipppisissi",  "mipipppisisssi", "mipipppissi",    "mipipppissisi",
      "mipipppisssi",   "mipipppisssisi", "mipisi",         "mipisippi",
      "mipisipppi",     "mipisissi",      "mipisissippi",   "mipisissipppi",
      "mipisisssi",     "mipisisssippi",  "mipisisssipppi", "mipissi",
      "mipissippi",     "mipissipppi",    "mipissisi",      "mipissisippi",
      "mipissisipppi",  "mipisssi",       "mipisssippi",    "mipisssipppi",
      "mipisssisi",     "mipisssisippi",  "mipisssisipppi", "mippi",
      "mippipi",        "mippipisi",      "mippipisissi",   "mippipisisssi",
      "mippipissi",     "mippipissisi",   "mippipisssi",    "mippipisssisi",
      "mippisi",        "mippisipi",      "mippisissi",     "mippisissipi",
      "mippisisssi",    "mippisisssipi",  "mippissi",       "mippissipi",
      "mippissisi",     "mippissisipi",   "mippisssi",      "mippisssipi",
      "mippisssisi",    "mippisssisipi",  "mipppi",         "mipppipi",
      "mipppipisi",     "mipppipisissi",  "mipppipisisssi", "mipppipissi",
      "mipppipissisi",  "mipppipisssi",   "mipppipisssisi", "mipppisi",
      "mipppisipi",     "mipppisissi",    "mipppisissipi",  "mipppisisssi",
      "mipppisisssipi", "mipppissi",      "mipppissipi",    "mipppissisi",
      "mipppissisipi",  "mipppisssi",     "mipppisssipi",   "mipppisssisi",
      "mipppisssisipi", "misi",           "misipi",         "misipippi",
      "misipippissi",   "misipippisssi",  "misipipppi",     "misipipppissi",
      "misipipppisssi", "misipissi",      "misipisssi",     "misippi",
      "misippipi",      "misippipissi",   "misippipisssi",  "misippissi",
      "misippisssi",    "misipppi",       "misipppipi",     "misipppipissi",
      "misipppipisssi", "misipppissi",    "misipppisssi",   "misissi",
      "misissipi",      "misissipippi",   "misissipipppi",  "misissippi",
      "misissippipi",   "misissipppi",    "misissipppipi",  "misisssi",
      "misisssipi",     "misisssipippi",  "misisssipipppi", "misisssippi",
      "misisssippipi",  "misisssipppi",   "misisssipppipi", "missi",
      "missipi",        "missipippi",     "missipippisi",   "missipipppi",
      "missipipppisi",  "missipisi",      "missippi",       "missippipi",
      "missippipisi",   "missippisi",     "missipppi",      "missipppipi",
      "missipppipisi",  "missipppisi",    "missisi",        "missisipi",
      "missisipippi",   "missisipipppi",  "missisippi",     "missisippipi",
      "missisipppi",    "missisipppipi",  "misssi",         "misssipi",
      "misssipippi",    "misssipippisi",  "misssipipppi",   "misssipipppisi",
      "misssipisi",     "misssippi",      "misssippipi",    "misssippipisi",
      "misssippisi",    "misssipppi",     "misssipppipi",   "misssipppipisi",
      "misssipppisi",   "misssisi",       "misssisipi",     "misssisipippi",
      "misssisipipppi", "misssisippi",    "misssisippipi",  "misssisipppi",
      "misssisipppipi" }
  };
  EXPECT_EQ(edge_disjoint, rec.get_simplified_paths(
                               graph::Traversal::all_edge_disjoint_paths));
}

TEST(SingleRecord, NoFiltersRamakrishna) {
  // Construct the record
  bloomfilter::HashSetPair hs(10);
  hs.add(hash::MD5).add(hash::SHA3_256);
  bfeattacks::SingleRecord<bloomfilter::BloomFilterStandard> rec(
      bloomfilter::BloomFilterStandard(256, hs));

  // Populate the bloom filter
  rec.bf.insert("ramakrishna");

  // Construct the graph
  rec.construct_graph("abcdefghijklmnopqrstuvwxyz");

  const string expected_bf =
      "BloomFilter:\n"
      "Size (m) = 256\n"
      "Hashes: Pair (k = 10) {MD5, SHA3-256}\n"
      "Insertion policy: 2-gram with start sentinel '^' and with stop sentinel "
      "'$'\n"
      "Contents: "
      "000011000101010010101000101000101000000010111000001000100001101011000001"
      "010111111000000010111110010010110011001111100000000100101101000000000010"
      "100110011010000000110100001001001001001010110110101100110110000010011010"
      "0000101000011011110000000111101010000010\n"
      "Actually Inserted(1): {ramakrishna}\n"
      "Actual Members(12): {^r, a$, ak, am, hn, is, kr, ma, na, ra, ri, sh}\n"
      "All(13): {^r, a$, ak, am, hn, is, kr, ma, na, ra, ri, sh, zr}\n"
      "False positives(1): {zr}";

  stringstream ss;
  ss << rec.bf;
  EXPECT_EQ(expected_bf, ss.str());

  // Run the traversals
  rec.setup_traversals({ { graph::Traversal::depth_first_search,
                           graph::Traversal::all_simple_paths,
                           graph::Traversal::all_edge_disjoint_paths } });
  rec.run_traversal(graph::Traversal::depth_first_search);
  rec.run_traversal(graph::Traversal::all_simple_paths);
  rec.run_traversal(graph::Traversal::all_edge_disjoint_paths);
  rec.simplify_paths();

  // Verify traversal results

  // This really could be any of the simple paths, but at the moment just
  // returns this one
  vector<string> dfs_paths = { { "ra" } };
  EXPECT_EQ(dfs_paths,
            rec.get_simplified_paths(graph::Traversal::depth_first_search));

  vector<string> simple_paths = {
    { "ra",     "rakrishna", "rakrishnama", "rama",     "ramakrishna",
      "rishna", "rishnakra", "rishnakrama", "rishnama", "rishnamakra" }
  };
  EXPECT_EQ(simple_paths,
            rec.get_simplified_paths(graph::Traversal::all_simple_paths));

  vector<string> edge_disjoint = {
    { "ra",          "rakra",    "rakrama",     "rakrishna", "rakrishnama",
      "rama",        "ramakra",  "ramakrishna", "rishna",    "rishnakra",
      "rishnakrama", "rishnama", "rishnamakra" }
  };
  EXPECT_EQ(edge_disjoint, rec.get_simplified_paths(
                               graph::Traversal::all_edge_disjoint_paths));
}

TEST(SingleRecord, NoFiltersWilliam) {
  // Construct the record
  bloomfilter::HashSetPair hs(10);
  hs.add(hash::MD5).add(hash::SHA3_256);
  bfeattacks::SingleRecord<bloomfilter::BloomFilterStandard> rec(
      bloomfilter::BloomFilterStandard(256, hs));

  // Populate the bloom filter
  rec.bf.insert("william");

  // Construct the graph
  rec.construct_graph("abcdefghijklmnopqrstuvwxyz");

  const string expected_bf =
      "BloomFilter:\n"
      "Size (m) = 256\n"
      "Hashes: Pair (k = 10) {MD5, SHA3-256}\n"
      "Insertion policy: 2-gram with start sentinel '^' and with stop sentinel "
      "'$'\n"
      "Contents: "
      "100000110000010001000011101110100000001000000010000010001011100000000001"
      "000110000100000000110010111100101001001000000000101100000010010000001000"
      "000000100000000000101001111000010101000000001000100100000100001010110010"
      "0000100000011000000110110001100000001100\n"
      "Actually Inserted(1): {william}\n"
      "Actual Members(8): {^w, am, ia, il, li, ll, m$, wi}\n"
      "All(8): {^w, am, ia, il, li, ll, m$, wi}\n"
      "False positives(0): {}";

  stringstream ss;
  ss << rec.bf;
  EXPECT_EQ(expected_bf, ss.str());

  // Run the traversals
  rec.setup_traversals({ { graph::Traversal::depth_first_search,
                           graph::Traversal::all_simple_paths,
                           graph::Traversal::all_edge_disjoint_paths } });
  rec.run_traversal(graph::Traversal::depth_first_search);
  rec.run_traversal(graph::Traversal::all_simple_paths);
  rec.run_traversal(graph::Traversal::all_edge_disjoint_paths);
  rec.simplify_paths();

  // Verify traversal results

  // This really could be any of the simple paths, but at the moment just
  // returns this one
  vector<string> dfs_paths = { { "wiam" } };
  EXPECT_EQ(dfs_paths,
            rec.get_simplified_paths(graph::Traversal::depth_first_search));

  vector<string> simple_paths = { { "wiam", "wiliam", "william" } };
  EXPECT_EQ(simple_paths,
            rec.get_simplified_paths(graph::Traversal::all_simple_paths));

  vector<string> edge_disjoint = { { "wiam",       "wiliam",    "wililliam",
                                     "wilillliam", "william",   "williliam",
                                     "willliam",   "willliliam" } };
  EXPECT_EQ(edge_disjoint, rec.get_simplified_paths(
                               graph::Traversal::all_edge_disjoint_paths));
}
