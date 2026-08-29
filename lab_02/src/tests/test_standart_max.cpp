#include "sequence.h"

#include "gtest/gtest.h"
#include <limits.h>

TEST(StandartMax, seq_empty) {
  auto sequence = std::make_shared<Sequence>();
  int max = INT_MIN;
  sequence->default_max(max);
  EXPECT_EQ(max, INT_MIN);
}

TEST(StandartMax, max_null) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{-1, -3, 0});
  ;
  int exp_max = 0;
  int max = INT_MIN;
  sequence->default_max(max);
  EXPECT_EQ(max, exp_max);
}

TEST(StandartMax, max_neg) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{-1, -3, -6});
  ;
  int exp_max = -1;
  int max = INT_MIN;
  sequence->default_max(max);
  EXPECT_EQ(max, exp_max);
}

TEST(StandartMax, max_pos) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{100, 23, 6});
  ;
  int exp_max = 100;
  int max = INT_MIN;
  sequence->default_max(max);
  EXPECT_EQ(max, exp_max);
}

TEST(StandartMax, seq_1_el) {
  auto sequence = std::make_shared<Sequence>(std::initializer_list<int>{100});
  ;
  int exp_max = 100;
  int max = INT_MIN;
  sequence->default_max(max);
  EXPECT_EQ(max, exp_max);
}

TEST(StandartMax, seq_2_el) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{100, 6});
  ;
  int exp_max = 100;
  int max = INT_MIN;
  sequence->default_max(max);
  EXPECT_EQ(max, exp_max);
}

TEST(StandartMax, max_is_first) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{100, 50, 20, 40});
  ;
  int exp_max = 100;
  int max = INT_MIN;
  sequence->default_max(max);
  EXPECT_EQ(max, exp_max);
}

TEST(StandartMax, max_is_last) {
  auto sequence =
      std::make_shared<Sequence>(std::initializer_list<int>{100, 50, 33, 200});
  ;
  int exp_max = 200;
  int max = INT_MIN;
  sequence->default_max(max);
  EXPECT_EQ(max, exp_max);
}

TEST(StandartMax, max_in_middle) {
  auto sequence = std::make_shared<Sequence>(
      std::initializer_list<int>{40, 20, 10, 0, 100, 50});
  ;
  int exp_max = 100;
  int max = INT_MIN;
  sequence->default_max(max);
  EXPECT_EQ(max, exp_max);
}
