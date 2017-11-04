//===-- bfeattacks/ParallelAccumulator.h ---------------------*- C++ -*----===//
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
/// \brief This file contains a parallel accumulator
///
//===----------------------------------------------------------------------===//
#ifndef BFEATTACKS_PARALLELACCUMULATOR_H_INCLUDED
#define BFEATTACKS_PARALLELACCUMULATOR_H_INCLUDED

#include <functional>
#include <future>
#include <iterator>
#include <ostream>
#include <string>
#include <thread>
#include <vector>

#include "bfeattacks/Accumulator.h"
#include "bfeattacks/SingleRecord.h"
#include "bloomfilter/BloomFilter.h"
#include "bloomfilter/HashSet.h"
#include "concurrent/ThreadPool.h"

namespace bfeattacks {
template <typename BFType, typename Container>
bfeattacks::Accumulator ParallelAccumulate(
    const Container input,
    std::function<bfeattacks::SingleRecord<BFType>(void)> BFBuilder,
    std::function<void(bfeattacks::SingleRecord<BFType>&)> BFFilter,
    const std::vector<graph::Traversal> traversals, const std::string alphabet,
    const typename Container::size_type blockSize,
    const unsigned numThreads = std::thread::hardware_concurrency(),
    std::ostream &out = std::cout,
    const typename Container::size_type reportMask = 0xFF);

template <typename BFType, typename Container>
bfeattacks::Accumulator
ThreadWorker(typename Container::const_iterator start,
             typename Container::const_iterator end,
             const std::vector<graph::Traversal> traversals,
             const std::string alphabet,
             std::function<bfeattacks::SingleRecord<BFType>(void)> BFBuilder,
	     std::function<void(bfeattacks::SingleRecord<BFType>&)> BFFilter);
}

namespace bfeattacks {
template <typename BFType, typename Container>
bfeattacks::Accumulator ParallelAccumulate(
    const Container input,
    std::function<bfeattacks::SingleRecord<BFType>(void)> BFBuilder,
    std::function<void(bfeattacks::SingleRecord<BFType>&)> BFFilter,
    const std::vector<graph::Traversal> traversals, const std::string alphabet,
    const typename Container::size_type blockSize, const unsigned numThreads,
    std::ostream &out, const typename Container::size_type reportMask) {
  // ceiling of input.size() / blockSize
  const typename Container::size_type numBlocks =
      (input.size() + blockSize - 1) / blockSize;

  std::vector<std::future<bfeattacks::Accumulator> > futures(numBlocks);
  concurrent::ThreadPoolSimple pool(numThreads);

  // Submit everything
  auto blockStart = input.begin();
  for (typename Container::size_type i = 0; i < numBlocks - 1; ++i) {
    auto blockEnd = blockStart;
    std::advance(blockEnd, blockSize);
    futures[i] = pool.submit([blockStart, blockEnd, traversals, alphabet,
                              BFBuilder, BFFilter]() {
      return ThreadWorker<BFType, Container>(blockStart, blockEnd, traversals,
                                             alphabet, BFBuilder, BFFilter);
    });
    blockStart = blockEnd;
  }
  // Last block submitted separately to avoid undefined behavior triggered by
  // considering an iterator advanced past the end of the container
  futures[numBlocks - 1] =
    pool.submit([blockStart, &input, traversals, alphabet, BFBuilder, BFFilter]() {
        return ThreadWorker<BFType, Container>(
            blockStart, input.end(), traversals, alphabet, BFBuilder, BFFilter);
      });
  out << "Tasks all in queue" << endl;

  // Collect results
  bfeattacks::Accumulator stats(traversals);
  for (typename Container::size_type i = 0; i < numBlocks; ++i) {
    stats += futures[i].get();
    if ((i & reportMask) == 0)
      out << "Completed tasks: " << i + 1 << "/" << numBlocks << endl;
  }

  return stats;
}

template <typename BFType, typename Container>
bfeattacks::Accumulator
ThreadWorker(typename Container::const_iterator start,
             typename Container::const_iterator end,
             const std::vector<graph::Traversal> traversals,
             const std::string alphabet,
             std::function<bfeattacks::SingleRecord<BFType>(void)> BFBuilder,
	     std::function<void(bfeattacks::SingleRecord<BFType>&)> BFFilter) {
  bfeattacks::Accumulator stats(traversals);

  for (typename Container::const_iterator word = start; word != end; ++word) {
    // Construct the record
    auto rec = BFBuilder();

    // Populate the bloom filter
    rec.insert(*word);

    // Construct the graph
    rec.construct_graph(alphabet);

    // Run the traversals
    rec.setup_traversals(traversals);
    for (const auto i : traversals)
      rec.run_traversal(i);
    rec.simplify_paths();

    // Apply filters
    BFFilter(rec);

    // Collect stats
    stats.add(rec);
  }

  return stats;
}
}

#endif
