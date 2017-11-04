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

TEST(MD5, RFC1321) {
  auto md5 = hash::getHash(hash::MD5);

  EXPECT_EQ(md5->name(), "MD5");

  testHash(md5, "", "d41d8cd98f00b204e9800998ecf8427e");
  testHash(md5, "a", "0cc175b9c0f1b6a831c399e269772661");
  testHash(md5, "abc", "900150983cd24fb0d6963f7d28e17f72");
  testHash(md5, "message digest", "f96b697d7cb7938d525a2f31aaf161d0");
  testHash(md5, "abcdefghijklmnopqrstuvwxyz",
           "c3fcd3d76192e4007dfb496cca67e13b");
  testHash(md5,
           "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
           "d174ab98d277d9f5a5611c2c9f419d9f");
  testHash(md5, "123456789012345678901234567890123456789012345678901"
                "23456789012345678901234567890",
           "57edf4a22be3c955ac49da2e2107b67a");
}
