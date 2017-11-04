//===-- util/String.h - Miscellaneous string functions -------*- C++ -*----===//
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
/// \brief This file contains miscellaneous functions useful when working with
/// std::string
///
//===----------------------------------------------------------------------===//
#ifndef UTIL_VECTOR_H_INCLUDED
#define UTIL_VECTOR_H_INCLUDED

#include <string>

namespace util {
  std::string stripNonAlpha(std::string s);
  std::string toLowerCase(std::string s);
}

#endif
