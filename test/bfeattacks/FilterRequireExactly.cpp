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

#include "bfeattacks/FilterRequireExactly.h"
#include "bfeattacks/SingleRecord.h"

#include "bloomfilter/BloomFilter.h"
#include "bloomfilter/HashSet.h"
#include "graph/Traversals.h"

// NOTE: These bloom filter setup is also tested as part of the SingleRecord
// tests and therefore only the filter is really tested here

TEST(FilterRequireExactly, Mississippi) {

  // Construct the record
  bloomfilter::HashSetPair hs(10);
  hs.add(hash::MD5).add(hash::SHA3_256);
  bfeattacks::SingleRecord<bloomfilter::BloomFilterStandard> rec(
      bloomfilter::BloomFilterStandard(256, hs));

  // Populate the bloom filter
  rec.bf.insert("mississippi");

  // Construct the graph
  rec.construct_graph("abcdefghijklmnopqrstuvwxyz");

  // Run the traversals
  rec.setup_traversals({ { graph::Traversal::depth_first_search,
                           graph::Traversal::all_simple_paths,
                           graph::Traversal::all_edge_disjoint_paths } });
  rec.run_traversal(graph::Traversal::depth_first_search);
  rec.run_traversal(graph::Traversal::all_simple_paths);
  rec.run_traversal(graph::Traversal::all_edge_disjoint_paths);
  rec.simplify_paths();

  // Apply the filter
  bfeattacks::filter_require_exactly(rec);

  // Verify traversal results

  // This really could be any of the simple paths, but at the moment just
  // returns this one
  vector<string> dfs_paths = {};
  EXPECT_EQ(dfs_paths,
            rec.get_simplified_paths(graph::Traversal::depth_first_search));

  vector<string> simple_paths = { "mippissi", "missippi" };
  EXPECT_EQ(simple_paths,
            rec.get_simplified_paths(graph::Traversal::all_simple_paths));

  vector<string> edge_disjoint = {
    "mipippisissi",   "mipippisisssi",  "mipippissi",    "mipippissisi",
    "mipippisssi",    "mipippisssisi",  "mipipppisissi", "mipipppisisssi",
    "mipipppissi",    "mipipppissisi",  "mipipppisssi",  "mipipppisssisi",
    "mipisissippi",   "mipisissipppi",  "mipisisssippi", "mipisisssipppi",
    "mipissippi",     "mipissipppi",    "mipissisippi",  "mipissisipppi",
    "mipisssippi",    "mipisssipppi",   "mipisssisippi", "mipisssisipppi",
    "mippipisissi",   "mippipisisssi",  "mippipissi",    "mippipissisi",
    "mippipisssi",    "mippipisssisi",  "mippisissi",    "mippisissipi",
    "mippisisssi",    "mippisisssipi",  "mippissi",      "mippissipi",
    "mippissisi",     "mippissisipi",   "mippisssi",     "mippisssipi",
    "mippisssisi",    "mippisssisipi",  "mipppipisissi", "mipppipisisssi",
    "mipppipissi",    "mipppipissisi",  "mipppipisssi",  "mipppipisssisi",
    "mipppisissi",    "mipppisissipi",  "mipppisisssi",  "mipppisisssipi",
    "mipppissi",      "mipppissipi",    "mipppissisi",   "mipppissisipi",
    "mipppisssi",     "mipppisssipi",   "mipppisssisi",  "mipppisssisipi",
    "misipippissi",   "misipippisssi",  "misipipppissi", "misipipppisssi",
    "misippipissi",   "misippipisssi",  "misippissi",    "misippisssi",
    "misipppipissi",  "misipppipisssi", "misipppissi",   "misipppisssi",
    "misissipippi",   "misissipipppi",  "misissippi",    "misissippipi",
    "misissipppi",    "misissipppipi",  "misisssipippi", "misisssipipppi",
    "misisssippi",    "misisssippipi",  "misisssipppi",  "misisssipppipi",
    "missipippi",     "missipippisi",   "missipipppi",   "missipipppisi",
    "missippi",       "missippipi",     "missippipisi",  "missippisi",
    "missipppi",      "missipppipi",    "missipppipisi", "missipppisi",
    "missisipippi",   "missisipipppi",  "missisippi",    "missisippipi",
    "missisipppi",    "missisipppipi",  "misssipippi",   "misssipippisi",
    "misssipipppi",   "misssipipppisi", "misssippi",     "misssippipi",
    "misssippipisi",  "misssippisi",    "misssipppi",    "misssipppipi",
    "misssipppipisi", "misssipppisi",   "misssisipippi", "misssisipipppi",
    "misssisippi",    "misssisippipi",  "misssisipppi",  "misssisipppipi",
  };
  EXPECT_EQ(edge_disjoint, rec.get_simplified_paths(
                               graph::Traversal::all_edge_disjoint_paths));
}

TEST(FilterRequireExactly, Ramakrishna) {
  // Construct the record
  bloomfilter::HashSetPair hs(10);
  hs.add(hash::MD5).add(hash::SHA3_256);
  bfeattacks::SingleRecord<bloomfilter::BloomFilterStandard> rec(
      bloomfilter::BloomFilterStandard(256, hs));

  // Populate the bloom filter
  rec.bf.insert("ramakrishna");

  // Construct the graph
  rec.construct_graph("abcdefghijklmnopqrstuvwxyz");

  // Run the traversals
  rec.setup_traversals({ { graph::Traversal::depth_first_search,
                           graph::Traversal::all_simple_paths,
                           graph::Traversal::all_edge_disjoint_paths } });
  rec.run_traversal(graph::Traversal::depth_first_search);
  rec.run_traversal(graph::Traversal::all_simple_paths);
  rec.run_traversal(graph::Traversal::all_edge_disjoint_paths);
  rec.simplify_paths();

  // Apply the filter
  bfeattacks::filter_require_exactly(rec);

  // Verify traversal results
  vector<string> dfs_paths = {};
  EXPECT_EQ(dfs_paths,
            rec.get_simplified_paths(graph::Traversal::depth_first_search));

  vector<string> simple_paths = { "rakrishnama", "ramakrishna",
                                  "rishnakrama", "rishnamakra" };
  EXPECT_EQ(simple_paths,
            rec.get_simplified_paths(graph::Traversal::all_simple_paths));

  vector<string> edge_disjoint = { "rakrishnama", "ramakrishna",
                                   "rishnakrama", "rishnamakra" };
  EXPECT_EQ(edge_disjoint, rec.get_simplified_paths(
                               graph::Traversal::all_edge_disjoint_paths));
}

TEST(FilterRequireExactly, William) {
  // Construct the record
  bloomfilter::HashSetPair hs(10);
  hs.add(hash::MD5).add(hash::SHA3_256);
  bfeattacks::SingleRecord<bloomfilter::BloomFilterStandard> rec(
      bloomfilter::BloomFilterStandard(256, hs));

  // Populate the bloom filter
  rec.bf.insert("william");

  // Construct the graph
  rec.construct_graph("abcdefghijklmnopqrstuvwxyz");

  // Run the traversals
  rec.setup_traversals({ { graph::Traversal::depth_first_search,
                           graph::Traversal::all_simple_paths,
                           graph::Traversal::all_edge_disjoint_paths } });
  rec.run_traversal(graph::Traversal::depth_first_search);
  rec.run_traversal(graph::Traversal::all_simple_paths);
  rec.run_traversal(graph::Traversal::all_edge_disjoint_paths);
  rec.simplify_paths();

  // Apply the filter
  bfeattacks::filter_require_exactly(rec);

  // Verify traversal results

  // This really could be any of the simple paths, but at the moment just
  // returns this one
  vector<string> dfs_paths = {};
  EXPECT_EQ(dfs_paths,
            rec.get_simplified_paths(graph::Traversal::depth_first_search));

  vector<string> simple_paths = { "william" };
  EXPECT_EQ(simple_paths,
            rec.get_simplified_paths(graph::Traversal::all_simple_paths));

  vector<string> edge_disjoint = { "wililliam", "wilillliam", "william",
                                   "williliam", "willliam",   "willliliam" };
  EXPECT_EQ(edge_disjoint, rec.get_simplified_paths(
                               graph::Traversal::all_edge_disjoint_paths));
}
