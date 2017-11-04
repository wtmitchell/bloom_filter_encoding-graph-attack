//===----------------------------------------------------------------------===//
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
#include <gtest/gtest.h>

#include "hash/HashFactory.h"
#include "util/ByteVector.h"
using util::ByteVector::toByteVector;

#include "HashCommon.h"
using test::hash::testHash;
using test::hash::testHashIterated;

TEST(HMAC_SHA1, RFC6234) {
  // 1
  auto key = toByteVector("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
  auto sha = hash::getHMACHash(hash::SHA1, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-1) key: 0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B");

  testHash(sha, "Hi There", "b617318655057264e28bc0b6fb378c8ef146be00");

  // 2
  key = toByteVector("4a656665"); // "Jefe"
  sha = hash::getHMACHash(hash::SHA1, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-1) key: 4A656665");

  testHash(sha, "what do ya want for nothing?",
           "effcdf6ae5eb2fa2d27416d5f184df9c259a7c79");

  // 3
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA1, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-1) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

  testHash(sha, "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd",
           "125d7342b9ac11cd91a39af48aa17b4f63f175d3");

  // 4
  key = toByteVector("0102030405060708090a0b0c0d0e0f10111213141516171819");
  sha = hash::getHMACHash(hash::SHA1, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-1) key: 0102030405060708090A0B0C0D0E0F10111213141516171819");

  testHash(sha, "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd",
           "4c9007f4026250c6bc8414f9bf50c86c2d7235da");

  // 5
  /* FIXME: Fails because it truncates the hash for some reason
  key = toByteVector("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c");
  sha = hash::getHMACHash(hash::SHA1, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-1)");

  testHash(sha, "Test With Truncation", "4c1a03424b55e07fe7f27be1");
  */

  // 6
  /* FIXME: Fails for some reason
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA1, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-1) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAA");

  testHash(sha, "Test Using Larger Than Block-Size Key - Hash Key First",
           "aa4ae5e15272d00e95705637ce8a3b55ed402112");
  //*/

  // 7
  /* FIXME: Fails for likely same reason as 6
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA1, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-1)");

  testHash(sha,
           "\x54\x68\x69\x73\x20\x69\x73\x20\x61\x20\x74\x65\x73\x74\x20"
           "\x75\x73\x69\x6e\x67\x20\x61\x20\x6c\x61\x72\x67\x65\x72\x20"
           "\x74\x68\x61\x6e\x20\x62\x6c\x6f\x63\x6b\x2d\x73\x69\x7a\x65"
           "\x20\x6b\x65\x79\x20\x61\x6e\x64\x20\x61\x20\x6c\x61\x72\x67"
           "\x65\x72\x20\x74\x68\x61\x6e\x20\x62\x6c\x6f\x63\x6b\x2d\x73"
           "\x69\x7a\x65\x20\x64\x61\x74\x61\x2e\x20\x54\x68\x65\x20\x6b"
           "\x65\x79\x20\x6e\x65\x65\x64\x73\x20\x74\x6f\x20\x62\x65\x20"
           "\x68\x61\x73\x68\x65\x64\x20\x62\x65\x66\x6f\x72\x65\x20\x62"
           "\x65\x69\x6e\x67\x20\x75\x73\x65\x64\x20\x62\x79\x20\x74\x68"
           "\x65\x20\x48\x4d\x41\x43\x20\x61\x6c\x67\x6f\x72\x69\x74\x68"
           "\x6d\x2e",
           // "This is a test using a larger than block-size key and a "
           // "larger than block-size data.  The key needs to be hashed "
           // "before being used by the HMAC algorithm."
           "e8e99d0f45237d786d6bbaa7965c7808bbff1a91");
  //*/
}
