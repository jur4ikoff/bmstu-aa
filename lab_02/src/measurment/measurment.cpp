#include "measurment.h"
#include "constants.h"
#include "sequence.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <memory>
#include <stdexcept>

void run_standart(std::ofstream &file, std::shared_ptr<Sequence> sequence,
                  size_t size, const std::string &exp_case) {
  std::cout << "Запуск замера времени стандартного поиска максимума, размером: "
            << size << std::endl;
  int max = INT_MIN;
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t j = 0; j < EXP_COUNT; j++) {
    max = INT_MIN;
    sequence->default_max(max);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  file << "Итерационный метод поиска максимума;" << exp_case << ';' << size
       << ";" << duration.count() / EXP_COUNT << "\n";
}

void run_recursive(std::ofstream &file, std::shared_ptr<Sequence> sequence,
                   size_t size, const std::string &exp_case) {
  std::cout << "Запуск замера времени рекурсивного поиска максимума, размером: "
            << size << std::endl;
  int max = INT_MIN;
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t j = 0; j < EXP_COUNT; j++) {
    max = INT_MIN;
    sequence->recursion_max(max, 0);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  file << "Рекурсивный метод поиска максимума;" << exp_case << ';' << size
       << ";" << duration.count() / EXP_COUNT << "\n";
}

void run(size_t start_size, size_t end_size, size_t n) {
  const std::string case_names[CASE_NUMBER] = {"random", "straight", "reverse"};

  std::ofstream file(DATA_PATH);
  if (!file.is_open()) {
    throw std::runtime_error("ОШИБКА!!! Файл В С Ë");
  }

  file << "algorithm;case;size;time\n";

  for (size_t j = 0; j < CASE_NUMBER; j++) {
    auto sequence = std::make_shared<Sequence>();
    for (size_t i = start_size; i <= end_size; i += n) {
      if (case_names[j] == "random")
        sequence->random_fill(i, 0, 1000);
      else if (case_names[j] == "straight")
        sequence->straight_fill(i, 0);
      else
        sequence->reverse_fill(i, i);

      run_standart(file, sequence, i, case_names[j]);
      run_recursive(file, sequence, i, case_names[j]);
      std::cout << std::endl;
    }
  }

  file.close();
  std::cout << "Вызов Python скрипта для графиков\n";
  system(PYTHON_COMMAND);
}
