# makefile for run
BUILD_DIR = ./src/build
CMAKE = cmake
MAKE = make
MAIN = report
LATEX = pdflatex

REPORT_SOURCE_PATH = ./report
REPORT_FINAL_PATH = ready/report.pdf


report/report.pdf: $(REPORT_SOURCE_PATH)/$(MAIN).tex
	cd $(REPORT_SOURCE_PATH) && $(LATEX) $(MAIN) > /dev/null
	cd $(REPORT_SOURCE_PATH) && $(LATEX) $(MAIN) > /dev/null
	$(MAKE) -f iu7-algan-cicd-student.mk compress


ready/report.pdf: report/report.pdf
	mkdir -p ./ready
	cp report/report.pdf $(REPORT_FINAL_PATH)

ready/stud-unit-test-report.json:
	@mkdir ready 2>/dev/null || true
	python3 src/test_runner.py
# 	@mkdir $(BUILD_DIR) 2>/dev/null || true
# 	@cd $(BUILD_DIR) && $(CMAKE) -DENABLE_COVERAGE=ON ..
# 	@cd $(BUILD_DIR) && $(MAKE)
	

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
	rm -f report/*.pdf
	echo OK


down:
	docker-compose down

stop:
	docker-compose stop

init:
	docker-compose up -d
	./src/dev/after_startup.sh
	./src/dev/init.sh

generate_rasp:
	python -m src.dev.generate_rasp

up:
	docker-compose up -d
	./src/dev/after_startup.sh