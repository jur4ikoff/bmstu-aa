#include "constants.h"
#include "measurment.h"
#include "sequence.h"

#include <fstream>
#include <iostream>
#include <limits.h>
#include <memory>
#include <stdexcept>

size_t input_size() {
  int n;
  std::cin >> n;
  if (n < 1)
    throw std::invalid_argument("ОШИБКА, неверный размер");
  return static_cast<size_t>(n);
}

void first_operation(void) {
  auto sequence = std::make_shared<Sequence>();
  sequence->read_from_console();

  std::cout << "\nСчитанная последовательность:\n";
  std::cout << *sequence << std::endl << std::endl;

  // Дефолтный алгоритм
  int max = INT_MIN;
  sequence->default_max(max);
  std::cout
      << "Максимальный элемент последовательности (стандартный алгоритм): "
      << max << std::endl;

  // Рекурсия
  max = INT_MIN;
  sequence->recursion_max(max, 0);
  std::cout
      << "Максимальный элемент последовательности (рекурсивный алгоритм): "
      << max << std::endl;
}

void second_operation(void) {
  size_t start_size, end_size, n;
  try {
    std::cout << "Введите начальный и конечный размер через пробел: ";
    start_size = input_size();
    end_size = input_size();

    std::cout << "Введите количество замеров: ";
    n = input_size();
  } catch (const std::invalid_argument &e) {
    std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
    return;
  }

  try {
    run(start_size, end_size, n);
  } catch (const std::invalid_argument &e) {
    std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
    return;
  }
}

int main() {

  auto operarion_manager = std::make_shared<OperationManager>();
  operarion_manager->print_menu();
  operarion_manager->read_operation();

  if (operarion_manager->operation == OP_EXIT) {
    exit(0);
  } else if (operarion_manager->operation == OP_MAX) {
    first_operation();
  } else if (operarion_manager->operation == OP_TEST) {
    second_operation();
  }
}

// if (operation == 1)
// {
//   try
//   {
//     return first_mode();
//   }
//   catch (const std::invalid_argument &e)
//   {
//     std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
//     return ERR;
//   }
// }
// else if (operation == 2)
// {
//   try
//   {
//     ;
//     // return second_mode();
//   }
//   catch (const std::invalid_argument &e)
//   {
//     std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
//     return ERR;
//   }
//