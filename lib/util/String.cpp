//===-- util/String.cpp - Miscellaneous string functions ------------------===//
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
//
//===----------------------------------------------------------------------===//
#include "util/String.h"

#include <algorithm>
using std::remove_if;
#include <cctype>
using std::isalpha;
#include <string>
using std::string;

#include <boost/algorithm/string.hpp>

string util::stripNonAlpha(string s) {
  s.erase(remove_if(s.begin(), s.end(), [](char a) { return !isalpha(a); }),
	    s.end());
  return s;
}

string util::toLowerCase(string s) {
    boost::algorithm::to_lower(s);
    return s;
}
