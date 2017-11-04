//===-- bfeattacks/Accumulator.cpp - Stat accumulator ----------*- C++ -*--===//
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
//
//===----------------------------------------------------------------------===//

#include "bfeattacks/Accumulator.h"

#include <cassert>
#include <ostream>
using std::endl;
#include <string>
using std::string;

#include "concurrent/Accumulator.h"
#include "stats/Confidence.h"
using stats::confidence_bound;

namespace bfeattacks {
Accumulator& Accumulator::operator+=(const Accumulator &other) {
  // Can only combine when using same traversals
  assert(traversals == other.traversals);

  bf_bits_set += other.bf_bits_set;
  bf_est_elts += other.bf_est_elts;

  graph_vertices_all += other.graph_vertices_all;
  graph_vertices_real += other.graph_vertices_real;
  graph_vertices_false += other.graph_vertices_false;

  graph_edges += other.graph_edges;

  trials += other.trials;

  for (std::vector<graph::Traversal>::size_type i = 0, e = traversals.size(); i < e; ++i) {
    traversalStats[i].total_guess_set += other.traversalStats[i].total_guess_set;
    traversalStats[i].correct_guess_set += other.traversalStats[i].correct_guess_set;
    traversalStats[i].incorrect_guess_set += other.traversalStats[i].incorrect_guess_set;

    for (const auto &w : other.traversalStats[i].missed)
      traversalStats[i].missed.push_back(w);
  }

  return *this;
}

namespace {
template <typename T>
void printAccumulator(std::ostream &out, const concurrent::Accumulator<T> acc, const std::string name) {
  out << name << ":\n"
      << "Range [" << acc.getMin() << ", " << acc.getMax() << "]\n"
    // mean w/ conf bound
    // std dev
      << "density:\n";
  const auto density = acc.getDensity();
  for (const auto &i : density)
    out << i.first << " ";
  out << "\n";
  for (const auto &i : density)
    out << i.second << " ";
  out << "\n";
}
}

std::ostream &operator<<(std::ostream &out, const bfeattacks::Accumulator &a) {
  double conf_bound = 0.05;
  out << "==================================\n"
      << "Stats: confidence bounds are " << conf_bound << "\n"
      << "Total trials: " << a.trials << "\n"
      << "==================================\n"
      << "Bloom filter:\n"
      << "----------------------------------\n";
  printAccumulator(out, a.bf_bits_set, "bf_bits_set");
  printAccumulator(out, a.bf_est_elts, "bf_est_elts");
  out << "----------------------------------\n"
      << "Graph:\n"
      << "----------------------------------\n";
  printAccumulator(out, a.graph_vertices_all, "graph_vertices_all");
  printAccumulator(out, a.graph_vertices_real, "graph_vertices_real");
  printAccumulator(out, a.graph_vertices_false, "graph_vertices_false");
  printAccumulator(out, a.graph_edges, "graph_edges");

  for (std::vector<graph::Traversal>::size_type i = 0, e = a.traversals.size();
       i < e; ++i) {
    out << "----------------------------------\n" << a.traversals[i] << ":\n"
        << "----------------------------------\n";
    printAccumulator(out, a.traversalStats[i].total_guess_set, "total guess set");
    printAccumulator(out, a.traversalStats[i].correct_guess_set, "correct guess set");
    printAccumulator(out, a.traversalStats[i].incorrect_guess_set, "incorrect guess set");

    out << "Missed by " << a.traversals[i] << "\n";
    for (const auto &w : a.traversalStats[i].missed)
      out << w << " ";
    out << "\n";
  }
  return out;
}
}
