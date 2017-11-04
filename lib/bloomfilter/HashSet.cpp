//===-- bloomfilter/HashSet.cpp - Bloom filter hash set --------------===//
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
//===----------------------------------------------------------------------===//
#include "bloomfilter/HashSet.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include "hash/HashFunction.h"
using hash::HashFunction;
#include "util/ByteVector.h"

bool bloomfilter::operator==(const bloomfilter::HashSetIteratorSimple &lhs,
                             const bloomfilter::HashSetIteratorSimple &rhs) {
  return lhs.index == rhs.index;
}

bool bloomfilter::operator!=(const bloomfilter::HashSetIteratorSimple &lhs,
                             const bloomfilter::HashSetIteratorSimple &rhs) {
  return lhs.index != rhs.index;
}

bloomfilter::HashSetIteratorSimple &bloomfilter::HashSetIteratorSimple::
operator++() {
  if (index < 0 || static_cast<unsigned>(index) >= hashes.size() - 1)
    index = -1;
  else
    ++index;
  return *this;
}

bloomfilter::HashSetIteratorSimple &bloomfilter::HashSetIteratorSimple::
operator++(int) {
  if (index < 0 || static_cast<unsigned>(index) >= hashes.size() - 1)
    index = -1;
  else
    ++index;
  return *this;
}

unsigned int bloomfilter::HashSetIteratorSimple::operator*() {
  return hashes[static_cast<unsigned>(index)]->calculate(in) % m;
}

bool bloomfilter::operator==(const bloomfilter::HashSetIteratorPair &lhs,
                             const bloomfilter::HashSetIteratorPair &rhs) {
  return lhs.index == rhs.index;
}

bool bloomfilter::operator!=(const bloomfilter::HashSetIteratorPair &lhs,
                             const bloomfilter::HashSetIteratorPair &rhs) {
  return lhs.index != rhs.index;
}

bloomfilter::HashSetIteratorPair &bloomfilter::HashSetIteratorPair::
operator++() {
  if (index < 0 || static_cast<unsigned>(index) >= k - 1)
    index = -1;
  else
    ++index;
  return *this;
}

bloomfilter::HashSetIteratorPair &bloomfilter::HashSetIteratorPair::
operator++(int) {
  if (index < 0 || static_cast<unsigned>(index) >= k - 1)
    index = -1;
  else
    ++index;
  return *this;
}

unsigned int bloomfilter::HashSetIteratorPair::operator*() {
  return (h1 + h2 * static_cast<unsigned>(index)) % m;
}
