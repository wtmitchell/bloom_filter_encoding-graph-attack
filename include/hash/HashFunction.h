//===-- hash/HashFunction.h - Hash Function class definitions ---*- C++ -*-===//
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
/// \brief This file contains the declaration of the HashFunction class and
/// IncrementalHashFunction class, which are the base classes for all hash
/// functions
///
//===----------------------------------------------------------------------===//
#ifndef HASH_HASHFUNCTION_H_INCLUDED
#define HASH_HASHFUNCTION_H_INCLUDED

#include <cstddef>
#include <string>
#include <vector>

#include "util/Types.h"

/// \brief hash contains all hash function related code, including cryptographic
/// and non-cryptographic hashes and checksums
namespace hash {
/// \brief Abstract base class for all hash functions
class HashFunction {
public:
  HashFunction() = default;

  virtual ~HashFunction() = default;
  HashFunction(const HashFunction& other) = default;
  HashFunction(HashFunction&& other) = default;
  HashFunction& operator=(const HashFunction& other) = default;
  HashFunction& operator=(HashFunction&& other) = default;

  /// \brief Calculates the hash
  virtual std::vector<byte> calculate(const byte in[],
                                      const std::size_t length) = 0;
  /// \brief Calculates the hash
  virtual std::vector<byte> calculate(const std::vector<byte> &in) = 0;
  /// \brief Calculates the hash
  virtual std::vector<byte> calculate(const std::string &in) = 0;
  /// \brief Returns a human readable name for the current hash function
  virtual std::string name() const;
  /// \brief Returns the number of bytes in the output of the hash
  virtual std::size_t output_length() const = 0;
  /// \brief Returns a clone of the current object
  virtual HashFunction* clone() const = 0;
};

/// \brief Abstract base class for hash functions that can be computed
/// incrementally
class IncrementalHashFunction : public HashFunction {
public:
  IncrementalHashFunction() = default;

  virtual ~IncrementalHashFunction() = default;
  IncrementalHashFunction(const IncrementalHashFunction& other) = default;
  IncrementalHashFunction(IncrementalHashFunction&& other) = default;
  IncrementalHashFunction& operator=(const IncrementalHashFunction& other) = default;
  IncrementalHashFunction& operator=(IncrementalHashFunction&& other) = default;

  /// \brief Adds param to the hash and returns the hash. Equivalent
  /// to an update() followed by final()
  virtual std::vector<byte> calculate(const byte in[],
                                      const std::size_t length);
  /// \brief Adds param to the hash and returns the hash. Equivalent
  /// to an update() followed by final()
  virtual std::vector<byte> calculate(const std::vector<byte> &in);
  /// \brief Adds param to the hash and returns the hash. Equivalent
  /// to an update() followed by final()
  virtual std::vector<byte> calculate(const std::string &in);

  /// \brief Finishes an incremental computation and returns the hash
  virtual std::vector<byte> final() = 0;
  /// \brief Adds in to the hash being computed
  virtual void update(const byte in[], const std::size_t length) = 0;
  /// \brief Adds in to the hash being computed
  virtual void update(const std::vector<byte> &in) = 0;
  /// \brief Adds in to the hash being computed
  virtual void update(const std::string &in) = 0;
};
}

#endif
