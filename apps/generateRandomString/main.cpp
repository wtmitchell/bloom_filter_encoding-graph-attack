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
#include <random>
using std::mt19937;
using std::uniform_int_distribution;
#include <string>
using std::stoul;
using std::string;
#include <utility>
using std::pair;
#include <vector>
using std::vector;

#include <boost/algorithm/string.hpp>

template <typename T>
string randomstring(string alphabet, unsigned length,
                    uniform_int_distribution<string::size_type> &dist,
                    T &rand_source);

template <typename T>
string randomstring(string alphabet, unsigned length,
                    uniform_int_distribution<string::size_type> &dist,
                    T &rand_source) {
  string ret;

  for (unsigned i = 0; i < length; ++i)
    ret += alphabet[dist(rand_source)];

  return ret;
}

int main(int argc, char **argv) {

  //string alphabet = "1234567890";
  string alphabet = "abcdefghijklmnopqrstuvwxyz";
  unsigned long count = 10000;
  unsigned length = 9;

  // Parse command line
  if (argc > 1) {
    try {
      count = stoul(argv[1]);
    }
    catch (std::invalid_argument e) {
      cout << "Invalid number of samples '" << argv[1] << "'" << endl;
    }
  }
  if (argc > 2) {
    try {
      length = static_cast<unsigned>(stoul(argv[2]));
    }
    catch (std::invalid_argument e) {
      cout << "Invalid sample length of '" << argv[2] << "'" << endl;
    }
  }
  if (argc > 3)
    alphabet = argv[3];

  // Set up RNG
  // Use a set seed
  mt19937 mt(12345);
  // Use a good seed, if possible
  // std::random_device seed;
  // mt19937 mt(seed);

  uniform_int_distribution<string::size_type> uniform_dist(0,
                                                           alphabet.size() - 1);

  // cout << "Alphabet: " << alphabet << "\n"
  //      << "Length: " << length << "\n"
  //      << "Number of trials: " << count << endl;

  for (unsigned long i = 0; i < count; ++i) {
    string rand = randomstring(alphabet, length, uniform_dist, mt);
    cout << rand << "\n";
  }

  return 0;
}
