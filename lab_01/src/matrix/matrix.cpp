#include "matrix.h"
#include <iostream>
#include <stdexcept>

#include <chrono>
#include <random>

Matrix::Matrix(size_t rows, size_t columns)
    : _columns_count(columns), _rows_count(rows) {
  _matrix = std::vector<std::vector<value_type>>(
      rows, std::vector<value_type>(columns));
}

Matrix::Matrix(size_t size) : _columns_count(size), _rows_count(size) {
  _matrix =
      std::vector<std::vector<value_type>>(size, std::vector<value_type>(size));
}

Matrix::Matrix(Matrix &other) {
  this->_rows_count = other._rows_count;
  this->_columns_count = other._columns_count;
  this->_matrix = other._matrix;
}

Matrix::Matrix(size_type rows, size_type columns,
               std::initializer_list<value_type> args) {
  _rows_count = rows;
  _columns_count = columns;

  _matrix = std::vector<std::vector<value_type>>(
      _rows_count, std::vector<value_type>(_columns_count));

  auto it = args.begin();
  for (size_t i = 0; i < _rows_count; i++) {
    for (size_t j = 0; j < _columns_count; j++) {
      _matrix[i][j] = *it;
      it++;
    }
  }
}

void Matrix::read_matrix_from_console() {
  std::cout << "Введите размеры матрицы через пробел: ";

  int n, m;
  if (!(std::cin >> n >> m) || n <= 0 || m <= 0) {
    throw std::invalid_argument("ОШИБКА!!! Неверно введен размер матрицы");
  }

  _rows_count = n;
  _columns_count = m;
  _matrix = std::vector<std::vector<value_type>>(n, std::vector<value_type>(m));

  std::cout << "Введите элементы матрицы через пробел:" << std::endl;
  for (size_type i = 0; i < _rows_count; i++) {
    for (size_type j = 0; j < _columns_count; j++) {
      if (!(std::cin >> _matrix[i][j])) {
        throw std::invalid_argument("Неверный ввод элемента матрицы");
      }
    }
  }
}

// Реализация оператора вывода
std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
  for (size_t i = 0; i < matrix._rows_count; i++) {
    for (size_t j = 0; j < matrix._columns_count; j++) {
      os << matrix._matrix[i][j] << '\t';
    }
    os << '\n';
  }
  return os;
}

void Matrix::multipy(Matrix &other, Matrix &answer) {
  if (this->_columns_count != other._rows_count) {
    throw std::invalid_argument("ОШИБКА!!! Количество столбцов матрицы должен "
                                "быть равен количеству строк второму");
  }

  if (this->_columns_count < 1 || this->_rows_count < 1 ||
      other._rows_count < 1 || other._columns_count < 1) {
    throw std::invalid_argument("ОШИБКА!!! Матрица пустая");
  }

  size_type n = this->_rows_count;
  size_type m = other._rows_count;
  size_type t = other._columns_count;

  if (answer.rows_count() != n || answer.columns_count() != t) {
    throw std::invalid_argument(
        "ОШИБКА!!! Матрица для ответа непроинициализирована");
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < t; j++) {
      for (size_t k = 0; k < m; k++) {
        answer._matrix[i][j] += this->_matrix[i][k] * other._matrix[k][j];
      }
    }
  }
}

void Matrix::winograd_algorithm(Matrix &other, Matrix &answer) {
  if (this->_columns_count != other._rows_count) {
    throw std::invalid_argument("ОШИБКА!!! Количество столбцов матрицы должен "
                                "быть равен количеству строк второму");
  }

  if (this->_columns_count < 1 || this->_rows_count < 1 ||
      other._rows_count < 1 || other._columns_count < 1) {
    throw std::invalid_argument("ОШИБКА!!! Матрица пустая");
  }

  size_type n = this->_rows_count;
  size_type m = other._rows_count;
  size_type t = other._columns_count;

  if (answer.rows_count() != n || answer.columns_count() != t) {
    throw std::invalid_argument(
        "ОШИБКА!!! Матрица для ответа непроинициализирована");
  }

  std::vector<value_type> row_sum(n), col_sum(t);

  for (size_type i = 0; i < n; i++) {
    for (size_type j = 0; j < m / 2; j++) {
      row_sum[i] += (this->_matrix[i][2 * j] * this->_matrix[i][2 * j + 1]);
    }
  }
  for (size_type i = 0; i < t; i++) {
    for (size_type j = 0; j < m / 2; j++) {
      col_sum[i] += (other._matrix[2 * j][i] * other._matrix[2 * j + 1][i]);
    }
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < t; j++) {
      answer._matrix[i][j] = -row_sum[i] - col_sum[j];
      for (size_t k = 0; k < m / 2; k++) {
        answer._matrix[i][j] +=
            (this->_matrix[i][2 * k + 1] + other._matrix[2 * k][j]) *
            (this->_matrix[i][2 * k] + other._matrix[2 * k + 1][j]);
      }
    }
  }

  if (m % 2 == 1) {
    for (size_type i = 0; i < n; i++) {
      for (size_type j = 0; j < t; j++) {
        answer._matrix[i][j] +=
            this->_matrix[i][m - 1] * other._matrix[m - 1][j];
      }
    }
  }
}

void Matrix::optimize_winograd_algorithm(Matrix &other, Matrix &answer) {
  if (this->_columns_count != other._rows_count) {
    throw std::invalid_argument("ОШИБКА!!! Количество столбцов матрицы должен "
                                "быть равен количеству строк второму");
  }

  if (this->_columns_count < 1 || this->_rows_count < 1 ||
      other._rows_count < 1 || other._columns_count < 1) {
    throw std::invalid_argument("ОШИБКА!!! Матрица пустая");
  }

  size_type n = this->_rows_count;
  size_type m = other._rows_count;
  size_type t = other._columns_count;

  if (answer.rows_count() != n || answer.columns_count() != t) {
    throw std::invalid_argument(
        "ОШИБКА!!! Матрица для ответа непроинициализирована");
  }

  size_type half_m = m / 2;
  size_type m_minus_one = m - 1;

  std::vector<value_type> row_sum(n), col_sum(t);

  for (size_type i = 0; i < n; i++) {
    for (size_type j = 0; j < half_m; j++) {
      row_sum[i] += (this->_matrix[i][j << 1] * this->_matrix[i][(j << 1) + 1]);
    }
  }
  for (size_type i = 0; i < t; i++) {
    for (size_type j = 0; j < half_m; j++) {
      col_sum[i] += (other._matrix[j << 1][i] * other._matrix[(j << 1) + 1][i]);
    }
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < t; j++) {
      answer._matrix[i][j] = -row_sum[i] - col_sum[j];
      for (size_t k = 0; k < half_m; k++) {
        answer._matrix[i][j] +=
            (this->_matrix[i][(k << 1) + 1] + other._matrix[k << 1][j]) *
            (this->_matrix[i][k << 1] + other._matrix[(k << 1) + 1][j]);
      }
    }
  }

  if (m % 2 == 1) {
    for (size_type i = 0; i < n; i++) {
      for (size_type j = 0; j < t; j++) {
        answer._matrix[i][j] +=
            this->_matrix[i][m_minus_one] * other._matrix[m_minus_one][j];
      }
    }
  }
}

void Matrix::fill_random(value_type min_value, value_type max_value) {
  // Инициализация генератора случайных чисел
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);
  std::uniform_int_distribution<value_type> distribution(min_value, max_value);

  // Заполнение матрицы случайными числами
  for (size_type i = 0; i < _rows_count; i++) {
    for (size_type j = 0; j < _columns_count; j++) {
      _matrix[i][j] = distribution(generator);
    }
  }
}

bool Matrix::operator==(const Matrix &other) const {
  if (_rows_count != other._rows_count ||
      _columns_count != other._columns_count) {
    return false;
  }

  for (size_type i = 0; i < _rows_count; i++) {
    if (!std::equal(_matrix[i].begin(), _matrix[i].end(),
                    other._matrix[i].begin())) {
      return false;
    }
  }

  return true;
}