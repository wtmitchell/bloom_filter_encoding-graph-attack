#ifndef TEST_HASHCOMMON_H_INCLUDED
#define TEST_HASHCOMMON_H_INCLUDED
//===-- test/hash/HashCommon.h - Common testing for hashes ------*- C++ -*-===//
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
/// \brief This file contains common testing routines for hash functions
///
//===----------------------------------------------------------------------===//

#include "hash/HashFunction.h"

#include <memory>
#include <string>

namespace test {
namespace hash {
void testHash(const std::unique_ptr< ::hash::IncrementalHashFunction> &hash,
              const std::string data, const std::string expected_hash);
void testHashBytes(const std::unique_ptr< ::hash::IncrementalHashFunction> &hash,
              const std::string data, const std::string expected_hash);
void
testHashIterated(const std::unique_ptr< ::hash::IncrementalHashFunction> &hash,
                 const std::string data, const int repetitions,
                 const std::string expected_hash);

// TODO: Implement the below correctly
// void
// testIncrementalHash(::hash::IncrementalHashFunction &hash,
//                    const std::string data, const std::string expected_hash);
//}
}
}
#endif
