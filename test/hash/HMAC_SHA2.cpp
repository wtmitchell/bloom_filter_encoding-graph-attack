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

TEST(HMAC_SHA224, RFC6234) {
  // 1
  auto key = toByteVector("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
  auto sha = hash::getHMACHash(hash::SHA_224, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-224) key: 0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B"
	    "0B0B0B");

  testHash(sha, "Hi There",
           "896fb1128abbdf196832107cd49df33f47b4b1169912ba4f53684b22");

  // 2
  key = toByteVector("4a656665"); // "Jefe"
  sha = hash::getHMACHash(hash::SHA_224, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-224) key: 4A656665");

  testHash(sha, "what do ya want for nothing?",
           "a30e01098bc6dbbf45690f3a7e9e6d0f8bbea2a39e6148008fd05e44");

  // 3
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_224, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-224) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAA");

  testHash(sha, "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd",
           "7fb3cb3588c6c1f6ffa9694d7d6ad2649365b0c1f65d69d1ec8333ea");

  // 4
  key = toByteVector("0102030405060708090a0b0c0d0e0f10111213141516171819");
  sha = hash::getHMACHash(hash::SHA_224, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-224) key: 0102030405060708090A0B0C0D0E0F1011"
	    "1213141516171819");

  testHash(sha, "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd",
           "6c11506874013cac6a2abc1bb382627cec6a90d86efc012de7afec5a");

  // 5
  /* FIXME: Fails because it truncates the hash for some reason
  key = toByteVector("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c");
  sha = hash::getHMACHash(hash::SHA_224, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-224)");

  testHash(sha, "Test With Truncation", "0e2aea68a90c8d37c988bcdb9fca6fa8");
  //*/

  // 6
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_224, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-224) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

  testHash(sha, "Test Using Larger Than Block-Size Key - Hash Key First",
           "95e9a0db962095adaebe9b2d6f0dbce2d499f112f2d2b7273fa6870e");

  // 7
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_224, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-224) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

  testHash(sha, "\x54\x68\x69\x73\x20\x69\x73\x20\x61\x20\x74\x65\x73\x74\x20"
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
           "3a854166ac5d9f023f54d517d0b39dbd946770db9c2b95c9f6f565d1");
}

TEST(HMAC_SHA256, RFC6234) {
  // 1
  auto key = toByteVector("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
  auto sha = hash::getHMACHash(hash::SHA_256, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-256) key: 0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B"
	    "0B0B0B");

  testHash(sha, "Hi There",
           "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32"
           "cff7");

  // 2
  key = toByteVector("4a656665"); // "Jefe"
  sha = hash::getHMACHash(hash::SHA_256, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-256) key: 4A656665");

  testHash(sha, "what do ya want for nothing?",
           "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec"
           "3843");

  // 3
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_256, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-256) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAA");

  testHash(sha, "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd",
           "773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced5"
           "65fe");

  // 4
  key = toByteVector("0102030405060708090a0b0c0d0e0f10111213141516171819");
  sha = hash::getHMACHash(hash::SHA_256, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-256) key: 0102030405060708090A0B0C0D0E0F1011"
	    "1213141516171819");

  testHash(sha, "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd",
           "82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729"
           "665b");

  // 5
  /* FIXME: Fails because it truncates the hash for some reason
  key = toByteVector("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c");
  sha = hash::getHMACHash(hash::SHA_256, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-256)");

  testHash(sha, "Test With Truncation", "a3b6167473100ee06e0c796c2955552b");
  //*/

  // 6
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_256, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-256) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

  testHash(sha, "Test Using Larger Than Block-Size Key - Hash Key First",
           "60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee3"
           "7f54");

  // 7
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_256, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-256) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

  testHash(sha, "\x54\x68\x69\x73\x20\x69\x73\x20\x61\x20\x74\x65\x73\x74\x20"
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
           "9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a"
           "35e2");
}

TEST(HMAC_SHA384, RFC6234) {
  // 1
  auto key = toByteVector("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
  auto sha = hash::getHMACHash(hash::SHA_384, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-384) key: 0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B"
	    "0B0B0B");

  testHash(sha, "Hi There",
           "afd03944d84895626b0825f4ab46907f15f9dadbe4101ec682aa034c7ceb"
           "c59cfaea9ea9076ede7f4af152e8b2fa9cb6");

  // 2
  key = toByteVector("4a656665"); // "Jefe"
  sha = hash::getHMACHash(hash::SHA_384, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-384) key: 4A656665");

  testHash(sha, "what do ya want for nothing?",
           "af45d2e376484031617f78d2b58a6b1b9c7ef464f5a01b47e42ec3736322"
           "445e8e2240ca5e69e2c78b3239ecfab21649");

  // 3
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_384, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-384) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAA");

  testHash(sha, "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd",
           "88062608d3e6ad8a0aa2ace014c8a86f0aa635d947ac9febe83ef4e55966"
           "144b2a5ab39dc13814b94e3ab6e101a34f27");

  // 4
  key = toByteVector("0102030405060708090a0b0c0d0e0f10111213141516171819");
  sha = hash::getHMACHash(hash::SHA_384, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-384) key: 0102030405060708090A0B0C0D0E0F1011"
	    "1213141516171819");

  testHash(sha, "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd",
           "3e8a69b7783c25851933ab6290af6ca77a9981480850009cc5577c6e1f57"
           "3b4e6801dd23c4a7d679ccf8a386c674cffb");

  // 5
  /* FIXME: Fails because it truncates the hash for some reason
  key = toByteVector("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c");
  sha = hash::getHMACHash(hash::SHA_384, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-384)");

  testHash(sha, "Test With Truncation", "3abf34c3503b2a23a46efc619baef897");
  //*/

  // 6
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_384, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-384) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

  testHash(sha, "Test Using Larger Than Block-Size Key - Hash Key First",
           "4ece084485813e9088d2c63a041bc5b44f9ef1012a2b588f3cd11f05033a"
           "c4c60c2ef6ab4030fe8296248df163f44952");

  // 7
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_384, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-384) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

  testHash(sha, "\x54\x68\x69\x73\x20\x69\x73\x20\x61\x20\x74\x65\x73\x74\x20"
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
           "6617178e941f020d351e2f254e8fd32c602420feb0b8fb9adccebb82461e"
           "99c5a678cc31e799176d3860e6110c46523e");
}

TEST(HMAC_SHA512, RFC6234) {
  // 1
  auto key = toByteVector("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
  auto sha = hash::getHMACHash(hash::SHA_512, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-512) key: 0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B"
	    "0B0B0B");

  testHash(sha, "Hi There",
           "87aa7cdea5ef619d4ff0b4241a1d6cb02379f4e2ce4ec2787ad0b30545e1"
           "7cdedaa833b7d6b8a702038b274eaea3f4e4be9d914eeb61f1702e696c20"
           "3a126854");

  // 2
  key = toByteVector("4a656665"); // "Jefe"
  sha = hash::getHMACHash(hash::SHA_512, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-512) key: 4A656665");

  testHash(sha, "what do ya want for nothing?",
           "164b7a7bfcf819e2e395fbe73b56e0a387bd64222e831fd610270cd7ea25"
           "05549758bf75c05a994a6d034f65f8f0e6fdcaeab1a34d4a6b4b636e070a"
           "38bce737");

  // 3
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_512, key);

  EXPECT_EQ(sha->name(), "HMAC(SHA-512) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAA");

  testHash(sha, "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd",
           "fa73b0089d56a284efb0f0756c890be9b1b5dbdd8ee81a3655f83e33b227"
           "9d39bf3e848279a722c806b485a47e67c807b946a337bee8942674278859"
           "e13292fb");

  // 4
  key = toByteVector("0102030405060708090a0b0c0d0e0f10111213141516171819");
  sha = hash::getHMACHash(hash::SHA_512, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-512) key: 0102030405060708090A0B0C0D0E0F1011"
	    "1213141516171819");

  testHash(sha, "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd",
           "b0ba465637458c6990e5a8c5f61d4af7e576d97ff94b872de76f8050361e"
           "e3dba91ca5c11aa25eb4d679275cc5788063a5f19741120c4f2de2adebeb"
           "10a298dd");

  // 5
  /* FIXME: Fails because it truncates the hash for some reason
  key = toByteVector("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c");
  sha = hash::getHMACHash(hash::SHA_512, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-512)");

  testHash(sha, "Test With Truncation", "415fad6271580a531d4179bc891d87a6");
  //*/

  // 6
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_512, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-512) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

  testHash(sha, "Test Using Larger Than Block-Size Key - Hash Key First",
           "80b24263c7c1a3ebb71493c1dd7be8b49b46d1f41b4aeec1121b013783f8"
           "f3526b56d037e05f2598bd0fd2215d6a1e5295e64f73f63f0aec8b915a98"
           "5d786598");

  // 7
  key = toByteVector("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  sha = hash::getHMACHash(hash::SHA_512, key);
  EXPECT_EQ(sha->name(), "HMAC(SHA-512) key: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

  testHash(sha, "\x54\x68\x69\x73\x20\x69\x73\x20\x61\x20\x74\x65\x73\x74\x20"
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
           "e37b6a775dc87dbaa4dfa9f96e5e3ffddebd71f8867289865df5a32d20cd"
           "c944b6022cac3c4982b10d5eeb55c3e4de15134676fb6de0446065c97440"
           "fa8c6a58");
}
