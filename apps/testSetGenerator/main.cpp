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
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <random>
using std::mt19937;
using std::uniform_int_distribution;
#include <string>
using std::string;

#include <boost/algorithm/string.hpp>

#include "bloomfilter/BloomFilter.h"
using bloomfilter::BloomFilterStandard;
#include "bloomfilter/HashSet.h"
using bloomfilter::HashSetPair;
#include "util/ByteVector.h"
using util::ByteVector::toByteVector;
#include "util/Hexadecimal.h"
#include "util/random/String.h"
using util::random::uniformString;

string encode(const string& in, const string& key1, const string& key2);
string normalize(const string& in);

string encode(const string& in, const string& skey1, const string& skey2) {
  auto key1 = toByteVector(skey1);
  auto key2 = toByteVector(skey2);
  const unsigned m = 1000;
  const unsigned k = 30;

  // HashSetPair generates k hashes from just two hashes. They are of the form
  // h_i(x) = hash_1(x) + i * hash_2(b), where i ranges from 0 to k-1.
  HashSetPair hs(k);
  // These HMAC hashes follow the standard HMAC nested hashing using ipad and opad
  // from FIPS-180
  hs.addHMAC(hash::SHA_256, key1).addHMAC(hash::SHA_256, key2);

  // BloomFilterStandard encoded the inserted words using bigrams, the start
  // sentenel of ^ and the stop sentinel of $.
  BloomFilterStandard filter(m, hs);

  filter.insert(in);

  string binary;
  // Store into out the binary string in msb to lsb order
  boost::to_string(filter.raw(), binary);

  // Hex now contains encoding in lsb to msb order
  string hex = util::binaryStringToHexString(binary.rbegin(), binary.rend());

  return hex;
}

string normalize(const string& in) {
  // No filtering
  return in;
  /*
  string out{in};
  // Convert to all lower case
  boost::algorithm::to_lower(out);

  // Remove characters that fail isalpha
  out.erase(remove_if(out.begin(), out.end(), [](char a) { return !isalpha(a); }),
	    out.end());
  return out;
  */
}

int main(int argc, const char* argv[]) {
  // Open file on argv[1] as input, and derive names for output
  if (argc != 2) {
    cerr << "Invalid usage. First parameter should be source filename" << endl;
    return EXIT_FAILURE;
  }

  string inputFilename{argv[1]};
  string filteredFilename{inputFilename + ".filtered"};
  string encodedFilename{inputFilename + ".encoded"};
  string keysFilename{inputFilename + ".keys"};


  cout << "Reading from: " << inputFilename << "\n"
       << "Writing filtered input to: " << filteredFilename << "\n"
       << "Writing encoded to: " << encodedFilename << "\n"
       << "With keys to: " << keysFilename << endl;

  ifstream input(inputFilename, std::ios::in);
  if (!input) {
    cerr << "Failed to open " << inputFilename << " for reading" << endl;
    return EXIT_FAILURE;
  }

  ofstream filtered(filteredFilename, std::ios::out);
  if (!filtered) {
    cerr << "Failed to open " << filteredFilename << " for writing" << endl;
    return EXIT_FAILURE;
  }

  ofstream encoded(encodedFilename, std::ios::out);
  if (!encoded) {
    cerr << "Failed to open " << encodedFilename << " for writing" << endl;
    return EXIT_FAILURE;
  }

  ofstream keys(keysFilename, std::ios::out);
  if (!keys) {
    cerr << "Failed to open " << keysFilename << " for writing" << endl;
    return EXIT_FAILURE;
  }

  // Generate random keys
  // Set up Mersenne Twister RNG
  // Use a set seed
  //mt19937 mt(1);

  // Use a good seed, if possible
  std::random_device seed;
  mt19937 mt(seed());

  const string hexAlpha = "0123456789ABCDEF";

  uniform_int_distribution<string::size_type> uniform_dist(0, hexAlpha.size() - 1);

  string key1 = uniformString(hexAlpha, 64, uniform_dist, mt);
  string key2 = uniformString(hexAlpha, 64, uniform_dist, mt);

  keys << "key1: " << key1 << "\n" << "key2: " << key2 << endl;
  keys.close();
  cout << "Keys (" << key1 << ", " << key2 << ") written." << endl;

  // For each line of input, shove it in a Bloom Filter and write it as output
  for(string line; std::getline(input, line);) {
    auto normed = normalize(line);
    filtered << normed << "\n";
    encoded << encode(normed, key1, key2) << "\n";
  }

  encoded.close();

  return 0;
}
