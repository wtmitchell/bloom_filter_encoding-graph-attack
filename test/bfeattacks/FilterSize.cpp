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

#include "bfeattacks/FilterSize.h"
#include "bfeattacks/SingleRecord.h"

#include "bloomfilter/BloomFilter.h"
#include "bloomfilter/HashSet.h"
#include "graph/Traversals.h"

// NOTE: These bloom filter setup is also tested as part of the SingleRecord
// tests and therefore only the filter is really tested here

TEST(FilterSize, Mississippi) {

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
  bfeattacks::filter_size(rec, 10, 12);

  // Verify traversal results

  // This really could be any of the simple paths, but at the moment just
  // returns this one
  vector<string> dfs_paths = {};
  EXPECT_EQ(dfs_paths,
            rec.get_simplified_paths(graph::Traversal::depth_first_search));

  vector<string> simple_paths = {};
  EXPECT_EQ(simple_paths,
            rec.get_simplified_paths(graph::Traversal::all_simple_paths));

  vector<string> edge_disjoint = {
    "mipippisissi", "mipippissi",   "mipippissisi", "mipippisssi",
    "mipipppisi",   "mipipppissi",  "mipipppisssi", "mipisipppi",
    "mipisissippi", "mipisisssi",   "mipissippi",   "mipissipppi",
    "mipissisippi", "mipisssippi",  "mipisssipppi", "mipisssisi",
    "mippipisissi", "mippipissi",   "mippipissisi", "mippipisssi",
    "mippisissi",   "mippisissipi", "mippisisssi",  "mippissipi",
    "mippissisi",   "mippissisipi", "mippisssipi",  "mippisssisi",
    "mipppipisi",   "mipppipissi",  "mipppipisssi", "mipppisipi",
    "mipppisissi",  "mipppisisssi", "mipppissipi",  "mipppissisi",
    "mipppisssi",   "mipppisssipi", "mipppisssisi", "misipippissi",
    "misipipppi",   "misipisssi",   "misippipissi", "misippissi",
    "misippisssi",  "misipppipi",   "misipppissi",  "misipppisssi",
    "misissipippi", "misissippi",   "misissippipi", "misissipppi",
    "misisssipi",   "misisssippi",  "misisssipppi", "missipippi",
    "missipippisi", "missipipppi",  "missippipi",   "missippipisi",
    "missippisi",   "missipppipi",  "missipppisi",  "missisipippi",
    "missisippi",   "missisippipi", "missisipppi",  "misssipippi",
    "misssipipppi", "misssipisi",   "misssippipi",  "misssippisi",
    "misssipppi",   "misssipppipi", "misssipppisi", "misssisipi",
    "misssisippi",  "misssisipppi",
  };

  EXPECT_EQ(edge_disjoint, rec.get_simplified_paths(
                               graph::Traversal::all_edge_disjoint_paths));
}

TEST(FilterSize, Ramakrishna) {
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
  bfeattacks::filter_size(rec, 10, 12);

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

TEST(FilterSize, William) {
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
  bfeattacks::filter_size(rec, 6, 8);

  // Verify traversal results

  // This really could be any of the simple paths, but at the moment just
  // returns this one
  vector<string> dfs_paths = {};
  EXPECT_EQ(dfs_paths,
            rec.get_simplified_paths(graph::Traversal::depth_first_search));

  vector<string> simple_paths = { "wiliam", "william" };
  EXPECT_EQ(simple_paths,
            rec.get_simplified_paths(graph::Traversal::all_simple_paths));

  vector<string> edge_disjoint = { "wiliam", "william", "willliam" };
  EXPECT_EQ(edge_disjoint, rec.get_simplified_paths(
                               graph::Traversal::all_edge_disjoint_paths));
}
