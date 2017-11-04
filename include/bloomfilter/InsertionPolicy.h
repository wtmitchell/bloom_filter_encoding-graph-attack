//===-- bloomfilter/HashSet.h - Bloom filter insertion policy ---*- C++ -*-===//
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
/// \brief This file defines insertion policies for a bloom filter
///
//===----------------------------------------------------------------------===//
#ifndef BLOOMFILTER_INSERTIONPOLICY_H_INCLUDED
#define BLOOMFILTER_INSERTIONPOLICY_H_INCLUDED

#include <ostream>
#include <sstream>
#include <string>

#include "util/Bits.h"

namespace bloomfilter {
/// This iterator always just returns T& in.
template <typename T = typename std::string>
class InsertionPolicyIteratorOneShot
    : public std::iterator<std::input_iterator_tag, T> {
public:
  typedef void all_iterator;
  template <typename A>
  friend bool operator==(const InsertionPolicyIteratorOneShot<A> &lhs,
                         const InsertionPolicyIteratorOneShot<A> &rhs);
  template <typename A>
  friend bool operator!=(const InsertionPolicyIteratorOneShot<A> &lhs,
                         const InsertionPolicyIteratorOneShot<A> &rhs);

  InsertionPolicyIteratorOneShot(const T &in_, int index_)
      : in(in_), index(index_) {}

  InsertionPolicyIteratorOneShot(const InsertionPolicyIteratorOneShot<T> &rhs)
      : in(rhs.in), index(rhs.index) {}

  InsertionPolicyIteratorOneShot &operator++();
  InsertionPolicyIteratorOneShot &operator++(int);
  const T operator*();

  const static std::string name() { return "OneShot"; }

private:
  const T &in;
  int index;
};

/// This iterator iterates through all possible n-grams
template <unsigned int N = 2, bool UseStartSentinel = true,
          char StartSentinel = '^', bool UseStopSentinel = true,
          char StopSentinel = '$', typename T = std::string>
class InsertionPolicyIteratorNGramAll
    : public std::iterator<std::input_iterator_tag, T> {
public:
  template <unsigned int A, bool B, char C, bool D, char E, typename F>
  friend bool
  operator==(const InsertionPolicyIteratorNGramAll<A, B, C, D, E, F> &lhs,
             const InsertionPolicyIteratorNGramAll<A, B, C, D, E, F> &rhs);
  template <unsigned int A, bool B, char C, bool D, char E, typename F>
  friend bool
  operator!=(const InsertionPolicyIteratorNGramAll<A, B, C, D, E, F> &lhs,
             const InsertionPolicyIteratorNGramAll<A, B, C, D, E, F> &rhs);

  InsertionPolicyIteratorNGramAll(const std::string &alphabet_, long index_)
      : alphabet(alphabet_), permutation(),
        permutation_limit(alphabet_.size() +
                          ((UseStartSentinel || UseStopSentinel) ? 1 : 0)),
        done(index_ < 0) {
    // We only allow starting or stopping at the end
    assert(index_ == 0 || index_ == -1);

    permutation.reserve(N);
    if (UseStartSentinel) {
      // Start at 0,0,...,0,1
      for (std::vector<unsigned>::size_type i = 1; i < N; ++i)
        permutation.push_back(0);
      permutation.push_back(1);
    } else if (UseStopSentinel) {
      // Start at 1,0,0,...,0
      permutation.push_back(1);
      for (std::vector<unsigned>::size_type i = 1; i < N; ++i)
        permutation.push_back(0);
    } else {
      // Start at 0,0,...,0
      for (std::vector<unsigned>::size_type i = 0; i < N; ++i)
        permutation.push_back(0);
    }
  }

  InsertionPolicyIteratorNGramAll(const InsertionPolicyIteratorNGramAll<
      N, UseStartSentinel, StartSentinel, UseStopSentinel, StopSentinel, T> &
                                      rhs)
      : alphabet(rhs.alphabet), permutation(rhs.permutation),
        permutation_limit(rhs.permutation_limit), done(rhs.done) {}

  InsertionPolicyIteratorNGramAll &operator++();
  InsertionPolicyIteratorNGramAll &operator++(int);
  const T operator*();

private:
  void increment_permutation();
  bool permutation_contains_holes();

  const std::string alphabet;
  std::vector<unsigned> permutation;
  const std::vector<unsigned>::size_type permutation_limit;
  bool done;
};

/// Iterator for going through a std::string by n-grams with optional start/stop
/// sentinels
template <unsigned int N = 2, bool UseStartSentinel = true,
          char StartSentinel = '^', bool UseStopSentinel = true,
          char StopSentinel = '$', typename T = std::string>
class InsertionPolicyIteratorNGram
    : public std::iterator<std::input_iterator_tag, T> {
public:
  typedef InsertionPolicyIteratorNGramAll<N, UseStartSentinel, StartSentinel,
                                          UseStopSentinel, StopSentinel,
                                          T> all_iterator;

  template <unsigned int A, bool B, char C, bool D, char E, typename F>
  friend bool
  operator==(const InsertionPolicyIteratorNGram<A, B, C, D, E, F> &lhs,
             const InsertionPolicyIteratorNGram<A, B, C, D, E, F> &rhs);
  template <unsigned int A, bool B, char C, bool D, char E, typename F>
  friend bool
  operator!=(const InsertionPolicyIteratorNGram<A, B, C, D, E, F> &lhs,
             const InsertionPolicyIteratorNGram<A, B, C, D, E, F> &rhs);

  InsertionPolicyIteratorNGram(const T &in_, int index_)
      : in(in_), index(index_) {}

  InsertionPolicyIteratorNGram(
      const InsertionPolicyIteratorNGram<N, UseStartSentinel, StartSentinel,
                                         UseStopSentinel, StopSentinel, T> &rhs)
      : in(rhs.in), index(rhs.index) {}

  InsertionPolicyIteratorNGram &operator++();
  InsertionPolicyIteratorNGram &operator++(int);
  const T operator*();

  const static std::string name() {
    std::stringstream ss;

    ss << N << "-gram";
    if (UseStartSentinel)
      ss << " with start sentinel '" << StartSentinel << "'";
    else
      ss << " without start sentinel";
    ss << " and ";
    if (UseStopSentinel)
      ss << "with stop sentinel '" << StopSentinel << "'";
    else
      ss << "without stop sentinel";

    return ss.str();
  }

private:
  const T &in;
  int index;
};

/// Generic class to provide begin()/end() to an InsertionPolicy iterator
template <typename Iterator, typename T = std::string>
class InsertionPolicyProcessor {
public:
  typedef Iterator iterator;
  typedef typename Iterator::all_iterator all_iterator;

  InsertionPolicyProcessor(const T in_) : in(in_) {}

  iterator begin() const { return iterator(in, 0); }
  iterator end() const { return iterator(in, -1); }

  static all_iterator all_begin(const std::string &alphabet) {
    return all_iterator(alphabet, 0);
  }
  static all_iterator all_end(const std::string &alphabet) {
    return all_iterator(alphabet, -1);
  }

private:
  const T in;
};

/// Generic class to provide iterators over an input
template <typename Processor, typename T = std::string> class InsertionPolicy {
public:
  typedef Processor processor;

  processor process(const T &in) const { return processor(in); }
};

template <typename A, typename B>
std::ostream &operator<<(std::ostream &out,
                         const InsertionPolicy<A, B> & /*ip*/) {
  out << InsertionPolicy<A, B>::processor::iterator::name();
  return out;
}

template <typename T>
bool operator==(const bloomfilter::InsertionPolicyIteratorOneShot<T> &lhs,
                const bloomfilter::InsertionPolicyIteratorOneShot<T> &rhs) {
  return lhs.index == rhs.index && lhs.in == rhs.in;
}

template <typename T>
bool operator!=(const bloomfilter::InsertionPolicyIteratorOneShot<T> &lhs,
                const bloomfilter::InsertionPolicyIteratorOneShot<T> &rhs) {
  return lhs.index != rhs.index || lhs.in != rhs.in;
}

template <typename T>
InsertionPolicyIteratorOneShot<T> &InsertionPolicyIteratorOneShot<T>::
operator++() {
  index = -1;
  return *this;
}

template <typename T>
InsertionPolicyIteratorOneShot<T> &InsertionPolicyIteratorOneShot<T>::
operator++(int) {
  index = -1;
  return *this;
}

template <typename T> const T InsertionPolicyIteratorOneShot<T>::operator*() {
  return in;
}

template <unsigned int A, bool B, char C, bool D, char E, typename F>
bool operator==(const InsertionPolicyIteratorNGram<A, B, C, D, E, F> &lhs,
                const InsertionPolicyIteratorNGram<A, B, C, D, E, F> &rhs) {
  return lhs.index == rhs.index;
}
template <unsigned int A, bool B, char C, bool D, char E, typename F>
bool operator!=(const InsertionPolicyIteratorNGram<A, B, C, D, E, F> &lhs,
                const InsertionPolicyIteratorNGram<A, B, C, D, E, F> &rhs) {
  return lhs.index != rhs.index;
}

template <unsigned int N, bool UseStartSentinel, char StartSentinel,
          bool UseStopSentinel, char StopSentinel, typename T>
InsertionPolicyIteratorNGram<N, UseStartSentinel, StartSentinel,
                             UseStopSentinel, StopSentinel, T> &
InsertionPolicyIteratorNGram<N, UseStartSentinel, StartSentinel,
                             UseStopSentinel, StopSentinel, T>::
operator++() {
  // If already at end, nothing to do
  if (index < 0)
    return *this;

  ++index;
  if (UseStartSentinel && UseStopSentinel) {
    if (static_cast<unsigned>(index) >= in.size() + N - 1)
      index = -1;
  } else if (UseStartSentinel || UseStopSentinel) {
    if (static_cast<unsigned>(index) >= in.size())
      index = -1;
  } else {
    if (static_cast<unsigned>(index) >= in.size() - N + 1)
      index = -1;
  }
  return *this;
}
template <unsigned int N, bool UseStartSentinel, char StartSentinel,
          bool UseStopSentinel, char StopSentinel, typename T>
InsertionPolicyIteratorNGram<N, UseStartSentinel, StartSentinel,
                             UseStopSentinel, StopSentinel, T> &
InsertionPolicyIteratorNGram<N, UseStartSentinel, StartSentinel,
                             UseStopSentinel, StopSentinel, T>::
operator++(int) {
  // If already at end, nothing to do
  if (index < 0)
    return *this;

  ++index;
  if (UseStartSentinel && UseStopSentinel) {
    if (static_cast<unsigned>(index) >= in.size() + N - 1)
      index = -1;
  } else if (UseStartSentinel || UseStopSentinel) {
    if (static_cast<unsigned>(index) >= in.size())
      index = -1;
  } else {
    if (static_cast<unsigned>(index) >= in.size() - N + 1)
      index = -1;
  }
  return *this;
}
template <unsigned int N, bool UseStartSentinel, char StartSentinel,
          bool UseStopSentinel, char StopSentinel, typename T>
const T InsertionPolicyIteratorNGram<N, UseStartSentinel, StartSentinel,
                                     UseStopSentinel, StopSentinel, T>::
operator*() {
  T ret;

  // Ensure there is something to do
  if (index < 0)
    return ret;

  if (UseStartSentinel) {
    typename T::size_type stringStart;
    // Add any start sentinel
    if (static_cast<unsigned>(index) < N) {
      for (unsigned int i = 1; i < N - static_cast<unsigned>(index); ++i)
        ret += StartSentinel;
      stringStart = 0;
    } else {
      stringStart = static_cast<unsigned>(index) - N + 1;
    }

    // Add piece from the actual string
    for (typename T::size_type i = stringStart; ret.size() < N && i < in.size();
         ++i)
      ret += in[i];

    if (UseStopSentinel) {
      // Add any stop sentinel
      while (ret.size() < N)
        ret += StopSentinel;
    }
  } else {
    // Add piece from the actual string
    for (typename T::size_type i = static_cast<unsigned>(index);
         ret.size() < N && i < in.size(); ++i)
      ret += in[i];

    if (UseStopSentinel) {
      // Add any stop sentinel
      while (ret.size() < N)
        ret += StopSentinel;
    }
  }
  return ret;
}

template <unsigned int N, bool UseStartSentinel, char StartSentinel,
          bool UseStopSentinel, char StopSentinel, typename T>
InsertionPolicyIteratorNGramAll<N, UseStartSentinel, StartSentinel,
                                UseStopSentinel, StopSentinel, T> &
InsertionPolicyIteratorNGramAll<N, UseStartSentinel, StartSentinel,
                                UseStopSentinel, StopSentinel, T>::
operator++() {
  // At end, don't modify it
  if (done)
    return *this;

  increment_permutation();

  if (UseStartSentinel || UseStopSentinel)
    while (!done && permutation_contains_holes())
      increment_permutation();

  return *this;
}

template <unsigned int N, bool UseStartSentinel, char StartSentinel,
          bool UseStopSentinel, char StopSentinel, typename T>
const T InsertionPolicyIteratorNGramAll<N, UseStartSentinel, StartSentinel,
                                        UseStopSentinel, StopSentinel, T>::
operator*() {
  if (done)
    return "";

  T ret;

  if (UseStartSentinel || UseStopSentinel) {
    bool encountered_non_sentinel = false;
    for (std::vector<unsigned>::const_iterator i = permutation.begin(),
                                               e = permutation.end();
         i != e; ++i) {
      encountered_non_sentinel |= *i > 0;
      if (*i == 0)
        ret += (!encountered_non_sentinel) ? StartSentinel : StopSentinel;
      else
        ret += alphabet[*i - 1];
    }
  } else {
    for (std::vector<unsigned>::const_iterator i = permutation.begin(),
                                               e = permutation.end();
         i != e; ++i) {
      ret += alphabet[*i];
    }
  }
  return ret;
}

template <unsigned int N, bool UseStartSentinel, char StartSentinel,
          bool UseStopSentinel, char StopSentinel, typename T>
void InsertionPolicyIteratorNGramAll<N, UseStartSentinel, StartSentinel,
                                     UseStopSentinel, StopSentinel,
                                     T>::increment_permutation() {
  std::vector<unsigned>::size_type pos = permutation.size() - 1;
  while (true) {
    ++permutation[pos];
    if (permutation[pos] >= permutation_limit) {
      // Done with everything
      if (pos == 0) {
        done = true;
        break;
      }

      permutation[pos] = 0;
      --pos;
    } else
      break;
  }
}

template <unsigned int N, bool UseStartSentinel, char StartSentinel,
          bool UseStopSentinel, char StopSentinel, typename T>
bool InsertionPolicyIteratorNGramAll<N, UseStartSentinel, StartSentinel,
                                     UseStopSentinel, StopSentinel,
                                     T>::permutation_contains_holes() {
  std::vector<unsigned>::const_iterator first = permutation.begin(),
                                        last = permutation.end();

  // Advanced first/last past any sentinels
  if (UseStartSentinel)
    for (; first != last && *first == 0; ++first)
      ;

  // If they are equal, then no sentinels at all
  if (first == last)
    return false;

  if (UseStopSentinel) {
    // Extra decrement/increment because last acts like .end() which points one
    // past
    --last;
    for (; first != last && *last == 0; --last)
      ;
    ++last;
  }

  // Between first and last, no sentinels can occur
  for (; first != last; ++first)
    if (*first == 0)
      return true;

  // None did
  return false;
}

template <unsigned int A, bool B, char C, bool D, char E, typename F>
bool operator==(const InsertionPolicyIteratorNGramAll<A, B, C, D, E, F> &lhs,
                const InsertionPolicyIteratorNGramAll<A, B, C, D, E, F> &rhs) {
  return (lhs.done && rhs.done) || lhs.permutation == rhs.permutation;
}
template <unsigned int A, bool B, char C, bool D, char E, typename F>
bool operator!=(const InsertionPolicyIteratorNGramAll<A, B, C, D, E, F> &lhs,
                const InsertionPolicyIteratorNGramAll<A, B, C, D, E, F> &rhs) {
  if (lhs.done && rhs.done)
    return false;
  return lhs.done != rhs.done || lhs.permutation != rhs.permutation;
}

typedef InsertionPolicy<InsertionPolicyProcessor<
    InsertionPolicyIteratorOneShot<> > > InsertionOneShot;
typedef InsertionPolicy<InsertionPolicyProcessor<
    InsertionPolicyIteratorNGram<> > > InsertionBigramWithSentinel;
typedef InsertionPolicy<InsertionPolicyProcessor<InsertionPolicyIteratorNGram<
    2, true, '|', true, '|'> > > InsertionBigramWithSameSentinel;
typedef InsertionPolicy<InsertionPolicyProcessor<InsertionPolicyIteratorNGram<
    2, true, '^', false> > > InsertionBigramOnlyStartSentinel;
typedef InsertionPolicy<InsertionPolicyProcessor<InsertionPolicyIteratorNGram<
    2, false, '^', true, '$'> > > InsertionBigramOnlyStopSentinel;
typedef InsertionPolicy<InsertionPolicyProcessor<InsertionPolicyIteratorNGram<
    2, false, '^', false, '$'> > > InsertionBigramNoSentinel;
typedef InsertionPolicy<InsertionPolicyProcessor<
    InsertionPolicyIteratorNGram<3> > > InsertionTrigramWithSentinel;
typedef InsertionPolicy<InsertionPolicyProcessor<InsertionPolicyIteratorNGram<
    3, false, '^', false, '$'> > > InsertionTrigramNoSentinel;
typedef InsertionPolicy<InsertionPolicyProcessor<
    InsertionPolicyIteratorNGram<4> > > InsertionQuadgramWithSentinel;
}

#endif
