//===-- bloomfilter/BloomHashSet.h - Bloom filter hash set ------*- C++ -*-===//
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
/// \brief This file defines a bloom filter
///
//===----------------------------------------------------------------------===//
#ifndef BLOOMFILTER_BLOOMFILTER_H_INCLUDED
#define BLOOMFILTER_BLOOMFILTER_H_INCLUDED

#include <ostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "HashSet.h"
#include "InsertionPolicy.h"

namespace bloomfilter {
/// Basic templated Bloom filter. Configurable based on the hashes and how
/// things are inserted. It also tracks what was inserted in the filter
template <typename Hashes, typename InsertionPolicy, bool TrackEntries = false>
class BloomFilter {
public:
  template <typename A, typename B, bool C>
  friend std::ostream &operator<<(std::ostream &out,
                                  const BloomFilter<A, B, C> &bf);
  template <typename A, typename B, bool C>
  friend std::pair<boost::dynamic_bitset<>::size_type,
                   boost::dynamic_bitset<>::size_type>
  dice_coefficient(BloomFilter<A, B, C> a, BloomFilter<A, B, C> b);

  BloomFilter(unsigned int m_, Hashes &hashes_)
      : hashes(hashes_), contents(m_), m(m_), policy(), real_inserted(),
        real_members(), all_alphabet(""), all_members(),
        all_members_valid(false), fake_members(), fake_members_valid(false) {}

  /// Inserts string in into Bloom Filter according to the insertion policy
  void insert(const std::string &in);

  /// Checks whether in is contained in the Bloom Filter using the insertion
  /// policy
  bool contains(const std::string &in) const;

  /// Checks whether blooms filter contains in and only in
  bool contains_exactly(const std::string &in) const;

  /// Returns a vector of potential members using the specified alphabet
  const std::vector<std::string> &
  potential_members(const std::string &alphabet) const;

  /// Returns just the false positive members. Requires potential_members to
  /// be
  /// called first
  const std::vector<std::string> &false_members() const;

  /// Returns a set of what was actually inserted
  const std::set<std::string> &true_members() const { return real_members; }

  /// Returns a vector of the strings inserted
  const std::vector<std::string> &actual_inserted() const {
    return real_inserted;
  }

  /// Direct access to the underlying bitset
  const boost::dynamic_bitset<> &raw() const { return contents; }

  /// The number of bits that are set
  boost::dynamic_bitset<>::size_type count() const { return contents.count(); }

  /// Returns the length
  unsigned int length() const { return m; }

  /// Returns the number of hashes
  unsigned int hash_count() const { return hashes.count(); }

private:
  Hashes hashes;
  boost::dynamic_bitset<> contents;
  unsigned int m;
  InsertionPolicy policy;
  std::vector<std::string> real_inserted;
  std::set<std::string> real_members;
  mutable std::string all_alphabet;
  mutable std::vector<std::string> all_members;
  mutable bool all_members_valid;
  mutable std::vector<std::string> fake_members;
  mutable bool fake_members_valid;
};

template <typename Hashes, typename InsertionPolicy, bool TrackEntries>
std::ostream &operator<<(
    std::ostream &out,
    const BloomFilter<Hashes, InsertionPolicy, TrackEntries> &bf) {
  out << "BloomFilter:\n"
      << "Size (m) = " << bf.contents.size() << "\n"
      << "Hashes: " << bf.hashes << "\n"
      << "Insertion policy: " << bf.policy << "\n"
      << "Contents: ";
  // print out contents this way instead of using toString or << of dynamic
  // bitset to get output in lsb to msb order.
  for (unsigned long i = 0, e = bf.contents.size(); i < e; ++i)
    out << bf.contents[i];
  out << "\n";

  if (TrackEntries) {
    out << "Actually Inserted(" << bf.real_inserted.size() << "): {";
    for (std::vector<std::string>::const_iterator i = bf.real_inserted.begin(),
                                                  e = bf.real_inserted.end();
         i != e;) {
      out << *i;

      if (++i != e)
        out << ", ";
    }
    out << "}\n"
        << "Actual Members(" << bf.real_members.size() << "): {";
    for (std::set<std::string>::const_iterator i = bf.real_members.begin(),
                                               e = bf.real_members.end();
         i != e;) {
      out << *i;

      if (++i != e)
        out << ", ";
    }
    out << "}\n";

    if (bf.all_members_valid) {
      out << "All(" << bf.all_members.size() << "): {";
      for (std::vector<std::string>::const_iterator i = bf.all_members.begin(),
                                                    e = bf.all_members.end();
           i != e;) {
        out << *i;

        if (++i != e)
          out << ", ";
      }
      out << "}\n";

      if (!bf.fake_members_valid)
        bf.false_members();

      out << "False positives(" << bf.fake_members.size() << "): {";
      for (std::vector<std::string>::const_iterator i = bf.fake_members.begin(),
                                                    e = bf.fake_members.end();
           i != e;) {
        out << *i;

        if (++i != e)
          out << ", ";
      }
      out << "}";
    } else {
      out << "All: ? No alphabet specified\n"
          << "False positives: ?";
    }
  } else {
    out << "Actually Inserted: ? Not tracked\n"
        << "Actual Members: ? Not tracked\n"
        << "All: ? No alphabet specified\n"
        << "False positives: ?";
  }

  return out;
}

template <typename Hashes, typename InsertionPolicy, bool TrackEntries>
void BloomFilter<Hashes, InsertionPolicy, TrackEntries>::insert(
    const std::string &in) {
  all_members_valid = false;
  fake_members_valid = false;
  if (TrackEntries)
    real_inserted.push_back(in);

  typename InsertionPolicy::processor ip = policy.process(in);

  for (typename InsertionPolicy::processor::iterator i = ip.begin(),
                                                     e = ip.end();
       i != e; ++i) {
    typename Hashes::processor hp = hashes.process(*i, m);

    if (TrackEntries)
      real_members.insert(*i);

    for (typename Hashes::processor::iterator j = hp.begin(), f = hp.end();
         j != f; ++j) {
      contents.set(*j);
    }
  }
}

template <typename Hashes, typename InsertionPolicy, bool TrackEntries>
bool BloomFilter<Hashes, InsertionPolicy, TrackEntries>::contains(
    const std::string &in) const {
  typename InsertionPolicy::processor ip = policy.process(in);

  for (typename InsertionPolicy::processor::iterator i = ip.begin(),
                                                     e = ip.end();
       i != e; ++i) {
    typename Hashes::processor hp = hashes.process(*i, m);

    for (typename Hashes::processor::iterator j = hp.begin(), f = hp.end();
         j != f; ++j) {
      if (!contents.test(*j))
        return false;
    }
  }

  return true;
}

template <typename Hashes, typename InsertionPolicy, bool TrackEntries>
bool BloomFilter<Hashes, InsertionPolicy, TrackEntries>::contains_exactly(
    const std::string &in) const {
  typename InsertionPolicy::processor ip = policy.process(in);
  boost::dynamic_bitset<> test_contents(contents.size());

  for (typename InsertionPolicy::processor::iterator i = ip.begin(),
                                                     e = ip.end();
       i != e; ++i) {
    typename Hashes::processor hp = hashes.process(*i, m);

    for (typename Hashes::processor::iterator j = hp.begin(), f = hp.end();
         j != f; ++j) {
      test_contents.set(*j);
    }
  }

  return test_contents == contents;
}

template <typename Hashes, typename InsertionPolicy, bool TrackEntries>
const std::vector<std::string> &
BloomFilter<Hashes, InsertionPolicy, TrackEntries>::potential_members(
    const std::string &alphabet) const {
  if (all_members_valid && all_alphabet == alphabet)
    return all_members;

  all_members.clear();
  all_alphabet = alphabet;
  fake_members_valid = false;

  typedef typename InsertionPolicy::processor processor;
  typedef typename InsertionPolicy::processor::all_iterator iterator;

  for (iterator i = processor::all_begin(alphabet),
                e = processor::all_end(alphabet);
       i != e; ++i) {
    typename Hashes::processor hp = hashes.process(*i, m);

    bool contained = true;
    for (typename Hashes::processor::iterator j = hp.begin(), f = hp.end();
         j != f; ++j) {
      if (!contents.test(*j)) {
        contained = false;
        break;
      }
    }

    if (contained)
      all_members.push_back(*i);
  }

  all_members_valid = true;

  return all_members;
}

template <typename Hashes, typename InsertionPolicy, bool TrackEntries>
const std::vector<std::string> &
BloomFilter<Hashes, InsertionPolicy, TrackEntries>::false_members() const {
  // If all members is not valid, we can't calculate false positives
  assert(all_members_valid);

  if (fake_members_valid)
    return fake_members;

  fake_members.clear();

  for (std::vector<std::string>::const_iterator i = all_members.begin(),
                                                e = all_members.end();
       i != e; ++i) {
    if (real_members.find(*i) == real_members.end())
      fake_members.push_back(*i);
  }

  fake_members_valid = true;

  return fake_members;
}

template <typename A, typename B, bool C>
std::pair<boost::dynamic_bitset<>::size_type,
          boost::dynamic_bitset<>::size_type>
dice_coefficient(BloomFilter<A, B, C> a, BloomFilter<A, B, C> b) {
  // Can only reasonably compare two bitsets of the same size
  assert(a.contents.size() == b.contents.size());

  // Calculate as:
  // 2 * | a \intersect b | / (|a| + |b|)
  // where |*| is number of bits set

  boost::dynamic_bitset<>::size_type intersection = 0;

  for (boost::dynamic_bitset<>::size_type i = 0, e = a.contents.size(); i != e;
       ++i) {
    if (a.contents[i] && b.contents[i])
      ++intersection;
  }

  boost::dynamic_bitset<>::size_type numerator = 2 * intersection;
  boost::dynamic_bitset<>::size_type denominator =
      a.contents.count() + b.contents.count();

  return std::make_pair(numerator, denominator);
}

typedef BloomFilter<HashSetPair, InsertionBigramWithSentinel, true>
BloomFilterStandard;
}

#endif
