//===-- bfeattacks/Accumulator.h - Accumulator of stats ------*- C++ -*----===//
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
/// \brief This file contains an accumulator used to gather statistics about
/// runs of various bfeattacks
///
//===----------------------------------------------------------------------===//
#ifndef BFEATTACKS_ACCUMULATOR_H_INCLUDED
#define BFEATTACKS_ACCUMULATOR_H_INCLUDED

#include <ostream>
#include <string>
#include <vector>

#include "bfeattacks/SingleRecord.h"
#include "concurrent/Accumulator.h"
#include "graph/Traversals.h"

#include <iostream>

namespace bfeattacks {
class Accumulator {
public:
  friend std::ostream &operator<<(std::ostream &out, const Accumulator &a);

  Accumulator(const std::vector<graph::Traversal> traversals_)
      : traversals(traversals_) {
    traversalStats.resize(traversals.size());
  }

  Accumulator& operator+=(const Accumulator &other);
  // record really should be const, but not at the moment due to how
  // SingleRecord is
  template <typename T> void add(SingleRecord<T> &record);

private:
  const std::vector<graph::Traversal> traversals;

  concurrent::Accumulator<size_t> bf_bits_set;
  concurrent::Accumulator<double> bf_est_elts;

  concurrent::Accumulator<size_t> graph_vertices_all;
  concurrent::Accumulator<size_t> graph_vertices_real;
  concurrent::Accumulator<size_t> graph_vertices_false;
  concurrent::Accumulator<size_t> graph_edges;

  struct traversalStat {
    concurrent::Accumulator<size_t> total_guess_set;
    concurrent::Accumulator<size_t> correct_guess_set;
    concurrent::Accumulator<size_t> incorrect_guess_set;

    std::vector<std::string> missed;
  };
  std::vector<traversalStat> traversalStats;

  size_t trials = 0;
};
std::ostream &operator<<(std::ostream &out, const Accumulator &a);
}

template <typename T>
void bfeattacks::Accumulator::add(bfeattacks::SingleRecord<T> &record) {
  // Bloom Filter stats
  bf_bits_set.add(record.bf.count());
  bf_est_elts.add(record.estimate_elements());

  // Graph stats
  graph_vertices_all.add(record.bf.potential_members(record.alphabet).size());
  graph_vertices_real.add(record.bf.true_members().size());
  graph_vertices_false.add(record.bf.false_members().size());

  graph_edges.add(record.edges.size());

  ++trials;

  // Stats per traversal type
  for (std::vector<graph::Traversal>::size_type i = 0, e = traversals.size();
       i < e; ++i) {
    auto paths = record.get_simplified_paths(traversals[i]);

    traversalStats[i].total_guess_set.add(paths.size());

    if (paths.size() > 10000)
      std::cout << "Word with > 10,000 " << traversals[i] << " paths: "
		<< record.inserted[0] << " with " << paths.size() << std::endl;

    bool found = false;
    for (const auto &p : paths) {
      if (p == record.inserted[0]) {
	traversalStats[i].correct_guess_set.add(paths.size());
	found = true;
	break;
      }
    }
    if (!found) {
      traversalStats[i].incorrect_guess_set.add(paths.size());
      traversalStats[i].missed.push_back(record.inserted[0]);
    }
  }
}

#endif
