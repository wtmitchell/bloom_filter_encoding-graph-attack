//===-- concurrent/LockingQueue.h - Locking Queue ------------*- C++ -*----===//
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
/// \brief This file contains an adapter around a std::queue that uses mutexes
/// for concurrency
///
//===----------------------------------------------------------------------===//
#ifndef CONCURRENT_LOCKINGQUEUE_H_INCLUDED
#define CONCURRENT_LOCKINGQUEUE_H_INCLUDED

#include <condition_variable>
#include <mutex>
#include <queue>

namespace concurrent {
template <typename T> class LockingQueue {
public:
  LockingQueue() = default;
  ~LockingQueue() = default;
  LockingQueue(const LockingQueue<T> &other) = delete;
  LockingQueue(LockingQueue<T> &&other) = delete;
  void operator=(const LockingQueue &other) = delete;
  void operator=(LockingQueue &&other) = delete;

  void push(T newItem);
  void waitAndPop(T& value);
  bool tryPop(T& value);
  bool empty() const;

private:
  std::queue<T> queue;
  mutable std::mutex lock;
  std::condition_variable cond;
};
}

namespace concurrent {
template <typename T> void LockingQueue<T>::push(T newItem) {
  std::lock_guard<std::mutex> lk(lock);
  queue.push(std::move(newItem));
  cond.notify_one();
}

template <typename T> void LockingQueue<T>::waitAndPop(T &value) {
  std::unique_lock<std::mutex> lk(lock);
  cond.wait(lk, [this] { return !queue.empty(); });
  value = std::move(queue.front());
  queue.pop();
}

template <typename T> bool LockingQueue<T>::tryPop(T &value) {
  std::lock_guard<std::mutex> lk(lock);
  if (queue.empty())
    return false;
  value = std::move(queue.front());
  queue.pop();
  return true;
}

template <typename T> bool LockingQueue<T>::empty() const {
  std::lock_guard<std::mutex> lk(lock);
  return queue.empty();
}
}

#endif
