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

#include <algorithm>
using std::fill;
#include <future>
using std::future;
#include <iterator>
using std::advance;
#include <numeric>
using std::accumulate;
#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include "concurrent/ThreadPool.h"

TEST(ThreadPoolSimple, ParallelAccumulate) {
  typedef long long T;
  const T total = 10000;
  const T blockSize = 10;
  const T numBlocks = (total + blockSize - 1) / blockSize;

  // Fill with 1 repeated total times
  vector<T> numbers(total);
  fill(numbers.begin(), numbers.end(), 1);

  vector<future<T> > futures(numBlocks);
  concurrent::ThreadPoolSimple pool;

  auto blockStart = numbers.begin();
  for (T i = 0; i < numBlocks; ++i) {
    auto blockEnd = blockStart;
    advance(blockEnd, blockSize);
    futures[i] = pool.submit([blockStart, blockEnd] {
      return accumulate(blockStart, blockEnd, T(0));
    });
    blockStart = blockEnd;
  }

  T result = 0;
  for (auto &i : futures) {
    T one = i.get();
    EXPECT_EQ(blockSize, one);
    result += one;
  }

  EXPECT_EQ(total, result);
}
