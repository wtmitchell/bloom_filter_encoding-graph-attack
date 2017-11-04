//===-- bfeattacks/GraphFactory.h - Graphs from BloomFilters -*- C++ -*----===//
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
/// \brief This file creates a graph from a BloomFilter with either letters as
/// vertices or n-grams as vertices
///
//===----------------------------------------------------------------------===//
#ifndef BFEATTACKS_GRAPHFACTORY_H_INCLUDED
#define BFEATTACKS_GRAPHFACTORY_H_INCLUDED

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "bloomfilter/BloomFilter.h"
#include "bloomfilter/InsertionPolicy.h"
#include "graph/Graph.h"

namespace bfeattacks {
template <typename Hashes, unsigned int N, bool UseStartSentinel,
          char StartSentinel, bool UseStopSentinel, char StopSentinel,
          bool TrackEntries>
graph::Graph_t constructGraph(
    const bloomfilter::BloomFilter<
        Hashes,
        bloomfilter::InsertionPolicy<bloomfilter::InsertionPolicyProcessor<
            bloomfilter::InsertionPolicyIteratorNGram<
                N, UseStartSentinel, StartSentinel, UseStopSentinel,
                StopSentinel> > >,
        TrackEntries> &bf,
    const std::string &alphabet);

// vertices is not const since it will be sorted
graph::Graph_t
constructGraph(std::vector<std::string> &vertices,
               const std::vector<std::pair<std::string, std::string> > &edges);

template <int N, bool UseStartSentinel, char StartSentinel,
          bool UseStopSentinel, char StopSentinel>
std::vector<std::pair<std::string, std::string> >
calculateEdges(std::vector<std::string> vertices);

std::string simplify_path(const std::vector<std::string> &path,
                          bool UseStartSentinel = false,
                          char StartSentinel = '^');
}

template <typename Hashes, unsigned int N, bool UseStartSentinel,
          char StartSentinel, bool UseStopSentinel, char StopSentinel,
          bool TrackEntries>
graph::Graph_t bfeattacks::constructGraph(
    const bloomfilter::BloomFilter<
        Hashes,
        bloomfilter::InsertionPolicy<bloomfilter::InsertionPolicyProcessor<
            bloomfilter::InsertionPolicyIteratorNGram<
                N, UseStartSentinel, StartSentinel, UseStopSentinel,
                StopSentinel> > >,
        TrackEntries> &bf,
    const std::string &alphabet) {
  auto vertices = bf.potential_members(alphabet);
  auto edges = calculateEdges<N, UseStartSentinel, StartSentinel,
                              UseStopSentinel, StopSentinel>(vertices);

  return constructGraph(vertices, edges);
}

template <int N, bool UseStartSentinel, char StartSentinel,
          bool UseStopSentinel, char StopSentinel>
std::vector<std::pair<std::string, std::string> >
bfeattacks::calculateEdges(std::vector<std::string> vertices) {
  std::vector<std::pair<std::string, std::string> > edges;

  // Find the vertices connected to the source
  if (UseStartSentinel) {
    // sources are only those that start with a start sentinel
    for (const auto &i : vertices) {
      std::string::size_type j;
      for (j = 0; j < N - 1 && i[j] == StartSentinel; ++j)
        ;

      if (j == N - 1)
        edges.push_back({ "Source", i });
    }
  } else {
    // Everything could be a source
    for (const auto &i : vertices)
      edges.push_back({ "Source", i });
  }

  // Find the vertices connected to the sinks
  if (UseStopSentinel) {
    // sinks are only those ending with a stop sentinel
    for (const auto &i : vertices) {
      std::string::size_type j;
      for (j = i.size() - 1; j > 0 && i[j] == StopSentinel; --j)
        ;

      if (j == 0)
        edges.push_back({ i, "Sink" });
    }
  } else {
    // Everything could be a sink
    for (const auto &i : vertices)
      edges.push_back({ i, "Sink" });
  }

  // Sort the vertices lexicographically just to be sure since equal_range below
  // assumes sorted
  std::sort(vertices.begin(), vertices.end());

  for (const auto &source : vertices) {
    // The lambda here makes equal range work for just the substring of the
    // smaller string
    auto targets = std::equal_range(
        vertices.begin(), vertices.end(),
        source.substr(1),
            [](const std::string & lhs, const std::string & rhs)->bool {
          for (std::string::size_type i = 0,
                                      e = std::min(lhs.size(), rhs.size());
               i != e; ++i) {
            if (lhs[i] != rhs[i])
              return lhs[i] < rhs[i];
          }
          return false;
        });
    for (auto i = targets.first; i != targets.second; ++i)
      edges.push_back(std::make_pair(source, *i));
  }

  return edges;
}

#endif
