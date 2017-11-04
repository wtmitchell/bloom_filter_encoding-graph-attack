//===-- util/random/String.h - Random String Creation ----------------*- C++ -*-===//
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
/// \brief This file defines basic routines for creating random strings
///
//===----------------------------------------------------------------------===//
#ifndef UTIL_RANDOM_STRING_H_INCLUDED
#define UTIL_RANDOM_STRING_H_INCLUDED

#include <string>
#include <random>

namespace util {
namespace random {
template <typename T>
std::string uniformString(std::string alphabet, unsigned length,
                         std::uniform_int_distribution<string::size_type> &dist,
                         T &rand_source);

template <typename T>
std::string uniformString(std::string alphabet, unsigned length,
                         std::uniform_int_distribution<string::size_type> &dist,
                         T &rand_source) {
  std::string ret;

  for (unsigned i = 0; i < length; ++i)
    ret += alphabet[dist(rand_source)];

  return ret;
}
}
}

#endif
