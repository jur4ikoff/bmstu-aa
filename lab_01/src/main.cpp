#include "constants.h"
#include "matrix.h"
#include "matrix/matrix.h"
#include "measurment.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

size_t input_size() {
  int temp;

  if (!(std::cin >> temp) || temp < 1) {
    throw std::invalid_argument("Неверный ввод размера");
  }

  return static_cast<size_t>(temp);
}

// Реализует первый режим работы программы
// Запрос матрицы у пользователя и расчет умноженич несколькими способами
int first_mode() {
  auto matrix_1 = std::make_shared<Matrix>();
  auto matrix_2 = std::make_shared<Matrix>();

  try {
    matrix_1->read_matrix_from_console();
    matrix_2->read_matrix_from_console();
  } catch (const std::invalid_argument &e) {
    std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
    return ERR;
  }

  auto answer = std::make_shared<Matrix>(matrix_1->rows_count(),
                                         matrix_2->columns_count());
  try {
    matrix_1->multipy(*matrix_2, *answer);
    std::cout << "Обычное умножение\n";
    std::cout << *answer;
    std::cout << std::endl;
  } catch (const std::invalid_argument &e) {
    std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
    return ERR;
  }

  try {
    matrix_1->winograd_algorithm(*matrix_2, *answer);
    std::cout << "Умножение алгоритмом виногдрада" << std::endl;
    std::cout << *answer;
    std::cout << std::endl;
  } catch (const std::invalid_argument &e) {
    std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
    return ERR;
  }

  try {
    matrix_1->optimize_winograd_algorithm(*matrix_2, *answer);
    std::cout << "Умножение оптимизированным алгоритмом виногдрада"
              << std::endl;
    std::cout << *answer;
    std::cout << std::endl;
  } catch (const std::invalid_argument &e) {
    std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
    return ERR;
  }

  return ERR_OK;
}

void print_menu() {
  std::cout << "Меню:\n";
  std::cout << "1. Умножение матриц\n";
  std::cout << "2. Замеры\n";
}

// Реализует замер производительности
int second_mode() {
  size_t start_size, end_size, n;

  // проверки, проверки, проверки, проверки, лишь бы не упало
  try {
    std::cout << "\033[1;33m" << "Размер больше 300 лучше не ставить"
              << "\033[0m" << std::endl;
    std::cout << "Введите начальный и конечный размер через пробел: ";
    start_size = input_size();
    end_size = input_size();

    std::cout << "Введите количество замеров: ";
    n = input_size();
  } catch (const std::invalid_argument &e) {
    std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
    return ERR;
  }

  if (n < 1) {
    std::cout << "\033[1;31m"
              << "ОШИБКА!!! количество замеров должно быть больше нуля!"
              << "\033[0m" << std::endl;
    return ERR;
  }

  try {
    run(start_size, end_size, n);
  } catch (const std::invalid_argument &e) {
    std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
    return ERR;
  }
  return ERR_OK;
}

int main() {
  print_menu();
  int operation;
  std::cout << "Введите номер операции: ";

  if (!(std::cin >> operation) || operation < 1 || operation > 2) {
    std::cout << "Ошибка ввода операции";
    return ERR;
  }

  if (operation == 1) {
    try {
      return first_mode();
    } catch (const std::invalid_argument &e) {
      std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
      return ERR;
    }
  } else if (operation == 2) {
    try {
      return second_mode();
    } catch (const std::invalid_argument &e) {
      std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
      return ERR;
    }
  }
  return ERR_OK;

  return 0;
}