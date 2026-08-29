#pragma once

#define EXP_COUNT 100

#define DATA_PATH "../data/speed.csv"
#define PYTHON_COMMAND "python3.10 ../build_charts.py"

#include <iostream>

void run(size_t start_size, size_t end_size, size_t n);