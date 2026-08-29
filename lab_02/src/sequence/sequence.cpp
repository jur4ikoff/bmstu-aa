#include "sequence.h"

#include <chrono>
#include <climits>
#include <random>

Sequence::Sequence(std::initializer_list<value_type> list) {
  _set_size(list.size());
  _memory_allocate(_len);

  auto container_iter = _container.begin();
  for (auto iter = list.begin(); iter != list.end(); iter++) {
    *container_iter = *iter;
    container_iter++;
  }
}

void Sequence::read_from_console() {
  _read_size();
  _memory_allocate(_len);

  _read_elements();
}

void Sequence::print_sequence() {
  for (size_type i = 0; i < _len; i++) {
    std::cout << _container[i] << " ";
  }
  std::cout << "\n";
}

/**
 * @brief Функция находит максимальный элемент
 * @param max ссылка на максимальный элемент
 * @note Убедитесь, что max "сброшен" INT_MIN
 */
void Sequence::default_max(int &max) {
  max = INT_MIN;

  for (size_type i = 0; i < _len; i++) {
    if (_container[i] > max)
      max = _container[i];
  }
}

/**
 * @brief Функция находит максимальный элемент
 * @param max ссылка на максимальный элемент
 * @note Убедитесь, что max "сброшен" INT_MIN
 */
void Sequence::recursion_max(int &max, size_type index) {
  if (index == _len)
    return;

  if (_container[index] > max)
    max = _container[index];

  recursion_max(max, index += 1);
}

std::ostream &operator<<(std::ostream &os, const Sequence &seq) {
  for (size_type i = 0; i < seq._len; ++i) {
    os << seq._container[i];
    if (i != seq._len - 1) {
      os << " ";
    }
  }
  return os;
}

void Sequence::random_fill(size_type size, int min_value, int max_value) {
  _set_size(size);
  _memory_allocate(_len);

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);
  std::uniform_int_distribution<value_type> distribution(min_value, max_value);

  for (size_t i = 0; i < size; i++) {
    _container[i] = distribution(generator);
  }
}

/**
 * @brief заполнение по возрастанию
 */
void Sequence::straight_fill(size_t size, int min_value) {
  _set_size(size);
  _memory_allocate(_len);

  for (size_type i = 0; i < size; i++) {
    _container[i] = i + min_value;
  }
}

/**
 * @brief заполнение по возрастанию
 */
void Sequence::reverse_fill(size_t size, int max_value) {
  _set_size(size);
  _memory_allocate(_len);
  for (size_type i = 0; i < size; i++) {
    _container[i] = max_value - i;
  }
}

size_type Sequence::size() { return _len; }

void Sequence::_read_size() {
  int size = 0;
  std::cout << "Введите размер последовательности: ";

  std::cin >> size;
  if (std::cin.fail())
    throw std::invalid_argument("ОШИБКА, неверный ввод размера");

  if (size > 0)
    _len = size;
  else
    throw std::invalid_argument("ОШИБКА, размер не может быть отрицательным");
}

void Sequence::_read_elements() {
  std::cout << "Введите " << _len << " элементов через пробел: ";
  value_type cur_element;
  for (size_type i; i < _len; i++) {
    std::cin >> _container[i];

    if (std::cin.fail())
      throw std::invalid_argument(
          "ОШИБКА, неверный ввод элементов последовательности");
  }
}

void Sequence::_memory_allocate(size_type size) {
  _container = std::vector<value_type>(size);
}

void Sequence::_set_size(size_type size) {
  if (size > 0)
    _len = size;
  else
    throw std::invalid_argument("ОШИБКА, неверный параметр размера");
}