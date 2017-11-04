//===-- concurrent/Accumulator.h - A statistical accumulator--*- C++ -*----===//
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
/// \brief This file contains a statistical accumulator similar in principle to
/// Boost.Accumulators, except this one only tracks items that can be combined
/// making it useful for a parallel reduce.
///
//===----------------------------------------------------------------------===//
#ifndef CONCURRENT_ACCUMULATOR_H_INCLUDED
#define CONCURRENT_ACCUMULATOR_H_INCLUDED

#include <algorithm>
#include <map>
#include <type_traits>
#include <utility>

namespace concurrent {
template <typename TrackingType, typename StorageType = unsigned,
          typename Container = std::map<TrackingType, StorageType> >
class Accumulator {
public:
  Accumulator();
  ~Accumulator() = default;
  Accumulator(const Accumulator &) = default;
  Accumulator(Accumulator &&other) = default;
  Accumulator &operator=(const Accumulator &) = default;
  Accumulator &operator=(Accumulator &&other) = default;

  Accumulator<TrackingType, StorageType, Container> &
  operator+=(const Accumulator<TrackingType, StorageType, Container> &other);

  void add(const TrackingType e);
  void combine(const Accumulator<TrackingType, StorageType, Container> &other);
  StorageType count(const TrackingType e) const;
  StorageType count() const;
  const Container &getDensity() const;
  TrackingType getMax() const;
  TrackingType getMin() const;

private:
  StorageType countItems;
  Container density;
};
}

namespace concurrent {
template <typename TrackingType, typename StorageType, typename Container>
Accumulator<TrackingType, StorageType, Container>::Accumulator()
    : countItems(0) {
  static_assert(std::is_integral<StorageType>::value,
                "StorageType must be integral");
  static_assert(std::is_unsigned<StorageType>::value,
                "StorageType must be unsigned");
}

template <typename TrackingType, typename StorageType, typename Container>
Accumulator<TrackingType, StorageType, Container> &
Accumulator<TrackingType, StorageType, Container>::
operator+=(const Accumulator<TrackingType, StorageType, Container> &other) {
  combine(other);
  return *this;
}

template <typename TrackingType, typename StorageType, typename Container>
void
Accumulator<TrackingType, StorageType, Container>::add(const TrackingType e) {
  ++countItems;
  auto i = density.find(e);
  if (i == density.end())
    density.emplace(e, 1);
  else
    ++i->second;
}

template <typename TrackingType, typename StorageType, typename Container>
void Accumulator<TrackingType, StorageType, Container>::combine(
    const Accumulator<TrackingType, StorageType, Container> &other) {
  for (const auto &i : other.density) {
    auto j = density.find(i.first);
    if (j == density.end())
      density.emplace(i);
    else
      j->second += i.second;
  }
  countItems += other.countItems;
}

template <typename TrackingType, typename StorageType, typename Container>
StorageType Accumulator<TrackingType, StorageType, Container>::count(
    const TrackingType e) const {
  auto i = density.find(e);
  if (i == density.end())
    return StorageType(0);
  return i->second;
}

template <typename TrackingType, typename StorageType, typename Container>
StorageType Accumulator<TrackingType, StorageType, Container>::count() const {
  return countItems;
}

template <typename TrackingType, typename StorageType, typename Container>
const Container &
Accumulator<TrackingType, StorageType, Container>::getDensity() const {
  return density;
}

template <typename TrackingType, typename StorageType, typename Container>
TrackingType Accumulator<TrackingType, StorageType, Container>::getMax() const {
  return std::max_element(density.begin(), density.end(),
                          [](std::pair<TrackingType, StorageType> a,
                             std::pair<TrackingType, StorageType> b) {
                            return a.first < b.first;
                          })->first;
}

template <typename TrackingType, typename StorageType, typename Container>
TrackingType Accumulator<TrackingType, StorageType, Container>::getMin() const {
  return std::min_element(density.begin(), density.end(),
                          [](std::pair<TrackingType, StorageType> a,
                             std::pair<TrackingType, StorageType> b) {
                            return a.first < b.first;
                          })->first;
}
}

#endif
