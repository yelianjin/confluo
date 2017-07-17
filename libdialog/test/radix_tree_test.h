#ifndef TEST_RADIX_TREE_TEST_H_
#define TEST_RADIX_TREE_TEST_H_

#include "radix_tree.h"
#include "gtest/gtest.h"

using namespace ::dialog::index;

class RadixTreeTest : public testing::Test {
};

TEST_F(RadixTreeTest, InsertGetTest) {
  radix_tree tree(sizeof(int32_t), 256);
  for (int32_t i = 0; i < 256; i++)
    tree.insert(byte_string(i * 8), i);

  for (int32_t i = 0; i < 256; i++) {
    const reflog* r = tree.at(byte_string(i * 8));
    ASSERT_TRUE(r != nullptr);
    ASSERT_EQ(static_cast<size_t>(i), r->at(0));
  }
}

TEST_F(RadixTreeTest, UpperLowerBoundTest) {
  radix_tree tree(sizeof(int32_t), 256);
  for (int32_t i = 0; i < 256; i++)
    tree.insert(byte_string(i * 8), i);

  auto it1 = tree.upper_bound(byte_string(8));
  ASSERT_EQ(static_cast<size_t>(1), it1->at(0));

  auto it2 = tree.lower_bound(byte_string(8));
  ASSERT_EQ(static_cast<size_t>(1), it2->at(0));

  auto it3 = tree.upper_bound(byte_string(7));
  ASSERT_EQ(static_cast<size_t>(1), it3->at(0));

  auto it4 = tree.lower_bound(byte_string(9));
  ASSERT_EQ(static_cast<size_t>(1), it4->at(0));

  auto it5 = tree.upper_bound(byte_string(255));
  ASSERT_EQ(static_cast<size_t>(32), it5->at(0));
}

TEST_F(RadixTreeTest, ReflogRangeLookupTest) {
  radix_tree tree(sizeof(int32_t), 256);
  for (int32_t i = 0; i < 256; i++)
    tree.insert(byte_string(i * 8), i);

  auto res1 = tree.range_lookup_reflogs(byte_string(0), byte_string(16));
  ASSERT_EQ(static_cast<size_t>(3), res1.count());
  uint64_t i = 0;
  for (reflog& refs : res1) {
    ASSERT_EQ(i, refs.at(0));
    i++;
  }

  auto res2 = tree.range_lookup_reflogs(byte_string(1), byte_string(15));
  ASSERT_EQ(static_cast<size_t>(1), res2.count());
  i = 1;
  for (reflog& refs : res2) {
    ASSERT_EQ(i, refs.at(0));
    i++;
  }
}

TEST_F(RadixTreeTest, ApproxCountTest) {
  radix_tree tree(sizeof(int32_t), 256);
  for (int32_t i = 0; i < 256; i++) {
    for (int32_t j = 0; j < i; j++) {
      tree.insert(byte_string(i * 8), i);
    }
  }

  size_t res1 = tree.approx_count(byte_string(0), byte_string(32));
  ASSERT_EQ(static_cast<size_t>(10), res1);

  size_t res2 = tree.approx_count(byte_string(1), byte_string(31));
  ASSERT_EQ(static_cast<size_t>(6), res2);
}

TEST_F(RadixTreeTest, RangeLookupTest) {
  radix_tree tree(sizeof(int32_t), 256);
  for (int32_t i = 0; i < 256; i++)
    tree.insert(byte_string(i * 8), i);

  auto res1f = tree.range_lookup(byte_string(0), byte_string(16));
  ASSERT_EQ(static_cast<size_t>(3), res1f.count());
  uint64_t i = 0;
  for (uint64_t val : res1f) {
    ASSERT_EQ(i, val);
    i++;
  }

  auto res2f = tree.range_lookup(byte_string(1), byte_string(15));
  ASSERT_EQ(static_cast<size_t>(1), res2f.count());
  i = 1;
  for (uint64_t val : res2f) {
    ASSERT_EQ(i, val);
    i++;
  }
}

#endif /* TEST_RADIX_TREE_TEST_H_ */
