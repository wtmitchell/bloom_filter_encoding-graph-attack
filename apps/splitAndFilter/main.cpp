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
#include <fstream>
using std::getline;
using std::ifstream;
using std::ofstream;
#include <map>
using std::map;
#include <string>
using std::string;
using std::to_string;
#include <utility>
using std::make_pair;
#include <vector>
using std::vector;

#include "util/String.h"
using util::stripNonAlpha;
using util::toLowerCase;
#include "util/Timer.h"
using util::Timer;

int main(const int argc, const char **argv) {
  if (argc <= 1) {
    cout << "Invalid usage. Pass filename as first argument." << endl;
    return 0;
  }

  string infilename(argv[1]);
  Timer t;
  cout << "Will process file: '" << infilename << "'\n";

  auto filter = [](string s) { return toLowerCase(stripNonAlpha(s)); };

  ifstream input(infilename);

  map<string::size_type, ofstream *> output;

  t.start();
  for (string line; getline(input, line);) {
    string filtered = filter(line);
    string::size_type length = filtered.length();

    if (output.count(length) == 0) {
      string outfilename = infilename + "." + to_string(length);
      cout << "Opening " << outfilename << endl;
      output[length] = new ofstream(outfilename);
    }

    *output[length] << filtered << "\n";
  }

  for (auto &i : output) {
    i.second->close();
    delete i.second;
  }
  t.stop();

  cout << "File processed." << t << endl;

  return 0;
}
