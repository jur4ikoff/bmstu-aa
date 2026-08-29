"""
Файл для генерации JSON-ки по шаблону
{
    "timestamp": "2024-07-14T19:46:32+03:00",
    "coverage": 0.1,
    "passed": 1,
    "failed": 0
}

"timestamp" - дататаймштамп в формате UTC с указанием зоны dtst=$(date +"%Y-%m-%dT%H:%M:%S%:z")
"coverage" - покрытие в процентах
"passed" - число пройденных модульных тестов при последнем тестировании
"failed" - число проваленных модульных тестов при последнем тестировании

Эх, вот бы написать это все на классах, а не в один файлик
"""


# gcovr -r .. --object-directory=. -v
import json
import subprocess
import datetime
import sys
import os
import re
import glob

GREEN = "\033[32m"
YELLOW = "\033[33m"
RED = "\033[31m"
RESET = "\033[0m"

DEBUG = 1

script_path = os.path.abspath(__file__)
script_dir = os.path.dirname(script_path)
FILE_PATH = script_dir + "/../ready/stud-unit-test-report.json"

BUILD_DIR = f"{script_dir}/build"
GCOVR_COMMAND = f"gcovr -r .. --object-directory= . -v"


def run_tests(path_to_test_exe: str):
    try:
        result = subprocess.run([path_to_test_exe], capture_output=True, text=True)

        output = result.stdout
        passed = 0
        failed = 0

        passed_match = re.findall(r"\[       OK \]", output)
        failed_match = re.search(r"\[  FAILED  \] (\d+) test", output)

        passed = len(passed_match)
        if failed_match:
            failed = int(failed_match.group(1))

        return (passed, failed, result.returncode == 0, output)

    except Exception as e:
        print(f"{RED}Ошибка, во время запуска тестов\n{RESET}")
        print(e)
        return 0, 1, False, None


def calculate_coverage(files_to_coverage: list[str]):
    try:
        result = subprocess.run(["gcovr", "-r", "..", f"--object-directory={script_dir}/build", "-v", "--print-summary"], capture_output=True, text=True)
        if result.returncode != 0:
            return 0
    
        output = result.stdout
        # print(output)

        result = {}
        pattern = r'^(\S+)\s+(\d+)\s+(\d+)\s+(\d+)%'

        for line in output.split("\n"):
            match = re.match(pattern, line.strip())

            if match:
                filename = match.group(1)
                lines = int(match.group(2))
                exec = int(match.group(3))
                coverage = int(match.group(4))
                result[filename] = {"lines" : lines, "execute_lines": exec, "coverage": coverage}
    except Exception as e:
        return 0
    
    files_count: int = 0
    coverage_sum = 0
    for file in files_to_coverage:
        is_need_file = False
        for key in result.keys():
            if file in key:
                # Эта штука нужно для того, чтобы запускать скрипт из различных директорий
                # ибо gcovr возвращает пути относительно запуска
                is_need_file = True
                file = key

        if is_need_file:
            line = result[file]
            coverage = int(line["coverage"])
            
            coverage_sum += coverage
            files_count += 1

    if files_count == 0:
        return 0
    return coverage_sum / files_count / 100 # Переводим из процентов к десятичной дроби [0, 1]

    


def main():
    if DEBUG:
        print(f"{GREEN}\n> ГЕНЕРАЦИЯ {FILE_PATH}{RESET}")
    test_executable_path: str = script_dir + "/build/tests.exe"

    if DEBUG:
        print(f"{GREEN}> ЗАПУСК ТЕСТОВ{RESET}")
    passed, failed, success, output = run_tests(test_executable_path)

    if not success or failed > 0:
        if DEBUG:
            print(f"{RED}ОШИБКА ТЕСТИРОВАНИЯ, ВЫВОД ЛОГОВ{RESET}")
            print(output)

    if DEBUG:
        print(f"{GREEN}> ЗАМЕР ПОКРЫТИЯ{RESET}")

    files_to_coverage = [f"matrix/matrix.cpp"]
    coverage = calculate_coverage(files_to_coverage)

    timestamp = datetime.datetime.now().astimezone().strftime("%Y-%m-%dT%H:%M:%S%z")
    timestamp = timestamp[:-2] + ":" + timestamp[-2:]  # Добавляем : в сдвиг время

    result = {
        "timestamp": timestamp,
        "coverage": coverage,
        "passed": passed,
        "failed": failed,
    }

    with open(FILE_PATH, "w") as f:
        json.dump(result, f, indent=4)

    if DEBUG:
        print(f"{GREEN}> \nTESTS RESULTS:{RESET}\n")
        print(json.dumps(result, indent=4))

    if success:
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
