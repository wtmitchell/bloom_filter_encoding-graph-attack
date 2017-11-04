//===-- graph/Traversals.h - Graph Traversals ----------------*- C++ -*----===//
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
/// \brief This file contains various graph traversal algorithms. Assumes the
/// Boost Graph Library.
///
//===----------------------------------------------------------------------===//
#ifndef GRAPH_TRAVERSALS_H_INCLUDED
#define GRAPH_TRAVERSALS_H_INCLUDED

#include <ostream>
#include <string>
#include <vector>

#include "graph/Graph.h"
#include "util/Timer.h"

namespace graph {
enum class Traversal {
  depth_first_search,
  all_simple_paths,
  all_edge_disjoint_paths
};

// Two variations, one taking a timer, the other not
void run_traversal(Graph_t &g, Vertex_t source, Vertex_t sink,
                   std::vector<std::vector<std::string> > &paths,
                   Traversal traversal, util::Timer &timer);
void run_traversal(Graph_t &g, Vertex_t source, Vertex_t sink,
                   std::vector<std::vector<std::string> > &paths,
                   Traversal traversal);

std::ostream &operator<<(std::ostream &out, const Traversal t);
}

#endif
