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

#include <algorithm>
using std::remove_if;
#include <cctype>
using std::isalpha;
#include <cmath>
using std::sqrt;
#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::getline;
using std::ifstream;
#include <string>
using std::string;
#include <utility>
using std::pair;
#include <vector>
using std::vector;

#include "bloomfilter/BloomFilter.h"
using bloomfilter::BloomFilter;
using bloomfilter::BloomFilterStandard;
#include "bloomfilter/HashSet.h"
using bloomfilter::HashSetPair;
#include "bloomfilter/InsertionPolicy.h"
using bloomfilter::InsertionBigramWithSentinel;
using bloomfilter::InsertionTrigramWithSentinel;
#include "hash/HashFactory.h"
#include "util/Timer.h"
using util::Timer;

#include "graph/Traversals.h"

#include <boost/algorithm/string.hpp>

#include "bfeattacks/FilterDictionary.h"
#include "bfeattacks/FilterRequireExactly.h"
#include "bfeattacks/FilterSize.h"
#include "bfeattacks/SingleRecord.h"

#include "bfeattacks/Accumulator.h"
using bfeattacks::Accumulator;

void test_bfe(string test, string alphabet,
              vector<graph::Traversal> &traversals, Accumulator &stats);

void test_bfe(string test, string alphabet,
              vector<graph::Traversal> &traversals, Accumulator &stats) {
  // Construct the record
  HashSetPair hs(30);
  hs.add(hash::MD5).add(hash::SHA3_256);
  bfeattacks::SingleRecord<BloomFilterStandard> rec(
      BloomFilterStandard(1000, hs));

  // Populate the bloom filter
  rec.insert(test);

  // Construct the graph
  rec.construct_graph(alphabet);
  //std::ofstream outfile(test + ".graphml");
  //rec.write_graphml(outfile);
  //outfile.close();

  //cout << rec.bf << endl;
  // cout << "estimated elements: " << rec.estimate_elements() << endl;

  // Run the traversals
  rec.setup_traversals(traversals);
  for (const auto i : traversals)
    rec.run_traversal(i);
  rec.simplify_paths();

  // Apply filters
  auto est_elts = std::lround(rec.estimate_elements());
  size_t est_len =
      static_cast<size_t>(est_elts) - 1; // 1 is n of n-grams minus 1
  // cout << "Filter len [" << est_len - 1 << ", " << est_len + 1 << "]" <<
  // endl;
  bfeattacks::filter_size(rec, est_len - 1, est_len + 1);
  bfeattacks::filter_require_exactly(rec);

  // bfeattacks::filter_dictionary(rec);

  // Collect stats
  stats.add(rec);
}

int main(int argc, char **argv) {
  vector<graph::Traversal> traversals = {
    { graph::Traversal::depth_first_search, graph::Traversal::all_simple_paths,
      graph::Traversal::all_edge_disjoint_paths }
  };
  Accumulator stats(traversals);

  if (argc <= 1) {
    // Smoke test
    // cout << "Test [1/4]--------------------------------------\n";
    // test_bfe("mississippi", "abcdefghijklmnopqrstuvwxyz", traversals, stats);
    // cout << "Test [2/4]--------------------------------------\n";
    // test_bfe("william", "abcdefghijklmnopqrstuvwxyz", traversals, stats);
    test_bfe("william", "abcdefghijklmnopqrstuvwxyz", traversals, stats);
    // cout << "Test [3/4]--------------------------------------\n";
    // test_bfe("ramakrishna", "abcdefghijklmnopqrstuvwxyz", traversals, stats);
    // cout << "Test [4/4]--------------------------------------\n";
    test_bfe("max", "abcdefghijklmnopqrstuvwxyz", traversals, stats);
    // test_bfe("youngbloodstillwell", "abcdefghijklmnopqrstuvwxyz", traversals,
    // stats);

    cout << stats << endl;
    return 0;
  }

  string filename(argv[1]);
  cout << "Will process file: '" << filename << "'\n";
  ifstream input(filename);

  for (string line; getline(input, line);) {
    // Clean up the string to be sure
    boost::algorithm::to_lower(line);
    line.erase(
        remove_if(line.begin(), line.end(), [](char a) { return !isalpha(a); }),
        line.end());

    test_bfe(line, "abcdefghijklmnopqrstuvwxyz", traversals, stats);
  }

  cout << stats << endl;

  return 0;
}
