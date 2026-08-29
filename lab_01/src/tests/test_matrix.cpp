#include "matrix.h"
#include "gtest/gtest.h"

TEST(DefaultMultipy, mul_1v1) {
  auto matrix_1 = std::make_shared<Matrix>(1, 1, std::initializer_list<int>{2});
  auto matrix_2 = std::make_shared<Matrix>(1, 1, std::initializer_list<int>{3});
  auto exp_answer =
      std::make_shared<Matrix>(1, 1, std::initializer_list<int>{6});

  auto answer = std::make_shared<Matrix>(1, 1);
  matrix_1->multipy(*matrix_2, *answer);

  bool rc = (*answer == *exp_answer);
  EXPECT_EQ(rc, true);
}

TEST(DefaultMultipy, mul_square) {
  auto matrix_1 =
      std::make_shared<Matrix>(2, 2, std::initializer_list<int>{1, 2, 3, 4});
  auto matrix_2 =
      std::make_shared<Matrix>(2, 2, std::initializer_list<int>{5, 6, 7, 8});
  auto exp_answer = std::make_shared<Matrix>(
      2, 2, std::initializer_list<int>{19, 22, 43, 50});

  auto answer = std::make_shared<Matrix>(2, 2);
  matrix_1->multipy(*matrix_2, *answer);

  bool rc = (*answer == *exp_answer);
  EXPECT_EQ(rc, true);
}

TEST(DefaultMultipy, mul_rect) {
  auto matrix_1 = std::make_shared<Matrix>(
      3, 2, std::initializer_list<int>{1, 2, 3, 4, 5, 6});
  auto matrix_2 = std::make_shared<Matrix>(
      2, 3, std::initializer_list<int>{7, 8, 9, 10, 11, 12});
  auto exp_answer = std::make_shared<Matrix>(
      3, 3, std::initializer_list<int>{27, 30, 33, 61, 68, 75, 95, 106, 117});

  auto answer = std::make_shared<Matrix>(3, 3);
  matrix_1->multipy(*matrix_2, *answer);

  bool rc = (*answer == *exp_answer);
  EXPECT_EQ(rc, true);
}

TEST(WinogradovMultipy, mul_1v1) {
  auto matrix_1 = std::make_shared<Matrix>(1, 1, std::initializer_list<int>{2});
  auto matrix_2 = std::make_shared<Matrix>(1, 1, std::initializer_list<int>{3});
  auto exp_answer =
      std::make_shared<Matrix>(1, 1, std::initializer_list<int>{6});

  auto answer = std::make_shared<Matrix>(1, 1);
  matrix_1->winograd_algorithm(*matrix_2, *answer);

  bool rc = (*answer == *exp_answer);
  EXPECT_EQ(rc, true);
}

TEST(WinogradovMultipy, mul_square) {
  auto matrix_1 =
      std::make_shared<Matrix>(2, 2, std::initializer_list<int>{1, 2, 3, 4});
  auto matrix_2 =
      std::make_shared<Matrix>(2, 2, std::initializer_list<int>{5, 6, 7, 8});
  auto exp_answer = std::make_shared<Matrix>(
      2, 2, std::initializer_list<int>{19, 22, 43, 50});

  auto answer = std::make_shared<Matrix>(2, 2);
  matrix_1->winograd_algorithm(*matrix_2, *answer);

  bool rc = (*answer == *exp_answer);
  EXPECT_EQ(rc, true);
}

TEST(WinogradovMultipy, mul_rect) {
  auto matrix_1 = std::make_shared<Matrix>(
      3, 2, std::initializer_list<int>{1, 2, 3, 4, 5, 6});
  auto matrix_2 = std::make_shared<Matrix>(
      2, 3, std::initializer_list<int>{7, 8, 9, 10, 11, 12});
  auto exp_answer = std::make_shared<Matrix>(
      3, 3, std::initializer_list<int>{27, 30, 33, 61, 68, 75, 95, 106, 117});

  auto answer = std::make_shared<Matrix>(3, 3);
  matrix_1->winograd_algorithm(*matrix_2, *answer);

  bool rc = (*answer == *exp_answer);
  EXPECT_EQ(rc, true);
}

TEST(WinogradovMultipyOptimize, mul_1v1) {
  auto matrix_1 = std::make_shared<Matrix>(1, 1, std::initializer_list<int>{2});
  auto matrix_2 = std::make_shared<Matrix>(1, 1, std::initializer_list<int>{3});
  auto exp_answer =
      std::make_shared<Matrix>(1, 1, std::initializer_list<int>{6});

  auto answer = std::make_shared<Matrix>(1, 1);
  matrix_1->optimize_winograd_algorithm(*matrix_2, *answer);

  bool rc = (*answer == *exp_answer);
  EXPECT_EQ(rc, true);
}

TEST(WinogradovMultipyOptimize, mul_square) {
  auto matrix_1 =
      std::make_shared<Matrix>(2, 2, std::initializer_list<int>{1, 2, 3, 4});
  auto matrix_2 =
      std::make_shared<Matrix>(2, 2, std::initializer_list<int>{5, 6, 7, 8});
  auto exp_answer = std::make_shared<Matrix>(
      2, 2, std::initializer_list<int>{19, 22, 43, 50});

  auto answer = std::make_shared<Matrix>(2, 2);
  matrix_1->optimize_winograd_algorithm(*matrix_2, *answer);

  bool rc = (*answer == *exp_answer);
  EXPECT_EQ(rc, true);
}

TEST(WinogradovMultipyOptimize, mul_rect) {
  auto matrix_1 = std::make_shared<Matrix>(
      3, 2, std::initializer_list<int>{1, 2, 3, 4, 5, 6});
  auto matrix_2 = std::make_shared<Matrix>(
      2, 3, std::initializer_list<int>{7, 8, 9, 10, 11, 12});
  auto exp_answer = std::make_shared<Matrix>(
      3, 3, std::initializer_list<int>{27, 30, 33, 61, 68, 75, 95, 106, 117});

  auto answer = std::make_shared<Matrix>(3, 3);
  matrix_1->optimize_winograd_algorithm(*matrix_2, *answer);

  bool rc = (*answer == *exp_answer);
  EXPECT_EQ(rc, true);
}
