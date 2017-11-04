/*
 * Copyright 2017 Will Mitchell
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
using std::cout;
using std::endl;
#include <functional>
using std::function;
#include <fstream>
using std::getline;
using std::ifstream;
#include <future>
using std::future;
#include <iterator>
using std::advance;
#include <string>
using std::string;
#include <thread>
#include <vector>
using std::vector;

#include "bfeattacks/Accumulator.h"
using bfeattacks::Accumulator;
#include "bfeattacks/FilterRequireExactly.h"
#include "bfeattacks/FilterSize.h"
#include "bfeattacks/SingleRecord.h"
#include "bloomfilter/BloomFilter.h"
using bloomfilter::BloomFilter;
using bloomfilter::BloomFilterStandard;
#include "bloomfilter/HashSet.h"
using bloomfilter::HashSetPair;
#include "bloomfilter/InsertionPolicy.h"
using bloomfilter::InsertionTrigramWithSentinel;
using bloomfilter::InsertionQuadgramWithSentinel;
#include "bfeattacks/ParallelAccumulator.h"
#include "graph/Traversals.h"
#include "util/ByteVector.h"
using util::ByteVector::toByteVector;
#include "util/String.h"
using util::stripNonAlpha;
using util::toLowerCase;
#include "util/Timer.h"
using util::Timer;

vector<string> loadAndFilter(string filename, function<string(string)> filter);

int main(const int argc, const char **argv) {
  if (argc <= 1) {
    cout << "Invalid usage. Pass filename as first argument." << endl;
    return 0;
  }

  unsigned numThreads = std::thread::hardware_concurrency();
  if (argc > 2)
    numThreads = static_cast<unsigned>(std::stoul(argv[2]));

  // Namelike
  const string alphabet = "abcdefghijklmnopqrstuvwxyz";
  auto filter = [](string s) { return toLowerCase(stripNonAlpha(s)); };

  // SSNlike
  //const string alphabet = "0123456789";
  //auto filter = [](string s) { return s; };

  cout << "Using " << numThreads << " threads.\n";

  string filename(argv[1]);
  Timer t;
  cout << "Will process file: '" << filename << "'\n";

  t.start();
  const vector<string> lines = loadAndFilter(filename, filter);
  t.stop();

  cout << "File loaded and filtered. " << lines.size() << " lines." << t << endl;

  const vector<graph::Traversal> traversals = {
    { graph::Traversal::depth_first_search, graph::Traversal::all_simple_paths,
      graph::Traversal::all_edge_disjoint_paths }
  };

  // Bigram
  typedef BloomFilterStandard BloomFilterType;
  const unsigned n = 2;
  // Trigram
  //typedef BloomFilter<HashSetPair, InsertionTrigramWithSentinel, true> BloomFilterType;
  //const unsigned n = 3;
  // Quadgram
  //typedef BloomFilter<HashSetPair, InsertionQuadgramWithSentinel, true> BloomFilterType;
  //const unsigned n = 4;

  const auto key1 = toByteVector("1111111111111111111111111111111111111111111111111111111111111111");
  const auto key2 = toByteVector("2222222222222222222222222222222222222222222222222222222222222222");
  const unsigned m = 1000;
  const unsigned k = 30;

  auto BFBuilder = [m, k, key1, key2]() {
    HashSetPair hs(k);
    hs.addHMAC(hash::SHA_256, key1).addHMAC(hash::SHA_256, key2);
    return bfeattacks::SingleRecord<BloomFilterType>(
        BloomFilterType(m, hs));
  };

  cout << "Using filter setup:\n" << BFBuilder() << "\n"
       << "Using alphabet: " << alphabet << endl;

  // No Filter
  /*
  auto BFFilter = [](bfeattacks::SingleRecord<BloomFilterType> &) {};
  cout << "Using no filters\n" << endl;
  //*/
  // Require Exactly filter
  //*
  auto BFFilter = [](bfeattacks::SingleRecord<BloomFilterType> &rec) {
    bfeattacks::filter_require_exactly(rec);
  };
  cout << "Using filter of only bfeattacks::filter_require_exactly" << endl;
  //*/
  // Length filter
  /*
  auto BFFilter = [n](bfeattacks::SingleRecord<BloomFilterType> &rec) {
    auto est_elts = std::lround(rec.estimate_elements());
    // n - 1 is the number of extra n-grams created past a word's length when
    // considering the sentinels
    size_t est_len = static_cast<size_t>(est_elts) - (n - 1);
    bfeattacks::filter_size(rec, est_len - 1, est_len + 1);
  };
  cout << "Using filter of only bfeattacks::filter_size" << endl;
  */

  t.start();
  auto stats = bfeattacks::ParallelAccumulate<BloomFilterType>(
      lines, BFBuilder, BFFilter, traversals, alphabet, 10, numThreads, cout, 0xFF);
  t.stop();

  cout << "Complete. Total of " << lines.size() << " lines." << t << endl;

  cout << stats << endl;

  return 0;
}

vector<string> loadAndFilter(string filename, function<string(string)> filter) {
  ifstream input(filename);

  vector<string> lines;

  for (string line; getline(input, line);)
    lines.push_back(filter(line));

  return lines;
}
