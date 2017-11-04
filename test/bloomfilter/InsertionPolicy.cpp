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

#include <string>
using std::string;

#include <iomanip>
using std::hex;
using std::setw;
using std::setfill;

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include "bloomfilter/InsertionPolicy.h"
using bloomfilter::InsertionOneShot;
using bloomfilter::InsertionBigramWithSentinel;
using bloomfilter::InsertionBigramWithSameSentinel;
using bloomfilter::InsertionBigramOnlyStartSentinel;
using bloomfilter::InsertionBigramOnlyStopSentinel;
using bloomfilter::InsertionBigramNoSentinel;
using bloomfilter::InsertionTrigramWithSentinel;
using bloomfilter::InsertionTrigramNoSentinel;

TEST(InsertionPolicy, OneShot) {
  typedef InsertionOneShot IP;
  IP ip;
  IP::processor p = ip.process("abcdefgh");

  IP::processor::iterator i = p.begin(), e = p.end();
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "abcdefgh");
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, BigramWithSentinel) {
  typedef InsertionBigramWithSentinel IP;
  IP ip;

  IP::processor p = ip.process("abcdefgh");

  IP::processor::iterator i = p.begin(), e = p.end();

  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "^a");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "ab");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "bc");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "cd");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "de");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "ef");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "fg");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "gh");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "h$");
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, BigramWithSameSentinel) {
  typedef InsertionBigramWithSameSentinel IP;
  IP ip;

  IP::processor p = ip.process("abcdefgh");

  IP::processor::iterator i = p.begin(), e = p.end();

  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "|a");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "ab");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "bc");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "cd");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "de");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "ef");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "fg");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "gh");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "h|");
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, BigramOnlyStartSentinel) {
  typedef InsertionBigramOnlyStartSentinel IP;
  IP ip;

  IP::processor p = ip.process("abcdefgh");

  IP::processor::iterator i = p.begin(), e = p.end();

  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "^a");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "ab");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "bc");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "cd");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "de");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "ef");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "fg");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "gh");
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, BigramOnlyStopSentinel) {
  typedef InsertionBigramOnlyStopSentinel IP;
  IP ip;

  IP::processor p = ip.process("abcdefgh");

  IP::processor::iterator i = p.begin(), e = p.end();

  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "ab");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "bc");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "cd");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "de");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "ef");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "fg");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "gh");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "h$");
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, BigramNoSentinel) {
  typedef InsertionBigramNoSentinel IP;
  IP ip;

  IP::processor p = ip.process("abcdefgh");

  IP::processor::iterator i = p.begin(), e = p.end();

  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "ab");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "bc");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "cd");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "de");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "ef");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "fg");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "gh");
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, TrigramWithSentinel) {
  typedef InsertionTrigramWithSentinel IP;
  IP ip;

  IP::processor p = ip.process("abcdefgh");

  IP::processor::iterator i = p.begin(), e = p.end();

  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "^^a");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "^ab");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "abc");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "bcd");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "cde");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "def");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "efg");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "fgh");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "gh$");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "h$$");
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, TrigramNoSentinel) {
  typedef InsertionTrigramNoSentinel IP;
  IP ip;

  IP::processor p = ip.process("abcdefgh");

  IP::processor::iterator i = p.begin(), e = p.end();

  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "abc");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "bcd");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "cde");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "def");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "efg");
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ(*i, "fgh");
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, BigramNoSentinelAll) {

  typedef bloomfilter::InsertionPolicyIteratorNGramAll<2, false, '^', false,
                                                       '$'> BigramIterator;

  BigramIterator i("abc", 0);
  BigramIterator e("abc", -1);

  EXPECT_NE(i, e);
  EXPECT_EQ("aa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cc", *i);
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, TrigramNoSentinelAll) {

  typedef bloomfilter::InsertionPolicyIteratorNGramAll<3, false, '^', false,
                                                       '$'> BigramIterator;

  BigramIterator i("abc", 0);
  BigramIterator e("abc", -1);

  EXPECT_NE(i, e);
  EXPECT_EQ("aaa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("abb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("abc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("acb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("acc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("baa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bbb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bbc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bcb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bcc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("caa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cbb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cbc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ccb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ccc", *i);
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, BigramWithSentinelAll) {

  typedef bloomfilter::InsertionPolicyIteratorNGramAll<2, true, '^', true, '$'>
  BigramIterator;

  BigramIterator i("abc", 0);
  BigramIterator e("abc", -1);

  EXPECT_NE(i, e);
  EXPECT_EQ("^a", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^b", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^c", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("a$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("b$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("c$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cc", *i);
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, TrigramWithSentinelAll) {

  typedef bloomfilter::InsertionPolicyIteratorNGramAll<3, true, '^', true, '$'>
  BigramIterator;

  BigramIterator i("abc", 0);
  BigramIterator e("abc", -1);

  EXPECT_NE(i, e);
  EXPECT_EQ("^^a", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^^b", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^^c", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^a$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^aa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^ab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^ac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^b$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^ba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^bb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^bc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^c$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^ca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^cb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^cc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("a$$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aa$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aaa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ab$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("abb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("abc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ac$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("acb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("acc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("b$$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ba$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("baa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bb$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bbb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bbc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bc$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bcb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bcc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("c$$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ca$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("caa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cb$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cbb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cbc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cc$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ccb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ccc", *i);
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, TrigramWithSameSentinelAll) {

  typedef bloomfilter::InsertionPolicyIteratorNGramAll<3, true, '|', true, '|'>
  BigramIterator;

  BigramIterator i("abc", 0);
  BigramIterator e("abc", -1);

  EXPECT_NE(i, e);
  EXPECT_EQ("||a", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("||b", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("||c", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|a|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|aa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|ab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|ac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|b|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|ba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|bb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|bc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|c|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|ca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|cb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("|cc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("a||", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aa|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aaa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ab|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("abb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("abc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ac|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("acb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("acc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("b||", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ba|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("baa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bb|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bbb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bbc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bc|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bcb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bcc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("c||", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ca|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("caa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cb|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cbb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cbc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cc|", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ccb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ccc", *i);
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, TrigramOnlyStartSentinelAll) {

  typedef bloomfilter::InsertionPolicyIteratorNGramAll<3, true, '^', false, '$'>
  BigramIterator;

  BigramIterator i("abc", 0);
  BigramIterator e("abc", -1);

  EXPECT_NE(i, e);
  EXPECT_EQ("^^a", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^^b", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^^c", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^aa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^ab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^ac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^ba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^bb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^bc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^ca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^cb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("^cc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aaa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("abb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("abc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("acb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("acc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("baa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bbb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bbc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bcb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bcc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("caa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cbb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cbc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ccb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ccc", *i);
  ++i;
  EXPECT_EQ(i, e);
}

TEST(InsertionPolicy, TrigramOnlyStopSentinelAll) {

  typedef bloomfilter::InsertionPolicyIteratorNGramAll<3, false, '^', true, '$'>
  BigramIterator;

  BigramIterator i("abc", 0);
  BigramIterator e("abc", -1);

  EXPECT_NE(i, e);
  EXPECT_EQ("a$$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aa$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aaa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ab$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("abb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("abc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ac$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("aca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("acb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("acc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("b$$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ba$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("baa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bb$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bbb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bbc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bc$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bcb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("bcc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("c$$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ca$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("caa", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cab", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cac", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cb$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cba", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cbb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cbc", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cc$", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("cca", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ccb", *i);
  ++i;
  EXPECT_NE(i, e);
  EXPECT_EQ("ccc", *i);
  ++i;
  EXPECT_EQ(i, e);
}
