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
///
/// \file
/// \brief This file contains unit testing BitTuple
///
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

#include "adt/BitTuple.h"

using namespace adt;

TEST(BitTuple, TrivialOperationAllSmall) {
  BitTuple<> x;

  EXPECT_EQ(0, x.size());
  EXPECT_TRUE(x.empty());

  x.resize(5);
  EXPECT_EQ(5, x.size());
  EXPECT_FALSE(x.empty());

  for (size_t i = 0; i < 5; ++i) {
    EXPECT_FALSE(x[i]);
    EXPECT_FALSE(x.test(i));
  }

  x.resize(3, true);
  EXPECT_EQ(3, x.size());
  EXPECT_FALSE(x.empty());

  for (size_t i = 0; i < 3; ++i) {
    EXPECT_FALSE(x[i]);
    EXPECT_FALSE(x.test(i));
  }

  x.resize(5, true);
  EXPECT_EQ(5, x.size());
  EXPECT_FALSE(x.empty());

  for (size_t i = 0; i < 3; ++i) {
    EXPECT_FALSE(x[i]);
    EXPECT_FALSE(x.test(i));
  }
  for (size_t i = 3; i < 5; ++i) {
    EXPECT_TRUE(x[i]);
    EXPECT_TRUE(x.test(i));
  }

  x[0] = true;
  for (size_t i = 0; i < 1; ++i) {
    EXPECT_TRUE(x[i]);
    EXPECT_TRUE(x.test(i));
  }
  for (size_t i = 1; i < 3; ++i) {
    EXPECT_FALSE(x[i]);
    EXPECT_FALSE(x.test(i));
  }
  for (size_t i = 3; i < 5; ++i) {
    EXPECT_TRUE(x[i]);
    EXPECT_TRUE(x.test(i));
  }
  x[1] = true;
  for (size_t i = 0; i < 2; ++i) {
    EXPECT_TRUE(x[i]);
    EXPECT_TRUE(x.test(i));
  }
  for (size_t i = 2; i < 3; ++i) {
    EXPECT_FALSE(x[i]);
    EXPECT_FALSE(x.test(i));
  }
  for (size_t i = 3; i < 5; ++i) {
    EXPECT_TRUE(x[i]);
    EXPECT_TRUE(x.test(i));
  }

  x[2] = true;
  for (size_t i = 0; i < 5; ++i) {
    EXPECT_TRUE(x[i]);
    EXPECT_TRUE(x.test(i));
  }

  x[0] = x[1] = x[2] = false;
  for (size_t i = 0; i < 3; ++i) {
    EXPECT_FALSE(x[i]);
    EXPECT_FALSE(x.test(i));
  }
  for (size_t i = 3; i < 5; ++i) {
    EXPECT_TRUE(x[i]);
    EXPECT_TRUE(x.test(i));
  }
  x.reset(3);
  x.reset(4);
  for (size_t i = 0; i < 5; ++i) {
    EXPECT_FALSE(x[i]);
    EXPECT_FALSE(x.test(i));
  }
  for (size_t i = 0; i < 5; ++i) {
    x.set(i);
    EXPECT_TRUE(x[i]);
  }

  // copy ctor
  BitTuple<> y(x);
  EXPECT_EQ(5, y.size());
  EXPECT_FALSE(y.empty());

  for (size_t i = 0; i < 5; ++i) {
    EXPECT_TRUE(y[i]);
    x.reset(i);
    EXPECT_FALSE(x[i]);
    EXPECT_TRUE(y[i]);
  }

  // move ctor
  BitTuple<> z(std::move(x));
  EXPECT_EQ(5, z.size());
  EXPECT_FALSE(z.empty());
  EXPECT_EQ(0, x.size());
  EXPECT_TRUE(x.empty());

  for (size_t i = 0; i < 5; ++i) {
    EXPECT_FALSE(z[i]);
  }
}

TEST(BitTuple, TrivialOperationAllLarge) {
  BitTuple<> x(100);
  EXPECT_EQ(100, x.size());
  EXPECT_FALSE(x.empty());

  for (size_t i = 0; i < 100; ++i) {
    EXPECT_FALSE(x[i]);
    EXPECT_FALSE(x.test(i));
    x.reset(i);
    EXPECT_FALSE(x[i]);
    EXPECT_FALSE(x.test(i));
  }
  for (size_t i = 0; i < 100; ++i) {
    x.set(i);
    EXPECT_TRUE(x[i]);
    EXPECT_TRUE(x.test(i));
    x.reset(i);
    EXPECT_FALSE(x[i]);
    EXPECT_FALSE(x.test(i));
  }

  // copy ctor
  // move ctor
}

// Test fixture for type parameterized tests
template <typename T>
class BitTupleTest : public ::testing::Test { };

// Test BitTuple with multiple Storage types
typedef ::testing::Types<uint8_t, uint16_t, uint32_t, uint64_t> BitTupleTestTypes;
TYPED_TEST_CASE(BitTupleTest, BitTupleTestTypes);

TYPED_TEST(BitTupleTest, DefaultCtor) {
  BitTuple<TypeParam> x;
  EXPECT_EQ(0, x.size());
  EXPECT_TRUE(x.empty());
}

TYPED_TEST(BitTupleTest, SizedCtorWithInitVal) {
  // Small
  const size_t small = 10;
  BitTuple<TypeParam> x(small, true);
  BitTuple<TypeParam> y(small, false);
  EXPECT_EQ(small, x.size()) << "small = " << small;
  EXPECT_FALSE(x.empty());
  EXPECT_EQ(small, y.size()) << "small = " << small;
  EXPECT_FALSE(y.empty());
  for (size_t i = 0; i < small; ++i) {
    EXPECT_TRUE(x.test(i)) << "i = " << i;
    EXPECT_FALSE(y.test(i)) << "i = " << i;
  }

  // Min size large if uintptr_t is 32 bits
  const size_t large32 = 27;
  BitTuple<TypeParam> a(large32 - 1, true);
  BitTuple<TypeParam> b(large32 - 1, false);
  EXPECT_EQ(large32 - 1, a.size()) << "large32 = " << large32;
  EXPECT_FALSE(a.empty());
  EXPECT_EQ(large32 - 1, b.size()) << "large32 = " << large32;
  EXPECT_FALSE(b.empty());
  for(size_t i = 0; i < large32 - 1; ++i) {
    EXPECT_TRUE(a.test(i)) << "i = " << i;
    EXPECT_FALSE(b.test(i)) << "i = " << i;
  }

  BitTuple<TypeParam> c(large32, true);
  BitTuple<TypeParam> d(large32, false);
  EXPECT_EQ(large32, c.size()) << "large32 = " << large32;
  EXPECT_FALSE(c.empty());
  EXPECT_EQ(large32, d.size()) << "large32 = " << large32;
  EXPECT_FALSE(d.empty());
  for(size_t i = 0; i < large32; ++i) {
    EXPECT_TRUE(c.test(i)) << "i = " << i;
    EXPECT_FALSE(d.test(i)) << "i = " << i;
  }

  // Min size large if uintptr_t is 64 bits
  const size_t large64 = 58;
  BitTuple<TypeParam> e(large64 - 1, true);
  BitTuple<TypeParam> f(large64 - 1, false);
  EXPECT_EQ(large64 - 1, e.size()) << "large64 = " << large64;
  EXPECT_FALSE(e.empty());
  EXPECT_EQ(large64 - 1, f.size()) << "large64 = " << large64;
  EXPECT_FALSE(f.empty());
  for(size_t i = 0; i < large64 - 1; ++i) {
    EXPECT_TRUE(e.test(i)) << "i = " << i;
    EXPECT_FALSE(f.test(i)) << "i = " << i;
  }

  BitTuple<TypeParam> g(large64, true);
  BitTuple<TypeParam> h(large64, false);
  EXPECT_EQ(large64, g.size()) << "large64 = " << large64;
  EXPECT_FALSE(g.empty());
  EXPECT_EQ(large64, h.size()) << "large64 = " << large64;
  EXPECT_FALSE(h.empty());
  for(size_t i = 0; i < large64; ++i) {
    EXPECT_TRUE(g.test(i)) << "i = " << i;
    EXPECT_FALSE(h.test(i)) << "i = " << i;
  }

  // Definitely Large, yet still having a length representable with a uint8_t
  const size_t largeMaxUint8 = 255;
  BitTuple<TypeParam> m(largeMaxUint8, true);
  BitTuple<TypeParam> n(largeMaxUint8, false);
  EXPECT_EQ(largeMaxUint8, m.size()) << "largeMaxUint8 = " << largeMaxUint8;
  EXPECT_FALSE(m.empty());
  EXPECT_EQ(largeMaxUint8, n.size()) << "largeMaxUint8 = " << largeMaxUint8;
  EXPECT_FALSE(n.empty());
  for(size_t i = 0; i < largeMaxUint8; ++i) {
    EXPECT_TRUE(m.test(i)) << "i = " << i;
    EXPECT_FALSE(n.test(i)) << "i = " << i;
  }

}

// copy ctor
TYPED_TEST(BitTupleTest, CopyCtor) {
  // Small
  BitTuple<TypeParam> x(10, true);
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());

  BitTuple<TypeParam> y(x);
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());
  EXPECT_EQ(10, y.size());
  EXPECT_EQ(10, y.count());
  x.reset();
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(0, x.count());
  EXPECT_EQ(10, y.size());
  EXPECT_EQ(10, y.count());

  // Large
  BitTuple<TypeParam> a(200, true);
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(200, a.count());

  BitTuple<TypeParam> b(a);
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(200, a.count());
  EXPECT_EQ(200, b.size());
  EXPECT_EQ(200, b.count());
  a.reset();
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(0, a.count());
  EXPECT_EQ(200, b.size());
  EXPECT_EQ(200, b.count());
}

// move ctor
TYPED_TEST(BitTupleTest, MoveCtor) {
  // Small
  BitTuple<TypeParam> x(10,true);
  EXPECT_EQ(10, x.size());
  BitTuple<TypeParam> y(std::move(x));
  EXPECT_EQ(0, x.size());
  EXPECT_EQ(10, y.size());

  // Large
  x.resize(200, true);

  EXPECT_EQ(200, x.size());
  EXPECT_EQ(200, x.count());

  BitTuple<TypeParam> z(std::move(x));
  EXPECT_EQ(0, x.size());
  EXPECT_EQ(0, x.count());
  EXPECT_EQ(200, z.size());
  EXPECT_EQ(200, z.count());
}

// count
TYPED_TEST(BitTupleTest, Count) {
  BitTuple<TypeParam> x;

  EXPECT_EQ(0, x.count());

  // default to small
  x.resize(10, true);
  EXPECT_EQ(10, x.count());
  EXPECT_EQ(10, x.size());

  // small to small bigger
  x.resize(20, true);
  EXPECT_EQ(20, x.count());
  EXPECT_EQ(20, x.size());

  // small to small smaller
  x.resize(10, true);
  EXPECT_EQ(10, x.count());
  EXPECT_EQ(10, x.size());

  // small to large
  x.resize(200, true);
  EXPECT_EQ(200, x.count());
  EXPECT_EQ(200, x.size());

  // large to large bigger
  x.resize(225, true);
  EXPECT_EQ(225, x.count());
  EXPECT_EQ(225, x.size());

  // large to large smaller
  x.resize(200, true);
  EXPECT_EQ(200, x.count());
  EXPECT_EQ(200, x.size());

  // large to small
  x.resize(10, true);
  EXPECT_EQ(10, x.count());
  EXPECT_EQ(10, x.size());

  // small to large (again)
  x.resize(200, true);
  EXPECT_EQ(200, x.count());
  EXPECT_EQ(200, x.size());

  // large to small (again)
  // 17 used here to force reading from multiple uint8_t to create new small
  // bits
  x.resize(17, true);
  EXPECT_EQ(17, x.count());
  EXPECT_EQ(17, x.size());
}

// any
TYPED_TEST(BitTupleTest, Any) {
  // Small
  BitTuple<TypeParam> x(3, false);

  EXPECT_EQ(3, x.size());

  for (size_t i = 0, e = x.size(); i < e; ++i) {
    EXPECT_FALSE(x.any()) << "i = " << i;
    x.set(i);
    EXPECT_TRUE(x.any()) << "i = " << i;
    x.reset(i);
  }

  // Large
  x.resize(200);
  EXPECT_FALSE(x.any());

  for (size_t i = 0, e = x.size(); i < e; ++i) {
    EXPECT_FALSE(x.any()) << "i = " << i;
    x.set(i);
    EXPECT_TRUE(x.any()) << "i = " << i;
    x.reset(i);
  }
}

// all
TYPED_TEST(BitTupleTest, All) {
    // Small
  BitTuple<TypeParam> x(3, false);

  EXPECT_EQ(3, x.size());

  for (size_t i = 0, e = x.size(); i < e; ++i) {
    EXPECT_FALSE(x.all()) << "i = " << i;
    x.set(i);
  }
  EXPECT_TRUE(x.all());

  // Large
  x.resize(200,true);
  EXPECT_TRUE(x.all());
  x.reset();
  EXPECT_FALSE(x.all());

  for (size_t i = 0, e = x.size(); i < e; ++i) {
    EXPECT_FALSE(x.all()) << "i = " << i;
    x.set(i);
  }
  EXPECT_TRUE(x.all());
}


// none
TYPED_TEST(BitTupleTest, None) {
    // Small
  BitTuple<TypeParam> x(3, false);

  EXPECT_EQ(3, x.size());

  for (size_t i = 0, e = x.size(); i < e; ++i) {
    EXPECT_TRUE(x.none()) << "i = " << i;
    x.set(i);
    EXPECT_FALSE(x.none()) << "i = " << i;
    x.reset(i);
  }

  // Large
  x.resize(200);
  EXPECT_TRUE(x.none());

  for (size_t i = 0, e = x.size(); i < e; ++i) {
    EXPECT_TRUE(x.none()) << "i = " << i;
    x.set(i);
    EXPECT_FALSE(x.none()) << "i = " << i;
    x.reset(i);
  }
}

// clear
TYPED_TEST(BitTupleTest, Clear) {
  // Small
  BitTuple<TypeParam> x(3);

  EXPECT_EQ(3, x.size());
  x.clear();
  EXPECT_EQ(0, x.size());

  // Large
  x.resize(200);
  EXPECT_EQ(200, x.size());
  x.clear();
  EXPECT_EQ(0, x.size());
}

// resize
TYPED_TEST(BitTupleTest, Resize) {
  BitTuple<TypeParam> x(10, true);
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());

  // Small -> Small smaller
  x.resize(5, true);
  EXPECT_EQ(5, x.size());
  EXPECT_EQ(5, x.count());

  // Small -> Small bigger
  x.resize(17, true);
  EXPECT_EQ(17, x.size());
  EXPECT_EQ(17, x.count());

  // Small -> Large
  x.resize(129, true);
  EXPECT_EQ(129, x.size());
  EXPECT_EQ(129, x.count());

  // Large -> Large smaller
  x.resize(128, true);
  EXPECT_EQ(128, x.size());
  EXPECT_EQ(128, x.count());

  // Large -> Large bigger
  x.resize(200, true);
  EXPECT_EQ(200, x.size());
  EXPECT_EQ(200, x.count());

  // Large -> Small
  x.resize(10, true);
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());
}

// set/reset all
TYPED_TEST(BitTupleTest, SetResetAll) {
  BitTuple<TypeParam> x(10, true);
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());
  x.reset();
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(0, x.count());
  x.set();
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());

  x.resize(201, true);
  EXPECT_EQ(201, x.size());
  EXPECT_EQ(201, x.count());
  x.reset();
  EXPECT_EQ(201, x.size());
  EXPECT_EQ(0, x.count());
  x.set();
  EXPECT_EQ(201, x.size());
  EXPECT_EQ(201, x.count());
}

// set/reset specific
TYPED_TEST(BitTupleTest, SetResetSpecific) {
  // Small
  BitTuple<TypeParam> x(3, false);

  for (size_t i = 0; i < 3; ++i) {
    EXPECT_FALSE(x.test(i)) << "i = " << i;
    x.reset(i);
    EXPECT_FALSE(x.test(i)) << "i = " << i;
    x.set(i);
    EXPECT_TRUE(x.test(i)) << "i = " << i;
  }

  // Large
  x.resize(200, true);
  for (size_t i = 0; i < 200; ++i) {
    EXPECT_TRUE(x.test(i)) << "i = " << i;
    x.reset(i);
    EXPECT_FALSE(x.test(i)) << "i = " << i;
    x.set(i);
    EXPECT_TRUE(x.test(i)) << "i = " << i;
  }
}

// set range
TYPED_TEST(BitTupleTest, SetRange) {
  // Small
  BitTuple<TypeParam> x(10, false);

  EXPECT_EQ(10, x.size());
  EXPECT_TRUE(x.none());

  for (size_t i = 0, e = x.size(); i <= e; ++i) {
    x.set(0, i);
    EXPECT_EQ(i, x.count());

    for (size_t j = 0; j < e; ++j) {
      if (j < i)
        EXPECT_TRUE(x.test(j)) << " i = " << i << " j = " << j;
      else
        EXPECT_FALSE(x.test(j)) << " i = " << i << " j = " << j;
    }

    x.reset();
    EXPECT_TRUE(x.none());
  }

  for (size_t i = 0, e = x.size(); i <= e; ++i) {
    x.set(e - i, e);
    EXPECT_EQ(i, x.count());

    for (size_t j = 0; j < e; ++j) {
      if (j < e - i)
        EXPECT_FALSE(x.test(j)) << " i = " << i << " j = " << j;
      else
        EXPECT_TRUE(x.test(j)) << " i = " << i << " j = " << j;
    }

    x.reset();
    EXPECT_TRUE(x.none());
  }

  // Large
  x.clear();
  x.resize(200, false);

  EXPECT_EQ(200, x.size());
  EXPECT_TRUE(x.none());

  for (size_t i = 0, e = x.size(); i <= e; ++i) {
    x.set(0, i);
    EXPECT_EQ(i, x.count());

    for (size_t j = 0; j < e; ++j) {
      if (j < i)
        EXPECT_TRUE(x.test(j)) << " i = " << i << " j = " << j;
      else
        EXPECT_FALSE(x.test(j)) << " i = " << i << " j = " << j;
    }

    x.reset();
    EXPECT_TRUE(x.none());
  }

  for (size_t i = 0, e = x.size(); i <= e; ++i) {
    x.set(e - i, e);
    EXPECT_EQ(i, x.count());

    for (size_t j = 0; j < e; ++j) {
      if (j < e - i)
        EXPECT_FALSE(x.test(j)) << " i = " << i << " j = " << j;
      else
        EXPECT_TRUE(x.test(j)) << " i = " << i << " j = " << j;
    }

    x.reset();
    EXPECT_TRUE(x.none());
  }
}

// reset range
TYPED_TEST(BitTupleTest, ResetRange) {
  // Small
  BitTuple<TypeParam> x(10, true);

  EXPECT_EQ(10, x.size());
  EXPECT_TRUE(x.all());

  for (size_t i = 0, e = x.size(); i <= e; ++i) {
    x.reset(0, i);
    EXPECT_EQ(x.size() - i, x.count());

    for (size_t j = 0; j < e; ++j) {
      if (j < i)
        EXPECT_FALSE(x.test(j)) << " i = " << i << " j = " << j;
      else
        EXPECT_TRUE(x.test(j)) << " i = " << i << " j = " << j;
    }

    x.set();
    EXPECT_TRUE(x.all());
  }

  for (size_t i = 0, e = x.size(); i <= e; ++i) {
    x.reset(e - i, e);
    EXPECT_EQ(x.size() - i, x.count());

    for (size_t j = 0; j < e; ++j) {
      if (j < e - i)
        EXPECT_TRUE(x.test(j)) << " i = " << i << " j = " << j;
      else
        EXPECT_FALSE(x.test(j)) << " i = " << i << " j = " << j;
    }

    x.set();
    EXPECT_TRUE(x.all());
  }

  // Large
  x.clear();
  x.resize(200, true);

  EXPECT_EQ(200, x.size());
  EXPECT_TRUE(x.all());

  for (size_t i = 0, e = x.size(); i <= e; ++i) {
    x.reset(0, i);
    EXPECT_EQ(x.size() - i, x.count());

    for (size_t j = 0; j < e; ++j) {
      if (j < i)
        EXPECT_FALSE(x.test(j)) << " i = " << i << " j = " << j;
      else
        EXPECT_TRUE(x.test(j)) << " i = " << i << " j = " << j;
    }

    x.set();
    EXPECT_TRUE(x.all());
  }

  for (size_t i = 0, e = x.size(); i <= e; ++i) {
    x.reset(e - i, e);
    EXPECT_EQ(x.size() - i, x.count());

    for (size_t j = 0; j < e; ++j) {
      if (j < e - i)
        EXPECT_TRUE(x.test(j)) << " i = " << i << " j = " << j;
      else
        EXPECT_FALSE(x.test(j)) << " i = " << i << " j = " << j;
    }

    x.set();
    EXPECT_TRUE(x.all());
  }
}

// flip all
TYPED_TEST(BitTupleTest, FlipAll) {
  // Small
  BitTuple<TypeParam> x(10, false);

  for (size_t i = 0; i < 10; i += 2)
    x.set(i);

  for (size_t i = 0; i < 10; i += 2)
    EXPECT_TRUE(x.test(i)) << "i = " << i;
  for (size_t i = 1; i < 10; i += 2)
    EXPECT_FALSE(x.test(i)) << "i = " << i;

  x.flip();

  for (size_t i = 0; i < 10; i += 2)
    EXPECT_FALSE(x.test(i)) << "i = " << i;
  for (size_t i = 1; i < 10; i += 2)
    EXPECT_TRUE(x.test(i)) << "i = " << i;

  // Large
  x.resize(200, false);
  x.reset();

  for (size_t i = 0; i < 200; i += 2)
    x.set(i);

  for (size_t i = 0; i < 200; i += 2)
    EXPECT_TRUE(x.test(i)) << "i = " << i;
  for (size_t i = 1; i < 200; i += 2)
    EXPECT_FALSE(x.test(i)) << "i = " << i;

  x.flip();

  for (size_t i = 0; i < 200; i += 2)
    EXPECT_FALSE(x.test(i)) << "i = " << i;
  for (size_t i = 1; i < 200; i += 2)
    EXPECT_TRUE(x.test(i)) << "i = " << i;

}

// flip specific
TYPED_TEST(BitTupleTest, FlipSpecific) {
  // Small
  BitTuple<TypeParam> x(10, false);

  EXPECT_EQ(0, x.count());
  x.flip(5);
  EXPECT_EQ(1, x.count());
  x.flip(5);
  EXPECT_EQ(0, x.count());

  // Large
  x.resize(200, false);
  EXPECT_EQ(0, x.count());
  for (size_t residue = 0; residue < 5; ++residue) {
    for (size_t i = residue; i < 200; i += 5)
      x.flip(i);
    EXPECT_EQ(40 * (residue + 1), x.count());
  }
  EXPECT_EQ(200, x.count());
  for (size_t residue = 0; residue < 5; ++residue) {
    for (size_t i = 4 - residue; i < 200; i += 5)
      x.flip(i);
    EXPECT_EQ(40 * (4 - residue), x.count());
  }
  EXPECT_EQ(0, x.count());
}

// flip copy via operator~
TYPED_TEST(BitTupleTest, FlipOperator) {
  // Small
  BitTuple<TypeParam> x(3, true);
  BitTuple<TypeParam> y = ~x;

  EXPECT_TRUE(x.all());
  EXPECT_TRUE(y.none());

  // Large
  x.resize(200, true);
  y = ~x;

  EXPECT_TRUE(x.all());
  EXPECT_TRUE(y.none());
}

// flip copy via operator~
TYPED_TEST(BitTupleTest, FlipOperatorMove) {
  // Small
  BitTuple<TypeParam> x(3, true);
  EXPECT_TRUE(x.all());
  BitTuple<TypeParam> y = ~std::move(x);
  EXPECT_TRUE(y.none());

  // Large
  BitTuple<TypeParam> z(200, true);
  EXPECT_TRUE(z.all());
  y = ~std::move(z);
  EXPECT_TRUE(y.none());
}

// reference []
TYPED_TEST(BitTupleTest, SubscriptReference) {
  // Small
  BitTuple<TypeParam> x(3, true);

  EXPECT_TRUE(x.test(0));
  EXPECT_TRUE(x.test(1));
  EXPECT_TRUE(x.test(2));
  x[0] = x[1] = x[2] = false;
  EXPECT_FALSE(x.test(0));
  EXPECT_FALSE(x.test(1));
  EXPECT_FALSE(x.test(2));

  // Large
  x.resize(200, false);
  // This loop is likely overkill but want to make sure it won't somehow fail on
  // some boundary of the underlying storage types
  for (size_t i = 1; i < 200; ++i) {
    EXPECT_FALSE(x.test(i)) << "i = " << i;
    x[i-1] = x[i] = true;
  }
  for (size_t i = 0; i < 200; ++i)
    EXPECT_TRUE(x.test(i)) << "i = " << i;
}

// non-ref []
TYPED_TEST(BitTupleTest, SubscriptNonRef) {
  // Small
  BitTuple<TypeParam> x(10, true);
  const BitTuple<TypeParam> &cx = x;
  EXPECT_EQ(10, cx.size());
  EXPECT_EQ(10, cx.count());

  for (size_t i = 0; i < 10; ++i)
    EXPECT_TRUE(cx[i]) << "i = " << i;
  x.reset();
  for (size_t i = 0; i < 10; ++i)
    EXPECT_FALSE(cx[i]) << "i = " << i;

  // Large
  x.clear();
  x.resize(200, true);
  EXPECT_EQ(200, cx.size());
  EXPECT_EQ(200, cx.count());
  for (size_t i = 0; i < 200; ++i)
    EXPECT_TRUE(cx[i]) << "i = " << i;
  x.reset();
  for (size_t i = 0; i < 200; ++i)
    EXPECT_FALSE(cx[i]) << "i = " << i;
}

// test specific
TYPED_TEST(BitTupleTest, TestSpecific) {
  // Small
  BitTuple<TypeParam> x(10, true);
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());
  for (size_t i = 0; i < 10; ++i)
    EXPECT_TRUE(x.test(i)) << "i = " << i;
  x.reset();
  for (size_t i = 0; i < 10; ++i)
    EXPECT_FALSE(x.test(i)) << "i = " << i;

  // Large
  x.clear();
  x.resize(200, true);
  EXPECT_EQ(200, x.size());
  EXPECT_EQ(200, x.count());
  for (size_t i = 0; i < 200; ++i)
    EXPECT_TRUE(x.test(i)) << "i = " << i;
  x.reset();
  for (size_t i = 0; i < 200; ++i)
    EXPECT_FALSE(x.test(i)) << "i = " << i;
}

// op== equality / op!= inequality
TYPED_TEST(BitTupleTest, EqualityInequality) {
  // Small Small
  BitTuple<TypeParam> a(10, true);
  BitTuple<TypeParam> b(10, false);

  EXPECT_TRUE(a == a);
  EXPECT_FALSE(a != a);
  EXPECT_TRUE(b == b);
  EXPECT_FALSE(b != b);

  EXPECT_TRUE(a != b);
  EXPECT_TRUE(b != a);
  EXPECT_FALSE(a == b);
  EXPECT_FALSE(b == a);

  b.set();

  EXPECT_TRUE(a == b);
  EXPECT_TRUE(b == a);
  EXPECT_FALSE(a != b);
  EXPECT_FALSE(b != a);

  // Small Large
  b.resize(200, true);
  EXPECT_TRUE(b == b);
  EXPECT_FALSE(b != b);

  EXPECT_TRUE(a != b);
  EXPECT_TRUE(b != a);
  EXPECT_FALSE(a == b);
  EXPECT_FALSE(b == a);

  // Large Large
  a.resize(200, true);
  EXPECT_TRUE(a == a);
  EXPECT_FALSE(a != a);

  EXPECT_TRUE(a == b);
  EXPECT_TRUE(b == a);
  EXPECT_FALSE(a != b);
  EXPECT_FALSE(b != a);

  a.reset();
  EXPECT_TRUE(a != b);
  EXPECT_TRUE(b != a);
  EXPECT_FALSE(a == b);
  EXPECT_FALSE(b == a);
}

// op&= intersection
TYPED_TEST(BitTupleTest, IntersectionOpAnd) {
  // Small
  BitTuple<TypeParam> a(10, true);
  BitTuple<TypeParam> b(10, false);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  a &= b;

  EXPECT_TRUE(a.none());
  EXPECT_TRUE(b.none());

  a &= b;

  EXPECT_TRUE(a.none());
  EXPECT_TRUE(b.none());

  // Large
  a.set();
  a.resize(200, true);
  b.set();
  b.resize(200, true);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());

  a &= b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());

  b.reset();

  EXPECT_TRUE(b.none());

  a &= b;

  EXPECT_TRUE(a.none());
  EXPECT_TRUE(b.none());
}


// reset(const &) diff
TYPED_TEST(BitTupleTest, DifferenceReset) {
  // Small
  BitTuple<TypeParam> a(10, true);
  BitTuple<TypeParam> b(10, false);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  a.reset(b);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  b.reset(a);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  a.reset(a);

  EXPECT_TRUE(a.none());

  b.reset(b);

  EXPECT_TRUE(b.none());

  // Large
  a.resize(200, true);
  b.resize(200, true);

  EXPECT_EQ(190, a.count());
  EXPECT_EQ(190, b.count());

  a.reset(b);

  EXPECT_TRUE(a.none());

  b.reset(a);

  EXPECT_EQ(190, b.count());

  b.reset(b);

  EXPECT_TRUE(b.none());
}

// op-= diff
TYPED_TEST(BitTupleTest, DifferenceOpMinus) {
  // Small
  BitTuple<TypeParam> a(10, true);
  BitTuple<TypeParam> b(10, false);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  a -= b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  b -= a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  a -= a;

  EXPECT_TRUE(a.none());

  b -= b;

  EXPECT_TRUE(b.none());

  // Large
  a.resize(200, true);
  b.resize(200, true);

  EXPECT_EQ(190, a.count());
  EXPECT_EQ(190, b.count());

  a -= b;

  EXPECT_TRUE(a.none());

  b -= a;

  EXPECT_EQ(190, b.count());

  b -= b;

  EXPECT_TRUE(b.none());
}

// test(const &) intersection is non-empty
TYPED_TEST(BitTupleTest, IntersectionNonEmpty) {
  BitTuple<TypeParam> a(10, false);
  BitTuple<TypeParam> b(10, false);

  EXPECT_FALSE(a.test(b));
  EXPECT_FALSE(b.test(a));

  for (unsigned i = 0; i < 10; ++i) {
    a.reset();
    b.reset();
    a.set(0,i);
    b.set(i,10);
    EXPECT_FALSE(a.test(b)) << "i = " << i;
    EXPECT_FALSE(b.test(a)) << "i = " << i;
    a.set(i);
    EXPECT_TRUE(a.test(b)) << "i = " << i;
    EXPECT_TRUE(b.test(a)) << "i = " << i;
  }

  a.resize(200);
  b.resize(200);

  for (unsigned i = 0; i < 200; ++i) {
    a.reset();
    b.reset();
    a.set(0,i);
    b.set(i,200);
    EXPECT_FALSE(a.test(b)) << "i = " << i;
    EXPECT_FALSE(b.test(a)) << "i = " << i;
    a.set(i);
    EXPECT_TRUE(a.test(b)) << "i = " << i;
    EXPECT_TRUE(b.test(a)) << "i = " << i;
  }

}

// op|= union
TYPED_TEST(BitTupleTest, IntersectionOpOr) {
  // Small
  BitTuple<TypeParam> a(10, true);
  BitTuple<TypeParam> b(10, false);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  a |= b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  b |= a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());

  // Large
  a.set();
  a.resize(200, true);
  b.set();
  b.resize(200, true);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());

  a |= b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());

  b.reset();

  EXPECT_TRUE(b.none());

  a |= b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  b |= a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());
}

// op^= symmetric difference
TYPED_TEST(BitTupleTest, SymmetricDifferenceOpXorEqual) {
  // Small
  BitTuple<TypeParam> a(10, true);
  BitTuple<TypeParam> b(10, true);

  a ^= b;

  EXPECT_TRUE(a.none());
  EXPECT_TRUE(b.all());

  a ^= b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());

  // Large
  a.resize(200, true);
  b.resize(200, true);

  a ^= b;

  EXPECT_TRUE(a.none());
  EXPECT_TRUE(b.all());

  a ^= b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());
}

// op= copy assignment
TYPED_TEST(BitTupleTest, AssignmentCopy) {
  // Small
  BitTuple<TypeParam> x(10, true);
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());

  // Overwrite Small
  BitTuple<TypeParam> y;
  y = x;
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());
  EXPECT_EQ(10, y.size());
  EXPECT_EQ(10, y.count());
  x.reset();
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(0, x.count());
  EXPECT_EQ(10, y.size());
  EXPECT_EQ(10, y.count());
  x.set();

  // Overwrite Large
  BitTuple<TypeParam> z(200);
  EXPECT_EQ(200, z.size());
  z = x;
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());
  EXPECT_EQ(10, z.size());
  EXPECT_EQ(10, z.count());
  x.reset();
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(0, x.count());
  EXPECT_EQ(10, z.size());
  EXPECT_EQ(10, z.count());

  // Large
  BitTuple<TypeParam> a(200, true);
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(200, a.count());

  // Overwrite Small
  BitTuple<TypeParam> b;
  b = a;
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(200, a.count());
  EXPECT_EQ(200, b.size());
  EXPECT_EQ(200, b.count());
  a.reset();
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(0, a.count());
  EXPECT_EQ(200, b.size());
  EXPECT_EQ(200, b.count());
  a.set();

  // Overwrite Large
  BitTuple<TypeParam> c(200);
  EXPECT_EQ(200, c.size());
  c = a;
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(200, a.count());
  EXPECT_EQ(200, c.size());
  EXPECT_EQ(200, c.count());
  a.reset();
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(0, a.count());
  EXPECT_EQ(200, c.size());
  EXPECT_EQ(200, c.count());
}


// op= move assignment
TYPED_TEST(BitTupleTest, AssignmentMove) {
  // Small
  BitTuple<TypeParam> x(10, true);
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(10, x.count());

  // Overwrite Small
  BitTuple<TypeParam> y;
  y = std::move(x);
  EXPECT_EQ(0, x.size());
  EXPECT_EQ(10, y.size());
  EXPECT_EQ(10, y.count());
  x.resize(10, false);
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(0, x.count());
  EXPECT_EQ(10, y.size());
  EXPECT_EQ(10, y.count());
  x.set();

  // Overwrite Large
  BitTuple<TypeParam> z(200);
  EXPECT_EQ(200, z.size());
  z = std::move(x);
  EXPECT_EQ(0, x.size());
  EXPECT_EQ(0, x.count());
  EXPECT_EQ(10, z.size());
  EXPECT_EQ(10, z.count());
  x.resize(10, false);
  EXPECT_EQ(10, x.size());
  EXPECT_EQ(0, x.count());
  EXPECT_EQ(10, z.size());
  EXPECT_EQ(10, z.count());

  // Large
  BitTuple<TypeParam> a(200, true);
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(200, a.count());

  // Overwrite Small
  BitTuple<TypeParam> b;
  b = std::move(a);
  EXPECT_EQ(0, a.size());
  EXPECT_EQ(0, a.count());
  EXPECT_EQ(200, b.size());
  EXPECT_EQ(200, b.count());
  a.resize(200, false);
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(0, a.count());
  EXPECT_EQ(200, b.size());
  EXPECT_EQ(200, b.count());
  a.set();

  // Overwrite Large
  BitTuple<TypeParam> c(200);
  EXPECT_EQ(200, c.size());
  c = std::move(a);
  EXPECT_EQ(0, a.size());
  EXPECT_EQ(0, a.count());
  EXPECT_EQ(200, c.size());
  EXPECT_EQ(200, c.count());
  a.resize(200, false);
  EXPECT_EQ(200, a.size());
  EXPECT_EQ(0, a.count());
  EXPECT_EQ(200, c.size());
  EXPECT_EQ(200, c.count());
}

// swap
TYPED_TEST(BitTupleTest, MemberSwap) {
  // Small - Small
  BitTuple<TypeParam> a(10, false);
  BitTuple<TypeParam> b(8, true);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(10, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(8, b.size());

  a.swap(b);

  EXPECT_TRUE(a.all());
  EXPECT_EQ(8, a.size());
  EXPECT_TRUE(b.none());
  EXPECT_EQ(10, b.size());

  b.swap(a);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(10, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(8, b.size());

  // Small - Large
  b.resize(200, true);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(10, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(200, b.size());

  a.swap(b);

  EXPECT_TRUE(a.all());
  EXPECT_EQ(200, a.size());
  EXPECT_TRUE(b.none());
  EXPECT_EQ(10, b.size());

  a.swap(b);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(10, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(200, b.size());

  // Large - Large
  a.resize(199, false);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(199, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(200, b.size());

  a.swap(b);

  EXPECT_TRUE(a.all());
  EXPECT_EQ(200, a.size());
  EXPECT_TRUE(b.none());
  EXPECT_EQ(199, b.size());

  a.swap(b);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(199, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(200, b.size());
}

// free std::swap
TYPED_TEST(BitTupleTest, FreeSwap) {
  // Small - Small
  BitTuple<TypeParam> a(10, false);
  BitTuple<TypeParam> b(8, true);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(10, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(8, b.size());

  std::swap(a, b);

  EXPECT_TRUE(a.all());
  EXPECT_EQ(8, a.size());
  EXPECT_TRUE(b.none());
  EXPECT_EQ(10, b.size());

  std::swap(a, b);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(10, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(8, b.size());

  // Small - Large
  b.resize(200, true);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(10, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(200, b.size());

  std::swap(a, b);

  EXPECT_TRUE(a.all());
  EXPECT_EQ(200, a.size());
  EXPECT_TRUE(b.none());
  EXPECT_EQ(10, b.size());

  std::swap(a, b);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(10, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(200, b.size());

  // Large - Large
  a.resize(199, false);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(199, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(200, b.size());

  std::swap(a, b);

  EXPECT_TRUE(a.all());
  EXPECT_EQ(200, a.size());
  EXPECT_TRUE(b.none());
  EXPECT_EQ(199, b.size());

  std::swap(a, b);

  EXPECT_TRUE(a.none());
  EXPECT_EQ(199, a.size());
  EXPECT_TRUE(b.all());
  EXPECT_EQ(200, b.size());
}


// free op& intersection
TYPED_TEST(BitTupleTest, IntersectionFree) {
  // Small
  BitTuple<TypeParam> a(10, true);
  BitTuple<TypeParam> b(10, false);
  BitTuple<TypeParam> c;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  c = a & b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.none());
  EXPECT_EQ(10, c.size());

  c = b & a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.none());
  EXPECT_EQ(10, c.size());

  c = a & a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());
  EXPECT_EQ(10, c.size());

  // Large
  a.set();
  a.resize(200, true);
  b.set();
  b.resize(200, true);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());

  c = a & b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());
  EXPECT_TRUE(c.all());
  EXPECT_EQ(200, c.size());

  b.reset();

  EXPECT_TRUE(b.none());

  c = a & b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.none());
  EXPECT_EQ(200, c.size());
}

// free op| union
TYPED_TEST(BitTupleTest, UnionFree) {
  // Small
  BitTuple<TypeParam> a(10, true);
  BitTuple<TypeParam> b(10, false);
  BitTuple<TypeParam> c;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  c = a | b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());
  EXPECT_EQ(10, c.size());

  c = b | a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());
  EXPECT_EQ(10, c.size());

  c = a | a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());
  EXPECT_EQ(10, c.size());

  // Large
  a.set();
  a.resize(200, true);
  b.set();
  b.resize(200, true);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());

  c = a | b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());
  EXPECT_TRUE(c.all());
  EXPECT_EQ(200, c.size());

  b.reset();

  EXPECT_TRUE(b.none());

  c = a | b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());
  EXPECT_EQ(200, c.size());
}

// free op- difference
TYPED_TEST(BitTupleTest, DifferenceFree) {
  // Small
  BitTuple<TypeParam> a(10, true);
  BitTuple<TypeParam> b(10, false);
  BitTuple<TypeParam> c;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());

  c = a - b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());
  EXPECT_EQ(10, c.size());

  c = b - a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.none());
  EXPECT_EQ(10, c.size());

  c = a - a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.none());
  EXPECT_EQ(10, c.size());

  // Large
  a.set();
  a.resize(200, true);
  b.set();
  b.resize(200, true);

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());

  c = a - b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());
  EXPECT_TRUE(c.none());
  EXPECT_EQ(200, c.size());

  b.reset();

  EXPECT_TRUE(b.none());

  c = a - b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());
  EXPECT_EQ(200, c.size());
}

// free op^ symmetric difference
TYPED_TEST(BitTupleTest, SymmetricDifferenceFree) {
  // Small
  BitTuple<TypeParam> a(10, true);
  BitTuple<TypeParam> b(10, true);

  BitTuple<TypeParam> c;
  c = a ^ b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());
  EXPECT_TRUE(c.none());

  c = b ^ a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());
  EXPECT_TRUE(c.none());

  b.reset();

  c = a ^ b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());

  c = b ^ a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());

  b.set();
  // Large
  a.resize(200, true);
  b.resize(200, true);

  c = a ^ b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());
  EXPECT_TRUE(c.none());

  c = b ^ a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.all());
  EXPECT_TRUE(c.none());

  b.reset();

  c = a ^ b;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());

  c = b ^ a;

  EXPECT_TRUE(a.all());
  EXPECT_TRUE(b.none());
  EXPECT_TRUE(c.all());
}
