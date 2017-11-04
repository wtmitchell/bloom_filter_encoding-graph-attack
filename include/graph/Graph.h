//===-- graph/Graph.h - Graph Declarations -------------------*- C++ -*----===//
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
/// Large portions of this file are copied and modified from
/// <boost/graph/depth_first_search.hpp>
///
//===----------------------------------------------------------------------===//
#ifndef GRAPH_GRAPH_H_INCLUDED
#define GRAPH_GRAPH_H_INCLUDED

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace graph {
struct vertex_info {
  std::string name;
};
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, vertex_info,
    boost::property<boost::edge_index_t, std::size_t> > Graph_t;
typedef boost::graph_traits<Graph_t>::vertex_descriptor Vertex_t;
typedef boost::graph_traits<Graph_t>::edge_descriptor Edge_t;
}

#endif
