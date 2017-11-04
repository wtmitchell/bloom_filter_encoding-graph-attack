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
using test::hash::testHashBytes;

// Source of these test vectors is
// cryptopp564.zip/TestVectors/sha3_*_fips_202.txt

TEST(SHA3_224, CryptoPPTestVectors) {
  auto sha3 = hash::getHash(hash::SHA3_224);

  EXPECT_EQ(sha3->name(), "SHA3-224");

  testHashBytes(sha3, "",
                "6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7");
  testHashBytes(sha3, "01",
                "488286d9d32716e5881ea1ee51f36d3660d70f0db03b3f612ce9eda4");
  testHashBytes(sha3, "69cb",
                "94bd25c4cf6ca889126df37ddd9c36e6a9b28a4fe15cc3da6debcdd7");
  testHashBytes(sha3, "bf5831",
                "1bb36bebde5f3cb6d8e4672acf6eec8728f31a54dacc2560da2a00cc");
  testHashBytes(sha3, "d148ce6d",
                "0b521dac1efe292e20dfb585c8bff481899df72d59983315958391ba");
  testHashBytes(sha3, "91c71068f8",
                "989f017709f50bd0230623c417f3daf194507f7b90a11127ba1638fa");
  testHashBytes(sha3, "e7183e4d89c9",
                "650618f3b945c07de85b8478d69609647d5e2a432c6b15fbb3db91e4");
  testHashBytes(sha3, "d85e470a7c6988",
                "8a134c33c7abd673cd3d0c33956700760de980c5aee74c96e6ba08b2");
}

TEST(SHA3_256, CryptoPPTestVectors) {
  auto sha3 = hash::getHash(hash::SHA3_256);

  EXPECT_EQ(sha3->name(), "SHA3-256");

  testHashBytes(
      sha3, "",
      "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a");
  testHashBytes(
      sha3, "e9",
      "f0d04dd1e6cfc29a4460d521796852f25d9ef8d28b44ee91ff5b759d72c1e6d6");
  testHashBytes(
      sha3, "d477",
      "94279e8f5ccdf6e17f292b59698ab4e614dfe696a46c46da78305fc6a3146ab7");
  testHashBytes(
      sha3, "b053fa",
      "9d0ff086cd0ec06a682c51c094dc73abdc492004292344bd41b82a60498ccfdb");
  testHashBytes(
      sha3, "e7372105",
      "3a42b68ab079f28c4ca3c752296f279006c4fe78b1eb79d989777f051e4046ae");
  testHashBytes(
      sha3, "0296f2c40a",
      "53a018937221081d09ed0497377e32a1fa724025dfdc1871fa503d545df4b40d");
  testHashBytes(
      sha3, "e6fd42037f80",
      "2294f8d3834f24aa9037c431f8c233a66a57b23fa3de10530bbb6911f6e1850f");
  testHashBytes(
      sha3, "37b442385e0538",
      "cfa55031e716bbd7a83f2157513099e229a88891bb899d9ccd317191819998f8");
}

TEST(SHA3_384, CryptoPPTestVectors) {
  auto sha3 = hash::getHash(hash::SHA3_384);

  EXPECT_EQ(sha3->name(), "SHA3-384");

  testHashBytes(sha3, "", "0c63a75b845e4f7d01107d852e4c2485c51a50aaaa94fc61995e"
                          "71bbee983a2ac3713831264adb47fb6bd1e058d5f004");
  testHashBytes(sha3, "80", "7541384852e10ff10d5fb6a7213a4a6c15ccc86d8bc1068ac0"
                            "4f69277142944f4ee50d91fdc56553db06b2f5039c8ab7");
  testHashBytes(sha3, "fb52", "d73a9d0e7f1802352ea54f3e062d3910577bf87edda48101"
                              "de92a3de957e698b836085f5f10cab1de19fd0c906e4838"
                              "5");
  testHashBytes(sha3, "6ab7d6", "ea12d6d32d69ad2154a57e0e1be481a45add739ee7dd6e"
                                "2a27e544b6c8b5ad122654bbf95134d567987156295d5e"
                                "57db");
  testHashBytes(sha3, "11587dcb", "cb6e6ce4a266d438ddd52867f2e183021be50223c7d5"
                                  "7f8fdcaa18093a9d0126607df026c025bff40bc314af"
                                  "43fd8a08");
  testHashBytes(sha3, "4d7fc6cae6", "e570d463a010c71b78acd7f9790c78ce946e00cc54"
                                    "dae82bfc3833a10f0d8d35b03cbb4aa2f9ba4b2749"
                                    "8807a397cd47");
  testHashBytes(sha3, "5a6659e9f0e7", "21b1f3f63b907f968821185a7fe30b16d47e1d6e"
                                      "e5b9c80be68947854de7a8ef4a03a6b2e4ec96ab"
                                      "dd4fa29ab9796f28");
  testHashBytes(sha3, "17510eca2fe11b", "35fba6958b6c68eae8f2b5f5bdf5ebcc565252"
                                        "bc70f983548c2dfd5406f111a0a95b1bb9a639"
                                        "988c8d65da912d2c3ea2");
}

TEST(SHA3_512, CryptoPPTestVectors) {
  auto sha3 = hash::getHash(hash::SHA3_512);

  EXPECT_EQ(sha3->name(), "SHA3-512");

  testHashBytes(sha3, "", "a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1"
                          "dcc1475c80a615b2123af1f5f94c11e3e9402c3ac558f500199d"
                          "95b6d3e301758586281dcd26");
  testHashBytes(sha3, "e5", "150240baf95fb36f8ccb87a19a41767e7aed95125075a2b2db"
                            "ba6e565e1ce8575f2b042b62e29a04e9440314a821c6224182"
                            "964d8b557b16a492b3806f4c39c1");
  testHashBytes(sha3, "ef26", "809b4124d2b174731db14585c253194c8619a68294c8c489"
                              "47879316fef249b1575da81ab72aad8fae08d24ece75ca1b"
                              "e46d0634143705d79d2f5177856a0437");
  testHashBytes(sha3, "37d518", "4aa96b1547e6402c0eee781acaa660797efe26ec00b4f2"
                                "e0aec4a6d10688dd64cbd7f12b3b6c7f802e2096c04120"
                                "8b9289aec380d1a748fdfcd4128553d781e3");
  testHashBytes(sha3, "fc7b8cda", "58a5422d6b15eb1f223ebe4f4a5281bc6824d1599d97"
                                  "9f4c6fe45695ca89014260b859a2d46ebf75f51ff204"
                                  "927932c79270dd7aef975657bb48fe09d8ea008e");
  testHashBytes(sha3, "4775c86b1c", "ce96da8bcd6bc9d81419f0dd3308e3ef541bc7b030"
                                    "eee1339cf8b3c4e8420cd303180f8da77037c8c1ae"
                                    "375cab81ee475710923b9519adbddedb36db0c199f"
                                    "70");
  testHashBytes(sha3, "71a986d2f662", "def6aac2b08c98d56a0501a8cb93f5b47d6322da"
                                      "f99e03255457c303326395f765576930f8571d89"
                                      "c01e727cc79c2d4497f85c45691b554e20da810c"
                                      "2bc865ef");
  testHashBytes(sha3, "ec83d707a1414a", "84fd3775bac5b87e550d03ec6fe4905cc60e85"
                                        "1a4c33a61858d4e7d8a34d471f05008b9a1d63"
                                        "044445df5a9fce958cb012a6ac778ecf45104b"
                                        "0fcb979aa4692d");
}
