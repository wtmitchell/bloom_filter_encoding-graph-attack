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
#include <string>
using std::string;
#include <utility>
using std::pair;
#include <vector>
using std::vector;

#include "graph/Graph.h"
using graph::Graph_t;
#include "graph/Traversals.h"

TEST(Traversal, P4C4Intersection) {
  // Graph is a P_4 whose middle two vertices are opposite vertices of a C_4
  // The endpoints of the P_4 are the source/sink
  // All edges "flow" from source to sink except for 5-2 to create
  // a edge disjoint paths

  Graph_t g(6);

  // Add labels to vertices
  for (unsigned char i = 0; i < 6; ++i)
    g[i].name = string({ static_cast<char>('a' + i) });

  array<array<unsigned, 2>, 8> edges{
    { { { 0, 2 } }, { { 2, 3 } }, { { 2, 4 } }, { { 2, 5 } },
      { { 3, 4 } }, { { 4, 1 } }, { { 5, 2 } }, { { 5, 4 } } }
  };

  size_t edge_index = 0;
  for (const auto &i : edges)
    boost::add_edge(i[0], i[1], edge_index++, g);

  vector<vector<string> > dfs_paths;
  graph::run_traversal(g, 0, 1, dfs_paths,
                       graph::Traversal::depth_first_search);

  // Really this could be any of the simple paths, but as implemented, this is
  // the
  // one that should be found (and only one)
  vector<vector<string> > expected_dfs_paths{
    { { { "a", "c", "d", "e", "b" } } }
  };
  EXPECT_EQ(expected_dfs_paths, dfs_paths);

  // All simple paths
  vector<vector<string> > all_simple_paths;
  graph::run_traversal(g, 0, 1, all_simple_paths,
                       graph::Traversal::all_simple_paths);

  vector<vector<string> > expected_simple_paths{
    { { { "a", "c", "d", "e", "b" } }, { { "a", "c", "e", "b" } },
      { { "a", "c", "f", "e", "b" } } }
  };
  EXPECT_EQ(expected_simple_paths, all_simple_paths);

  // All edge disjoint paths
  vector<vector<string> > all_edge_disjoint_paths;
  graph::run_traversal(g, 0, 1, all_edge_disjoint_paths,
                       graph::Traversal::all_edge_disjoint_paths);

  vector<vector<string> > expected_edge_disjoint_paths{
    { { { "a", "c", "d", "e", "b" } },
      { { "a", "c", "e", "b" } },
      { { "a", "c", "f", "c", "d", "e", "b" } },
      { { "a", "c", "f", "c", "e", "b" } },
      { { "a", "c", "f", "e", "b" } } }
  };
  EXPECT_EQ(expected_edge_disjoint_paths, all_edge_disjoint_paths);
}
