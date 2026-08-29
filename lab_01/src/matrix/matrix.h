#pragma once

#include "constants.h"

#include <memory>
#include <vector>

class Matrix {
  using size_type = size_t;

public:
  Matrix() {
    _rows_count = 0;
    _columns_count = 0;
  };
  Matrix(size_t rows, size_t columns);
  Matrix(size_t size);
  Matrix(Matrix &other);
  Matrix(size_type rows, size_type columns, std::initializer_list<value_type>);

  // Оператор присваивания перемещением
  Matrix &operator=(Matrix &&other) noexcept {
    if (this != &other) {
      _rows_count = std::move(other._rows_count);
      _columns_count = std::move(other._columns_count);
      _matrix = std::move(other._matrix);
    }
    return *this;
  }
  bool operator==(const Matrix &other) const;

  void read_matrix_from_console();
  void fill_random(value_type min_val, value_type max_val);

  size_type rows_count(void) { return _rows_count; }

  size_type columns_count(void) { return _columns_count; }

  void multipy(Matrix &other, Matrix &answer);
  void winograd_algorithm(Matrix &other, Matrix &answer);
  void optimize_winograd_algorithm(Matrix &other, Matrix &answer);

  friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);

  ~Matrix() = default;

private:
  std::vector<std::vector<value_type>> _matrix;
  size_type _columns_count;
  size_type _rows_count;
};