#include "sequence.h"

#include "gtest/gtest.h"
#include <limits.h>

TEST(RecursiveMax, seq_empty) {
  auto sequence = std::make_shared<Sequence>();
  int max = INT_MIN;
  sequence->recursion_max(max, 0);
  EXPECT_EQ(max, INT_MIN);
}

TEST(RecursiveMax, max_null) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{-1, -3, 0});
  ;
  int exp_max = 0;
  int max = INT_MIN;
  sequence->recursion_max(max, 0);
  EXPECT_EQ(max, exp_max);
}

TEST(RecursiveMax, max_neg) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{-1, -3, -6});
  ;
  int exp_max = -1;
  int max = INT_MIN;
  sequence->recursion_max(max, 0);
  EXPECT_EQ(max, exp_max);
}

TEST(RecursiveMax, max_pos) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{100, 23, 6});
  ;
  int exp_max = 100;
  int max = INT_MIN;
  sequence->recursion_max(max, 0);
  EXPECT_EQ(max, exp_max);
}

TEST(RecursiveMax, seq_1_el) {
  auto sequence = std::make_shared<Sequence>(std::initializer_list<int>{100});
  ;
  int exp_max = 100;
  int max = INT_MIN;
  sequence->recursion_max(max, 0);
  EXPECT_EQ(max, exp_max);
}

TEST(RecursiveMax, seq_2_el) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{100, 6});
  ;
  int exp_max = 100;
  int max = INT_MIN;
  sequence->recursion_max(max, 0);
  EXPECT_EQ(max, exp_max);
}

TEST(RecursiveMax, max_is_first) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{100, 50, 20, 40});
  ;
  int exp_max = 100;
  int max = INT_MIN;
  sequence->recursion_max(max, 0);
  EXPECT_EQ(max, exp_max);
}

TEST(RecursiveMax, max_is_last) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{100, 50, 33, 200});
  ;
  int exp_max = 200;
  int max = INT_MIN;
  sequence->recursion_max(max, 0);
  EXPECT_EQ(max, exp_max);
}

TEST(RecursiveMax, max_in_middle) {
  auto sequence = std::make_shared<Sequence>(
      std::initializer_list<int>{40, 20, 10, 0, 100, 50});
  ;
  int exp_max = 100;
  int max = INT_MIN;
  sequence->recursion_max(max, 0);
  EXPECT_EQ(max, exp_max);
}
