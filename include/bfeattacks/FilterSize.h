//===-- bfeattacks/FilterSize.h - A Record filter ------------*- C++ -*----===//
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
/// \brief This file contains filter_size which attempts to filter
/// the false paths found by their size.
///
//===----------------------------------------------------------------------===//
#ifndef BFEATTACKS_FILTERSIZE_H_INCLUDED
#define BFEATTACKS_FILTERSIZE_H_INCLUDED

#include <algorithm>

#include "bfeattacks/SingleRecord.h"

namespace bfeattacks {
template <typename T>
void filter_size(bfeattacks::SingleRecord<T> &record,
                 const std::string::size_type min,
                 const std::string::size_type max);
}

template <typename T>
void bfeattacks::filter_size(bfeattacks::SingleRecord<T> &record,
                             const std::string::size_type min,
                             const std::string::size_type max) {
  for (auto &i : record.simplified_paths) {
    i.erase(
        std::remove_if(i.begin(), i.end(), [&record, min, max](std::string s) {
          return !(min <= s.size() && s.size() <= max);
        }),
        i.end());
  }
}

#endif
