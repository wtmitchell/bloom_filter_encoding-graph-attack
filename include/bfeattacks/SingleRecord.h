//===-- bfeattacks/SingleRecord.h - A single BFE record ------*- C++ -*----===//
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
/// \brief This file contains functions and structs to facilitate breaking a
/// single record.
///
//===----------------------------------------------------------------------===//
#ifndef BFEATTACKS_SINGLERECORD_H_INCLUDED
#define BFEATTACKS_SINGLERECORD_H_INCLUDED

#include <cmath>
#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "bfeattacks/GraphFactory.h"
#include "bloomfilter/BloomFilter.h"
#include "graph/Graph.h"
#include "graph/Traversals.h"

#include <boost/graph/graphml.hpp>

namespace bfeattacks {
template <typename BloomFilter> class SingleRecord {
public:
  template <typename A>
  friend std::ostream &operator<<(std::ostream &out, const SingleRecord<A> &r);

  SingleRecord(BloomFilter bf_) : bf(bf_), source(0), sink(1) {}

  void setup_traversals(const std::vector<graph::Traversal> &t);
  void run_traversal(const graph::Traversal t);
  void simplify_paths();
  const std::vector<std::string> &
  get_simplified_paths(const graph::Traversal t);
  void construct_graph(const std::string &alphabet_);
  void write_graphml(std::ostream &out);
  void insert(const std::string &in);
  double estimate_elements() const;

  BloomFilter bf;
  const graph::Vertex_t source;
  const graph::Vertex_t sink;
  graph::Graph_t g;
  std::string alphabet;
  std::vector<std::string> inserted;
  std::vector<std::string> edges;
  std::vector<std::vector<std::vector<std::string> > > paths;
  std::vector<std::vector<std::string> > simplified_paths;
  std::map<graph::Traversal, unsigned> traversals;
};

template <typename BloomFilter>
std::ostream &operator<<(std::ostream &out, const SingleRecord<BloomFilter> &r) {
  out << r.bf << "\n";

  return out;
}
}

template <typename T>
void bfeattacks::SingleRecord<T>::setup_traversals(
    const std::vector<graph::Traversal> &t) {
  paths.clear();
  traversals.clear();

  unsigned count = 0;
  for (const auto i : t)
    traversals[i] = count++;

  paths.resize(traversals.size());
}

template <typename T>
void bfeattacks::SingleRecord<T>::run_traversal(const graph::Traversal t) {
  unsigned index = traversals[t];

  graph::run_traversal(g, source, sink, paths[index], t);
}

template <typename T>
const std::vector<std::string> &
bfeattacks::SingleRecord<T>::get_simplified_paths(const graph::Traversal t) {
  return simplified_paths[traversals[t]];
}

template <typename T> void bfeattacks::SingleRecord<T>::simplify_paths() {
  simplified_paths.clear();
  simplified_paths.resize(paths.size());

  for (const auto &t : traversals) {
    unsigned index(t.second);
    for (const auto &i : paths[index])
      simplified_paths[index].push_back(simplify_path(i, true, '^'));
  }
}

template <typename T>
void
bfeattacks::SingleRecord<T>::construct_graph(const std::string &alphabet_) {
  alphabet = alphabet_;
  edges = bf.potential_members(alphabet);
  g = bfeattacks::constructGraph(bf, alphabet);
}

template <typename T>
void bfeattacks::SingleRecord<T>::write_graphml(std::ostream &out) {
  boost::dynamic_properties dp;
  dp.property("Label", get(&graph::vertex_info::name, g));
  dp.property("node_id", get(boost::vertex_index, g));
  boost::write_graphml(out, g, dp);
}

template <typename T>
void bfeattacks::SingleRecord<T>::insert(const std::string &in) {
  bf.insert(in);
  inserted.push_back(in);
}

template <typename T> double bfeattacks::SingleRecord<T>::estimate_elements() const {
  return -static_cast<double>(bf.length()) *
         std::log(1 - (static_cast<double>(bf.count()) /
                       static_cast<double>(bf.length()))) /
         static_cast<double>(bf.hash_count());
}

#endif
