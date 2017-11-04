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
#include <vector>
using std::vector;

#include "bloomfilter/BloomFilter.h"
using bloomfilter::BloomFilterStandard;
#include "bloomfilter/HashSet.h"
using bloomfilter::HashSetPair;
#include "bloomfilter/InsertionPolicy.h"
using bloomfilter::InsertionBigramWithSentinel;

#include "util/ByteVector.h"
using util::ByteVector::toByteVector;

#include "bfeattacks/SingleRecord.h"
#include "bfeattacks/FilterRequireExactly.h"

int main(int /*argc*/, char **/*argv*/) {

  const auto alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const auto data = "WILLIAM";
  const auto k = 6u;
  const auto m = 200u;
  //const auto n = 2u; // implied by used of bigram insertion policy

  auto key1 = toByteVector("1111111111111111111111111111111111111111111111111111111111111111");
  auto key2 = toByteVector("2222222222222222222222222222222222222222222222222222222222222222");


  // Print out contents of filter itself

  // Construct the filter
  HashSetPair hs(k);
  hs.addHMAC(hash::SHA_256, key1).addHMAC(hash::SHA_256, key2);
  //BloomFilterStandard filter(m, hs);
  bfeattacks::SingleRecord<BloomFilterStandard> rec(BloomFilterStandard(m, hs));

  cout << "After filter construction==========\n"
       << rec.bf << "\n";

  // Populate the bloom filter
  rec.insert(data);

  cout << "Insert \"" << data << "\"==========\n"
       << rec.bf << "\n";

  // Run the attack
  rec.construct_graph(alphabet);

  cout << "After attack \"" << data << "\"==========\n"
       << rec.bf <<"\n";

  cout << "Filter: ";
  for (auto i = 0u; i < m - 4; i += 4) {
    auto character = (rec.bf.raw()[i] << 3) + (rec.bf.raw()[i + 1] << 2) +
                     (rec.bf.raw()[i + 2] << 1) + rec.bf.raw()[i + 3];

    switch (character) {
    case 0:
      cout << "0";
      break;
    case 1:
      cout << "1";
      break;
    case 2:
      cout << "2";
      break;
    case 3:
      cout << "3";
      break;
    case 4:
      cout << "4";
      break;
    case 5:
      cout << "5";
      break;
    case 6:
      cout << "6";
      break;
    case 7:
      cout << "7";
      break;
    case 8:
      cout << "8";
      break;
    case 9:
      cout << "9";
      break;
    case 10:
      cout << "A";
      break;
    case 11:
      cout << "B";
      break;
    case 12:
      cout << "C";
      break;
    case 13:
      cout << "D";
      break;
    case 14:
      cout << "E";
      break;
    case 15:
      cout << "F";
      break;
    }
  }
  cout << "\n";

  // Run the traversals
  vector<graph::Traversal> traversals = {
    { graph::Traversal::depth_first_search, graph::Traversal::all_simple_paths,
      graph::Traversal::all_edge_disjoint_paths }
  };

  rec.setup_traversals(traversals);
  for (const auto i : traversals)
    rec.run_traversal(i);
  rec.simplify_paths();

  cout << "Found paths ==========\n";
  // Detected paths
  for (const auto i : traversals) {
    cout << "Traversal: " << "\n";
    for (const auto j : rec.get_simplified_paths(i))
      cout << j << " ";
    cout << "\n";
  }


  bfeattacks::filter_require_exactly(rec);

  cout << "After filter_require_exactly ==========\n";
  // Detected paths
  for (const auto i : traversals) {
    cout << "Traversal: " << i << "\n";
    for (const auto j : rec.get_simplified_paths(i))
      cout << j << " ";
    cout << "\n";
  }



  return 0;
}
