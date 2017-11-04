//===-- concurrent/ThreadPool.h - Various Thread Pools -------*- C++ -*----===//
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
/// \brief This file contains various thread pool strategies.
///
//===----------------------------------------------------------------------===//
#ifndef CONCURRENT_THREADPOOL_H_INCLUDED
#define CONCURRENT_THREADPOOL_H_INCLUDED

#include <atomic>
#include <future>
#include <thread>
#include <vector>

#include "concurrent/FunctionWrapper.h"
#include "concurrent/Guards.h"
#include "concurrent/LockingQueue.h"

namespace concurrent {
class ThreadPoolSimple {
public:
  ThreadPoolSimple(
      const unsigned threadCount = std::thread::hardware_concurrency())
      : done(false), joiner(threads) {
    try {
      for (unsigned i = 0; i < threadCount; ++i)
        threads.push_back(std::thread(&ThreadPoolSimple::workerThread, this));
    }
    catch (...) {
      done = true;
      throw;
    }
  }
  ~ThreadPoolSimple() { done = true; }

  template <typename F>
  std::future<typename std::result_of<F()>::type> submit(F f) {
    typedef typename std::result_of<F()>::type result_type;

    std::packaged_task<result_type()> task(std::move(f));
    std::future<result_type> result(task.get_future());
    queue.push(std::move(task));
    return result;
  }

private:
  std::atomic_bool done;
  LockingQueue<FunctionWrapper> queue;
  std::vector<std::thread> threads;
  GuardThreads<std::vector<std::thread> > joiner;

  void workerThread() {
    while (!done) {
      FunctionWrapper task;
      if (queue.tryPop(task))
        task();
      else
        std::this_thread::yield();
    }
  }
};
}

#endif
