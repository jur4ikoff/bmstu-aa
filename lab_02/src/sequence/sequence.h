#include "constants.h"
#include <iostream>
#include <vector>

class Sequence {
public:
  Sequence() : _len(0) {}
  Sequence(std::initializer_list<value_type> list);

  friend std::ostream &operator<<(std::ostream &os, const Sequence &seq);

  void read_from_console();
  void print_sequence();

  void random_fill(size_type size, int min_value, int max_value);
  void straight_fill(size_t size, int min_value);
  void reverse_fill(size_t size, int max_value);

  void default_max(int &max);
  void recursion_max(int &max, size_type index);

  size_type size();

private:
  size_type _len;
  std::vector<value_type> _container;

  void _read_size();
  void _read_elements();

  void _set_size(size_type size);
  void _memory_allocate(size_type size);
};
