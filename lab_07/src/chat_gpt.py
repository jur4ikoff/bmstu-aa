import os
from PyPDF2 import PdfReader


def is_cyrillic_letter(ch):
    return 'А' <= ch <= 'Я' or 'а' <= ch <= 'я' or ch == 'Ё' or ch == 'ё'


def check_pdf_for_invalid_cyrillic_tokens(pdf_path):
    reader = PdfReader(pdf_path)
    results = []

    for page_number, page in enumerate(reader.pages, start=1):
        text = page.extract_text()
        if not text:
            continue

        text = text.replace('\u00A0', ' ')
        lines = text.splitlines()

        for line_number, line in enumerate(lines, start=1):
            tokens = line.split()
            for token in tokens:
                has_cyrillic = False
                has_foreign_letter = False

                for ch in token:
                    if ch.isalpha():
                        if is_cyrillic_letter(ch):
                            has_cyrillic = True
                        else:
                            has_foreign_letter = True

                if has_cyrillic and has_foreign_letter:
                    results.append((token, page_number, line_number))

    if results:
        return True, results
    return False, []


def process_all_pdfs_in_directory(directory):
    reports = {}
    for root, _, files in os.walk(directory):
        for file in files:
            if file.lower().endswith(".pdf"):
                path = os.path.join(root, file)
                reports[path] = check_pdf_for_invalid_cyrillic_tokens(path)
    return reports


def print_report(reports):
    for pdf_path, (exists, entries) in reports.items():
        print("=" * 60)
        print(f"Файл: {os.path.basename(pdf_path)}")
        if not exists:
            print("Нарушения не обнаружены")
        else:
            print("Обнаружены некорректные кириллические токены:")
            for token, page, line in entries:
                print(f"  Страница {page}, строка {line}: {token}")
        print()


if __name__ == "__main__":
    reports = process_all_pdfs_in_directory("files")
    print_report(reports)
