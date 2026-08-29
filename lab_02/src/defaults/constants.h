#pragma once
#include <iostream>
#include <stdexcept>

#define ERR_OK 0
#define ERR 1

using value_type = int;
using size_type = size_t;

typedef enum { OP_EXIT, OP_MAX, OP_TEST } operations_t;

class OperationManager {
public:
  operations_t operation;

  OperationManager() = default;

  void print_menu() {
    std::cout << "Меню:\n";
    std::cout << "0. Выход\n";
    std::cout
        << "1. Найти максимальное значение элементво последовательности\n";
    std::cout << "2. Замеры\n";
  }

  void read_operation(bool verbose = true) {
    int temp_operation;

    std::cout << "Введите номер операции: ";
    if (!(std::cin >> temp_operation) || temp_operation < 0 ||
        temp_operation > OP_TEST)
      throw std::invalid_argument("ОШИБКА, неверный диапазон данных");

    operation = static_cast<operations_t>(temp_operation);
  }
};
