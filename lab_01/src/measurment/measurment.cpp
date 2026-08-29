#include "measurment.h"
#include "constants.h"
#include "matrix.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>

static void run_default(std::ofstream &file, Matrix &matrix_1, Matrix &matrix_2,
                        Matrix &answer, size_t size) {
  // обычный замер
  std::cout << "Запуск замера времени обычного умножения, размером: " << size
            << std::endl;
  auto start = std::chrono::high_resolution_clock::now();

  for (size_t j = 0; j < EXP_COUNT; j++) {
    matrix_1.multipy(matrix_2, answer);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  file << "Стандартный метод умножения матриц;" << size << ";"
       << duration.count() / EXP_COUNT << "\n";
}

// Dr. winoGradov
static void run_winogradov(std::ofstream &file, Matrix &matrix_1,
                           Matrix &matrix_2, Matrix &answer, size_t size) {
  std::cout
      << "Запуск замера времени умножения алгоритмом Виногдрадова, размером: "
      << size << std::endl;
  auto start = std::chrono::high_resolution_clock::now();

  for (size_t j = 0; j < EXP_COUNT; j++) {
    matrix_1.winograd_algorithm(matrix_2, answer);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  file << "Метод Виноградова;" << size << ";" << duration.count() / EXP_COUNT
       << "\n";
}

// Замер оптимизированного виноградова
static void run_winograd_opt(std::ofstream &file, Matrix &matrix_1,
                             Matrix &matrix_2, Matrix &answer, size_t size) {
  std::cout
      << "Запуск замера времени умножения алгоритмом Виногдрадова, размером: "
      << size << std::endl;
  auto start = std::chrono::high_resolution_clock::now();

  for (size_t j = 0; j < EXP_COUNT; j++) {
    matrix_1.optimize_winograd_algorithm(matrix_2, answer);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  file << "Оптимизированный метод Виноградова;" << size << ";"
       << duration.count() / EXP_COUNT << "\n";
}

void run(size_t start_size, size_t end_size, size_t n) {
  std::ofstream file(DATA_PATH);
  if (!file.is_open()) {
    throw std::runtime_error("ОШИБКА!!! Файл В С Ë");
  }

  file << "method;size;time\n";

  for (size_t i = start_size; i <= end_size; i += n) {
    auto matrix_1 = std::make_shared<Matrix>(i);
    auto matrix_2 = std::make_shared<Matrix>(i);
    auto answer = std::make_shared<Matrix>(i);

    matrix_1->fill_random(0, 1000);
    matrix_2->fill_random(0, 1000);

    run_default(file, *matrix_1, *matrix_2, *answer, i);
    run_winogradov(file, *matrix_1, *matrix_2, *answer, i);
    run_winograd_opt(file, *matrix_1, *matrix_2, *answer, i);
    std::cout << std::endl;
  }

  file.close();
  system(PYTHON_COMMAND);
}
