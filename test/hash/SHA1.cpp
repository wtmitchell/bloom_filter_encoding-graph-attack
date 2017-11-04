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

#include "HashCommon.h"
using test::hash::testHash;
using test::hash::testHashIterated;

TEST(SHA1, RFC3174) {
  auto sha = hash::getHash(hash::SHA1);

  EXPECT_EQ(sha->name(), "SHA-1");

  testHash(sha, "abc", "a9993e364706816aba3e25717850c26c9cd0d89d");
  testHash(sha, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
           "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
  testHashIterated(sha, "a", 1000000,
                   "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
  testHashIterated(
      sha, "0123456701234567012345670123456701234567012345670123456701234567",
      10, "dea356a2cddd90c7a7ecedc5ebb563934f460452");
}

TEST(SHA1, PartialRFC6234) {
  auto sha = hash::getHash(hash::SHA1);

  EXPECT_EQ(sha->name(), "SHA-1");

  // 1
  testHash(sha, "abc", "a9993e364706816aba3e25717850c26c9cd0d89d");
  // 2
  testHash(sha, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
           "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
  // 3
  testHashIterated(sha, "a", 1000000,
                   "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
  // 4
  testHashIterated(
      sha, "0123456701234567012345670123456701234567012345670123456701234567",
      10, "dea356a2cddd90c7a7ecedc5ebb563934f460452");
  // 5
  /* Skipped due to extra bytes
    testHash(sha, "", "29826b003b906e660eff4027ce98af3531ac75ba");
    { "", 0, 0, 0x98, 5,
    "29826B003B906E660EFF4027CE98AF3531AC75BA" },
  */
  // 6
  testHash(sha, "\x5e", "5e6f80a34a9798cafc6a5db96cc57ba4c4db59c2");
  // 7
  /* Skipped due to extra bytes
    #define TEST7_1                                                 \
    "\x49\xb2\xae\xc2\x59\x4b\xbe\x3a\x3b\x11\x75\x42\xd9\x4a\xc8"
    { TEST7_1, length(TEST7_1), 1, 0x80, 3,
    "6239781E03729919C01955B3FFA8ACB60B988340" },
  */
  // 8
  testHash(sha,
           "\x9a\x7d\xfd\xf1\xec\xea\xd0\x6e\xd6\x46\xaa\x55\xfe\x75\x71\x46",
           "82abff6605dbe1c17def12a394fa22a82b544a35");
  // 9
  /* Skipped due to extra bytes
    #define TEST9_1                                                  \
    "\x65\xf9\x32\x99\x5b\xa4\xce\x2c\xb1\xb4\xa2\xe7\x1a\xe7\x02\x20"  \
    "\xaa\xce\xc8\x96\x2d\xd4\x49\x9c\xbd\x7c\x88\x7a\x94\xea\xaa\x10"  \
    "\x1e\xa5\xaa\xbc\x52\x9b\x4e\x7e\x43\x66\x5a\x5a\xf2\xcd\x03\xfe"  \
    "\x67\x8e\xa6\xa5\x00\x5b\xba\x3b\x08\x22\x04\xc2\x8b\x91\x09\xf4"  \
    "\x69\xda\xc9\x2a\xaa\xb3\xaa\x7c\x11\xa1\xb3\x2a"
    // 9
    { TEST9_1, length(TEST9_1), 1, 0xE0, 3,
    "8C5B2A5DDAE5A97FC7F9D85661C672ADBF7933D4" },
  */
  // 10
  /* Not working. Reason unknown
    testHash(sha,
    "\xf7\x8f\x92\x14\x1b\xcd\x17\x0a\xe8\x9b\x4f\xba\x15\xa1\xd5\x9f"
    "\x3f\xd8\x4d\x22\x3c\x92\x51\xbd\xac\xbb\xae\x61\xd0\x5e\xd1\x15"
    "\xa0\x6a\x7c\xe1\x17\xb7\xbe\xea\xd2\x44\x21\xde\xd9\xc3\x25\x92"
    "\xbd\x57\xed\xea\xe3\x9c\x39\xfa\x1f\xe8\x94\x6a\x84\xd0\xcf\x1f"
    "\x7b\xee\xad\x17\x13\xe2\xe0\x95\x98\x97\x34\x7f\x67\xc8\x0b\x04"
    "\x00\xc2\x09\x81\x5d\x6b\x10\xa6\x83\x83\x6f\xd5\x56\x2a\x56\xca"
    "\xb1\xa2\x8e\x81\xb6\x57\x66\x54\x63\x1c\xf1\x65\x66\xb8\x6e\x3b"
    "\x33\xa1\x08\xb0\x53\x07\xc0\x0a\xff\x14\xa7\x68\xed\x73\x50\x60"
    "\x6a\x0f\x85\xe6\xa9\x1d\x39\x6f\x5b\x5c\xbe\x57\x7f\x9b\x38\x80"
    "\x7c\x7d\x52\x3d\x6d\x79\x2f\x6e\xbc\x24\xa4\xec\xf2\xb3\xa4\x27"
    "\xcd\xbb\xfb",
           "cb0082c8f197d260991ba6a460e76e202bad27b3");
  */
}
