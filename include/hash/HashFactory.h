//===-- hash/HashFactory.h - Factory for Hashes --------------*- C++ -*----===//
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
/// \brief This file returns hash function pointers for a specified hash
///
//===----------------------------------------------------------------------===//
#ifndef HASH_HASHFACTORY_H_INCLUDED
#define HASH_HASHFACTORY_H_INCLUDED

#include <memory>
#include <vector>

#include "hash/HashFunction.h"

namespace hash {
enum Hashes {
  BLAKE2B_224,
  BLAKE2B_256,
  BLAKE2B_384,
  BLAKE2B_512,
  MD5,
  SHA1,
  SHA_224,
  SHA_256,
  SHA_384,
  SHA_512,
  SHA3_224,
  SHA3_256,
  SHA3_384,
  SHA3_512
};

std::unique_ptr<IncrementalHashFunction> getHash(const Hashes hash);
std::unique_ptr<IncrementalHashFunction>
getHMACHash(const Hashes hash, const std::vector<byte> &Key);
}

#endif
