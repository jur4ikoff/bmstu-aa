BUILD_DIR = ./src/build
CMAKE = cmake
MAKE = make
# Обязательно в реализации
# - Цели НЕ менять
# - Зависимости и сценарии поменять на необходимые

# Рекомендуется проект выстраивать вокруг собственных сценариев сборки
# Рекомендуется в этом файле оставить только свои вызовы "make build",
# "cmake .", "msBuild proj1.sln ./out" и операции копирования

# Сценарий интерфейса специально не носит название "makefile" -
# Вы можете отсюда обращаться к своим сценариям

# Цели и зависимости в этом и в своих makefile и его аналогах остаются
# под ответственностью студента.

# Никто специально не будет искать и ругаться на Вас, если у Вас,
# например, перекомпилируется весь проект при изменении одного модуля
# (преступное неиспользование возможностей инкрементальной сборки).
# Но если, всё-таки, преподаватели заметят грязь - то спросят.

ready/report.pdf: report/report.pdf
	mkdir -p ./ready
	cp report/report.pdf ready/report.pdf

# Реализация по желанию - удалить цели, если нет реализации
# UPD 2025-2026: так как из доступных остались только Си, Си++ и Раст,
# которые имеют популярные библиотеки для модульного тестирования,
# то модульное тестирование обязательно должно проводиться на сервере
#
# Пример содержимого:
#
# { 
#     "timestamp": "2024-07-14T19:46:32+03:00",
#     "coverage": 0.1,
#     "passed": 1,
#     "failed": 0
# }
#
# "timestamp" - дататаймштамп в формате UTC с указанием зоны dtst=$(date +"%Y-%m-%dT%H:%M:%S%:z")
# "coverage" - покрытие в процентах
# "passed" - число пройденных модульных тестов при последнем тестировании
# "failed" - число проваленных модульных тестов при последнем тестировании
#
# Сборка и запуск модульных тестов прямо на сервере
ready/stud-unit-test-report.json:
	@mkdir ready 2>/dev/null || true
	@mkdir $(BUILD_DIR) 2>/dev/null || true
	@cd $(BUILD_DIR) && $(CMAKE) -DENABLE_COVERAGE=ON ..
	@cd $(BUILD_DIR) && $(MAKE)
	python3 src/test_runner.py

# Очистка
.PHONY: clean
clean:
	rm -rf ready
	rm -rf src/build
	rm -rf src/charts
	rm -rf src/data
	rm -rf report/*.log
	rm -rf report/*.aux
	rm -rf report/*.bbl
	rm -rf report/*.blg
	rm -rf report/*.out
	rm -rf report/*.gz
	rm -rf report/*.toc
	echo OK

build:
	@mkdir -p $(BUILD_DIR) 2>/dev/null || true
	@cd $(BUILD_DIR) && $(CMAKE) ..
	@cd $(BUILD_DIR) && $(MAKE)

build_asan:
	@mkdir -p $(BUILD_DIR) 2>/dev/null || true
	@cd $(BUILD_DIR) && $(CMAKE) -DENABLE_ASAN=ON ..
	@cd $(BUILD_DIR) && $(MAKE)

build_ubsan:
	@mkdir -p $(BUILD_DIR) 2>/dev/null || true
	@cd $(BUILD_DIR) && $(CMAKE) -DENABLE_UBSAN=ON ..
	@cd $(BUILD_DIR) && $(MAKE)

# Пересборка (очистка + сборка)
rebuild: clean build

# Запуск проекта (предполагаем, что исполняемый файл имеет то же имя, что и папка проекта)
run: build
	@mkdir -p src/data 2>/dev/null || true
	@cd $(BUILD_DIR) && ./app.exe

# Настройки форматтера
CLANG_FORMAT = clang-format
STYLE = LLVM
SOURCE_EXTENSIONS = *.cpp *.h *.hpp *.cc *.cxx *.hxx
SOURCE_DIRS = src include lib test

# Форматирование всех исходных файлов
format:
	@echo "Formatting source files with $(CLANG_FORMAT) ($(STYLE) style)..."
	@for dir in $(SOURCE_DIRS); do \
		if [ -d "$$dir" ]; then \
			echo "Formatting files in $$dir/"; \
			find "$$dir" -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.hxx" \) -exec $(CLANG_FORMAT) -i -style=$(STYLE) {} \; ; \
		fi \
	done
	@echo "✅ Formatting completed."

tests:
	@echo "Starting tests"
	@mkdir -p $(BUILD_DIR) 2>/dev/null || true
	@cd $(BUILD_DIR) && $(CMAKE) ..
	@cd $(BUILD_DIR) && $(MAKE)
	@cd $(BUILD_DIR) && ./tests.exe
