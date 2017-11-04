//===-- bfeattacks/GraphFactory.cpp - Graphs from BloomFilters -*- C++ -*--===//
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
//
// This file contains some implementations to construct a graph
//
//===----------------------------------------------------------------------===//

#include "bfeattacks/GraphFactory.h"

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
#include "util/Vector.h"

#include <boost/graph/adjacency_list.hpp>

Graph_t bfeattacks::constructGraph(vector<string> &vertices,
                                   const vector<pair<string, string> > &edges) {
  Graph_t g(vertices.size() + 2);

  // Sort the vertices lexicographically just to be sure since binary_search
  // below
  // assumes sorted
  std::sort(vertices.begin(), vertices.end());

  // Add labels to vertices
  g[0].name = "Source";
  g[1].name = "Sink";
  for (string::size_type i = 0; i < vertices.size(); ++i)
    g[i + 2].name = vertices[i];

  // Add the edges to the graph
  size_t edge_index = 0;
  for (const auto &i : edges) {
    auto tail_idx =
        (i.first == "Source") ? 0 : util::binary_search(vertices, i.first) + 2;
    auto head_idx =
        (i.second == "Sink") ? 1 : util::binary_search(vertices, i.second) + 2;

    boost::add_edge(tail_idx, head_idx, edge_index++, g);
  }

  return g;
}

string bfeattacks::simplify_path(const vector<string> &path,
                                 bool UseStartSentinel, char StartSentinel) {
  string simple = "";

  for (const auto &i : path) {
    if (i == "Source" || i == "Sink")
      continue;
    if (UseStartSentinel && i[0] == StartSentinel)
      continue;
    simple += i[0];
  }

  return simple;
}
