#include "sequence.h"

#include "gtest/gtest.h"
#include <iostream>
#include <limits.h>

TEST(Fill, random) {
  auto sequence = std::make_shared<Sequence>();
  sequence->random_fill(1, 1, 1000);

  int exp_size = 1;
  EXPECT_EQ(sequence->size(), exp_size);
}

TEST(Fill, straight) {
  auto sequence = std::make_shared<Sequence>();
  sequence->straight_fill(5, 3);

  int exp_size = 5;
  EXPECT_EQ(sequence->size(), exp_size);
}

TEST(Fill, reverse) {
  auto sequence = std::make_shared<Sequence>();
  sequence->reverse_fill(5, 8);

  int exp_size = 5;
  EXPECT_EQ(sequence->size(), exp_size);
}
