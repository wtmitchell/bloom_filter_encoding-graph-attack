//===-- hash/HashFactory.cpp - Factory for Hashes ------------*- C++ -*----===//
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
//===----------------------------------------------------------------------===//

#include "hash/HashFactory.h"

#include <vector>
using std::vector;

#include "hash/HashFunction.h"
using hash::HashFunction;
#include "util/ByteVector.h"
using util::ByteVector::toString;

// Since MD5 is considered weak, cryptopp puts it in the Weak namespace
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/hmac.h>
#include <cryptopp/md5.h>
#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>

// CryptoPP Hash wrapper and HMAC wrapper
namespace {
// Templated wrapper around a hash function from the Crypto++ library
template <typename H>
class CryptoPPHash : public hash::IncrementalHashFunction {
public:
  CryptoPPHash() : impl() {}
  ~CryptoPPHash() = default;
  CryptoPPHash(const CryptoPPHash<H>& other) = default;
  CryptoPPHash(CryptoPPHash<H>&& other) = default;
  CryptoPPHash<H>& operator=(const CryptoPPHash<H>& other) = default;
  CryptoPPHash<H>& operator=(CryptoPPHash<H>&& other) = default;

  std::vector<byte> final() override;
  std::string name() const override;
  std::size_t output_length() const override;
  void update(const byte in[], const std::size_t length) override;
  void update(const std::vector<byte> &in) override;
  void update(const std::string &in) override;
  HashFunction* clone() const override;

private:
  // PIMPL of actual hash
  H impl;
};

template <typename H> std::vector<byte> CryptoPPHash<H>::final() {
  std::vector<byte> out(impl.DigestSize(), 0);
  impl.Final(out.data());
  return out;
}

template <typename H> std::string CryptoPPHash<H>::name() const {
  return impl.AlgorithmName();
}

template <typename H> std::size_t CryptoPPHash<H>::output_length() const {
  return impl.DigestSize();
}

template <typename H>
void CryptoPPHash<H>::update(const byte in[], const size_t length) {
  impl.Update(in, length);
}

template <typename H>
void CryptoPPHash<H>::update(const std::vector<byte> &in) {
  update(in.data(), in.size());
}

template <typename H> void CryptoPPHash<H>::update(const std::string &in) {
  // reinterpret needed to cast from char* to byte* since byte is explicitly
  // unsigned
  update(reinterpret_cast<const byte *>(in.data()), in.length());
}

template <typename H> HashFunction *CryptoPPHash<H>::clone() const {
  return new CryptoPPHash<H>(*this);
}

// Templated wrapper to implement HMAC
template <typename H>
class CryptoPPHMAC : public hash::IncrementalHashFunction {
public:
  CryptoPPHMAC(const std::vector<byte> &Key) : K(Key) {
    impl.SetKey(K.data(), K.size());
  }
  ~CryptoPPHMAC() = default;
  CryptoPPHMAC(const CryptoPPHMAC<H>& other) = default;
  CryptoPPHMAC(CryptoPPHMAC<H>&& other) = default;
  CryptoPPHMAC<H>& operator=(const CryptoPPHMAC<H>& other) = default;
  CryptoPPHMAC<H>& operator=(CryptoPPHMAC<H>&& other) = default;

  std::vector<byte> final() override;
  std::string name() const override;
  std::size_t output_length() const override;
  void update(const byte in[], const std::size_t length) override;
  void update(const std::vector<byte> &in) override;
  void update(const std::string &in) override;
  HashFunction* clone() const override;

private:
  // PIMPL of hash
  CryptoPP::HMAC<H> impl;
  const std::vector<byte> K;
};

template <typename H> std::vector<byte> CryptoPPHMAC<H>::final() {
  std::vector<byte> out(impl.DigestSize(), 0);
  impl.Final(out.data());
  return out;
}

template <typename H> std::string CryptoPPHMAC<H>::name() const {
  return impl.AlgorithmName() + " key: " + toString(K);
}

template <typename H> std::size_t CryptoPPHMAC<H>::output_length() const {
  return impl.DigestSize();
}

template <typename H>
void CryptoPPHMAC<H>::update(const byte in[], const size_t length) {
  impl.Update(in, length);
}

template <typename H>
void CryptoPPHMAC<H>::update(const std::vector<byte> &in) {
  update(in.data(), in.size());
}

template <typename H> void CryptoPPHMAC<H>::update(const std::string &in) {
  // reinterpret needed to cast from char* to byte* since byte is explicitly
  // unsigned
  update(reinterpret_cast<const byte *>(in.data()), in.length());
}

template <typename H> HashFunction *CryptoPPHMAC<H>::clone() const {
  return new CryptoPPHMAC<H>(*this);
}
}

#include <botan/blake2b.h>

// Botan Hash wrapper
namespace {
// Templated wrapper around a hash function from the Crypto++ library
template <typename H> class BotanHash : public hash::IncrementalHashFunction {
public:
  // Forward all ctor args to impl
  template <typename... Args>
  BotanHash(Args &&... args) : impl(std::forward<Args>(args)...) {}
  ~BotanHash() = default;
  BotanHash(const BotanHash<H> &other) = default;
  BotanHash(BotanHash<H> &&other) = default;
  BotanHash<H> &operator=(const BotanHash<H> &other) = default;
  BotanHash<H> &operator=(BotanHash<H> &&other) = default;

  std::vector<byte> final() override;
  std::string name() const override;
  std::size_t output_length() const override;
  void update(const byte in[], const std::size_t length) override;
  void update(const std::vector<byte> &in) override;
  void update(const std::string &in) override;
  HashFunction *clone() const override;

private:
  // PIMPL of actual hash
  H impl;
};

template <typename H> std::vector<byte> BotanHash<H>::final() {
  std::vector<byte> out(impl.output_length(), 0);
  impl.final(out.data());
  return out;
}

template <typename H> std::string BotanHash<H>::name() const {
  return impl.name();
}

template <typename H> std::size_t BotanHash<H>::output_length() const {
  return impl.output_length();
}

template <typename H>
void BotanHash<H>::update(const byte in[], const size_t length) {
  impl.update(in, length);
}

template <typename H> void BotanHash<H>::update(const std::vector<byte> &in) {
  update(in.data(), in.size());
}

template <typename H> void BotanHash<H>::update(const std::string &in) {
  impl.update(in);
}

template <typename H> HashFunction *BotanHash<H>::clone() const {
  return new BotanHash<H>(*this);
}
}

std::unique_ptr<hash::IncrementalHashFunction>
hash::getHash(const Hashes hash) {
  switch (hash) {
  case BLAKE2B_224:
    return std::make_unique<BotanHash<Botan::Blake2b> >(224u);
  case BLAKE2B_256:
    return std::make_unique<BotanHash<Botan::Blake2b> >(256u);
  case BLAKE2B_384:
    return std::make_unique<BotanHash<Botan::Blake2b> >(384u);
  case BLAKE2B_512:
    return std::make_unique<BotanHash<Botan::Blake2b> >(512u);
  case MD5:
    return std::make_unique<CryptoPPHash<CryptoPP::Weak::MD5> >();
  case SHA1:
    return std::make_unique<CryptoPPHash<CryptoPP::SHA1> >();
  case SHA_224:
    return std::make_unique<CryptoPPHash<CryptoPP::SHA224> >();
  case SHA_256:
    return std::make_unique<CryptoPPHash<CryptoPP::SHA256> >();
  case SHA_384:
    return std::make_unique<CryptoPPHash<CryptoPP::SHA384> >();
  case SHA_512:
    return std::make_unique<CryptoPPHash<CryptoPP::SHA512> >();
  case SHA3_224:
    return std::make_unique<CryptoPPHash<CryptoPP::SHA3_224> >();
  case SHA3_256:
    return std::make_unique<CryptoPPHash<CryptoPP::SHA3_256> >();
  case SHA3_384:
    return std::make_unique<CryptoPPHash<CryptoPP::SHA3_384> >();
  case SHA3_512:
    return std::make_unique<CryptoPPHash<CryptoPP::SHA3_512> >();
  }
  assert(0 && "Unknown hash");
  return nullptr;
}

std::unique_ptr<hash::IncrementalHashFunction>
hash::getHMACHash(const Hashes hash, const vector<byte> &Key) {
  switch (hash) {
  case BLAKE2B_224:
  case BLAKE2B_256:
  case BLAKE2B_384:
  case BLAKE2B_512:
    assert(0 && "BLAKE2B Not Supported for HMAC yet");
  case MD5:
    return std::make_unique<CryptoPPHMAC<CryptoPP::Weak::MD5> >(Key);
  case SHA1:
    return std::make_unique<CryptoPPHMAC<CryptoPP::SHA1> >(Key);
  case SHA_224:
    return std::make_unique<CryptoPPHMAC<CryptoPP::SHA224> >(Key);
  case SHA_256:
    return std::make_unique<CryptoPPHMAC<CryptoPP::SHA256> >(Key);
  case SHA_384:
    return std::make_unique<CryptoPPHMAC<CryptoPP::SHA384> >(Key);
  case SHA_512:
    return std::make_unique<CryptoPPHMAC<CryptoPP::SHA512> >(Key);
  case SHA3_224:
    //return make_unique<CryptoPPHMAC<CryptoPP::SHA3_224> >(Key);
  case SHA3_256:
    //return make_unique<CryptoPPHMAC<CryptoPP::SHA3_256> >(Key);
  case SHA3_384:
    //return make_unique<CryptoPPHMAC<CryptoPP::SHA3_384> >(Key);
  case SHA3_512:
    //return make_unique<CryptoPPHMAC<CryptoPP::SHA3_512> >(Key);
    assert(0 && "SHA3 Not Supported for HMAC yet");
  }
  assert(0 && "Unknown hash for HMAC");
  return nullptr;
}
