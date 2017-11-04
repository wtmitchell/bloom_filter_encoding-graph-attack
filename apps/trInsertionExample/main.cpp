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

#include "bloomfilter/BloomFilter.h"
using bloomfilter::BloomFilterStandard;
#include "bloomfilter/HashSet.h"
using bloomfilter::HashSetPair;
#include "bloomfilter/InsertionPolicy.h"
using bloomfilter::InsertionBigramWithSentinel;

#include "util/ByteVector.h"
using util::ByteVector::toByteVector;

int main(int /*argc*/, char **/*argv*/) {

  const auto data = "SMITH";
  const auto k = 3u;
  const auto m = 35u;
  //const auto n = 2u; // implied by used of bigram insertion policy

  auto key1 = toByteVector("1111111111111111111111111111111111111111111111111111111111111111");
  auto key2 = toByteVector("2222222222222222222222222222222222222222222222222222222222222222");


  // Print out hashes of individual bigrams
  auto hash1 = hash::getHMACHash(hash::SHA_256, key1);
  auto hash2 = hash::getHMACHash(hash::SHA_256, key2);

  InsertionBigramWithSentinel ip;
  auto bigrams = ip.process(data);

  cout << "H_1: " << hash1->name() << "\n"
       << "H_2: " << hash2->name() << "\n\n";

  cout << "\\begin{xalignat*}{2}\n";
  for (const auto &i : bigrams) {
    auto h1 = hash1->calculate(i);
    auto h2 = hash2->calculate(i);

    cout << "\\textrm{HMAC}_{\\textrm{SHA-256}, k_1}(\\texttt{" << i
         << "}) &\\equiv " << h1 % m << " \\pmod{" << m << "} & "
         << "\\textrm{HMAC}_{\\textrm{SHA-256}, k_2}(\\texttt{" << i
         << "}) &\\equiv " << h2 % m << " \\pmod{" << m << "}\\\\\n";
  }
  cout << "\\end{xalignat*}\n";

  cout << "\n==========================\n\n";


  // Print out contents of filter itself

  // Construct the filter
  HashSetPair hs(k);
  hs.addHMAC(hash::SHA_256, key1).addHMAC(hash::SHA_256, key2);
  BloomFilterStandard filter(m, hs);

  cout << "After filter construction==========\n"
       << filter;

  // Populate the bloom filter
  filter.insert(data);

  cout << "\nInsert \"" << data << "\"==========\n"
       << filter;



  return 0;
}
