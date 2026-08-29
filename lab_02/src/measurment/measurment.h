#pragma once

#define EXP_COUNT 1000

// if DEBUG
#define DATA_PATH "./../charts/res.csv"
#define PYTHON_COMMAND "python3.10 ../build_charts.py"
// else
//     #define DATA_PATH "./charts/res.csv"
//     #define PYTHON_COMMAND "python3.10 ../build_charts.py"
#define CASE_NUMBER 3

#include <iostream>

void run(size_t start_size, size_t end_size, size_t n);