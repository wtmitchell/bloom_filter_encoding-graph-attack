//===-- bloomfilter/HashSet.h - Bloom filter hash set ------*- C++ -*-===//
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
/// \brief This file defines a hash set for use in bloom filter
///
//===----------------------------------------------------------------------===//
#ifndef BLOOMFILTER_HASHSET_H_INCLUDED
#define BLOOMFILTER_HASHSET_H_INCLUDED

#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "hash/HashFactory.h"
#include "hash/HashFunction.h"
#include "util/ByteVector.h"
#include "util/Types.h"

namespace bloomfilter {
// This iterator just cycles through each hash in its vector
// This could easily have been a std::random_iterator_tag, but the extras of
// that weren't needed at implementation time
class HashSetIteratorSimple
    : public std::iterator<std::input_iterator_tag, std::vector<byte> > {
public:
  friend bool operator==(const HashSetIteratorSimple &lhs,
                         const HashSetIteratorSimple &rhs);
  friend bool operator!=(const HashSetIteratorSimple &lhs,
                         const HashSetIteratorSimple &rhs);

  HashSetIteratorSimple(const std::vector<std::unique_ptr<hash::HashFunction> > &hashes_,
                        const std::string &in_, int index_, unsigned int m_,
                        unsigned int k_)
      : hashes(hashes_), in(in_), index(index_), m(m_), k(k_) {}

  HashSetIteratorSimple(const HashSetIteratorSimple &rhs)
      : hashes(rhs.hashes), in(rhs.in), index(rhs.index), m(rhs.m), k(rhs.k) {}

  HashSetIteratorSimple &operator++();
  HashSetIteratorSimple &operator++(int);
  unsigned int operator*();

  const static std::string name() { return "Simple"; }

private:
  const std::vector<std::unique_ptr<hash::HashFunction> > &hashes;
  const std::string &in;
  int index;
  const unsigned int m;
  const unsigned int k;
};

// This iterator builds hashes according to Kirsch&Mitzenmacher 2006
// This could easily have been a std::random_iterator_tag, but the extras of
// that weren't needed at implementation time
class HashSetIteratorPair
    : public std::iterator<std::input_iterator_tag, std::vector<byte> > {
public:
  friend bool operator==(const HashSetIteratorPair &lhs,
                         const HashSetIteratorPair &rhs);
  friend bool operator!=(const HashSetIteratorPair &lhs,
                         const HashSetIteratorPair &rhs);

  HashSetIteratorPair(const std::vector<std::unique_ptr<hash::HashFunction> > &hashes_,
                      const std::string &in_, int index_, unsigned int m_,
                      unsigned int k_)
      : hashes(hashes_), in(in_), index(index_), m(m_), k(k_),
        h1((hashes[0]->calculate(in)) % m), h2((hashes[1]->calculate(in)) % m) {
    assert(hashes.size() == 2);
    assert(m != 0);
    assert(k != 0);
  }

  HashSetIteratorPair(const HashSetIteratorPair &rhs)
      : hashes(rhs.hashes), in(rhs.in), index(rhs.index), m(rhs.m), k(rhs.k),
        h1(rhs.h1), h2(rhs.h2) {}

  HashSetIteratorPair &operator++();
  HashSetIteratorPair &operator++(int);
  unsigned int operator*();

  const static std::string name() { return "Pair"; }

private:
  const std::vector<std::unique_ptr<hash::HashFunction> > &hashes;
  const std::string &in;
  int index;
  const unsigned int m;
  const unsigned int k;
  const unsigned int h1;
  const unsigned int h2;
};

template <typename Iterator> class HashSetProcessor {
public:
  typedef Iterator iterator;

  HashSetProcessor(const std::vector<std::unique_ptr<hash::HashFunction> > &hashes_,
                   const std::string in_, unsigned int m_, unsigned int k_)
      : hashes(hashes_), in(in_), m(m_), k(k_) {}

  iterator begin() const { return iterator(hashes, in, 0, m, k); }
  iterator end() const { return iterator(hashes, in, -1, m, k); }

private:
  const std::vector<std::unique_ptr<hash::HashFunction> > &hashes;
  const std::string in;
  const unsigned int m;
  const unsigned int k;
};

template <typename Processor> class HashSet {
public:
  typedef Processor processor;

  template <typename A>
  friend std::ostream &operator<<(std::ostream &out, const HashSet<A> &bhs);
  template <typename A>
  friend bool operator==(const HashSet<A> &lhs, const HashSet<A> &rhs);

  explicit HashSet(unsigned int k_ = 0) : k(k_) {}

  ~HashSet() = default;
  HashSet(const HashSet& other) : k(other.k) {
    for (const auto& i : other.functions)
      functions.emplace_back(i->clone());
  }
  HashSet(HashSet&& other) : k(other.k) {
    std::swap(functions, other.functions);
  }
  HashSet& operator=(const HashSet& other) = delete;
  HashSet& operator=(HashSet&& other) {
    k = other.k;
    std::swap(functions, other.functions);
  }

  HashSet &add(hash::Hashes hash);
  HashSet &addHMAC(hash::Hashes hash, std::vector<byte> key);

  const std::vector<std::unique_ptr<hash::HashFunction> > &hashes() const { return functions; }

  std::vector<std::string> names() const;
  processor process(const std::string &in, unsigned int m) const {
    return processor(functions, in, m, k);
  }

  unsigned int count() const { return k; }

private:
  std::vector<std::unique_ptr<hash::HashFunction> > functions;
  unsigned int k;
};

template <typename Processor>
std::ostream &operator<<(std::ostream &out, const HashSet<Processor> &hs) {
  // Could use HashSet::names() here instead of functions directly, but then
  // need to store the temp vector
  out << Processor::iterator::name() << " (k = " << hs.k << ") {";
  for (std::vector<std::unique_ptr<hash::HashFunction> >::const_iterator
           i = hs.functions.begin(),
           e = hs.functions.end();
       i != e; ++i) {
    if (i != hs.functions.begin())
      out << ", ";
    out << (*i)->name();
  }
  out << "}";
  return out;
}

template <typename Processor>
bool operator==(const HashSet<Processor> &lhs, const HashSet<Processor> &rhs) {
  return lhs.functions == rhs.functions;
}

template <typename Processor>
HashSet<Processor> &HashSet<Processor>::add(hash::Hashes h) {
  functions.emplace_back(hash::getHash(h));
  k = std::max(k, static_cast<unsigned>(functions.size()));
  return *this;
}

template <typename Processor>
HashSet<Processor> &HashSet<Processor>::addHMAC(hash::Hashes h, std::vector<byte> key) {
  functions.emplace_back(hash::getHMACHash(h, key));
  k = std::max(k, static_cast<unsigned>(functions.size()));
  return *this;
}

template <typename Processor>
std::vector<std::string> HashSet<Processor>::names() const {
  std::vector<std::string> nameList;

  for (std::vector<std::unique_ptr<hash::HashFunction> >::const_iterator i = functions.begin(),
                                                         e = functions.end();
       i != e; ++i)
    nameList.push_back((*i)->name());

  return nameList;
}

typedef HashSet<HashSetProcessor<HashSetIteratorSimple> > HashSetSimple;
typedef HashSet<HashSetProcessor<HashSetIteratorPair> > HashSetPair;
}

#endif
