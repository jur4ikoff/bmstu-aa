import os
import re
from pathlib import Path
from PyPDF2 import PdfReader
from PyPDF2.errors import PdfReadError


def find_non_cyrillic_in_cyrillic_tokens(pdf_path):
    if not os.path.isfile(pdf_path):
        return (False, [])
    
    try:
        reader = PdfReader(pdf_path)
    except Exception:
        return (False, [])
    
    results = []
    # Паттерн для извлечения потенциальных "слов":
    # буквы (любые), цифры, дефис, апострофы (включая Unicode ’)
    word_pattern = re.compile(r"[a-zA-Zа-яА-ЯёЁ0-9\-\'\u2019]+")

    for page_num, page in enumerate(reader.pages, start=1):
        try:
            text = page.extract_text()
        except Exception:
            continue
        if not text:
            continue
        lines = text.split('\n')
        for line_num, line in enumerate(lines, start=1):
            tokens = word_pattern.findall(line)
            for token in tokens:
                if re.search(r'[а-яА-ЯёЁ]', token):  # есть кириллица?
                    # Проверяем: есть ли символы вне разрешённого кириллического набора?
                    if re.search(r'[^а-яА-ЯёЁ0-9\-\'\u2019]', token):
                        results.append((token, (page_num, line_num)))
                        break
    return (len(results) > 0, results)


def process_all_pdfs_in_directory(directory="files"):
    results_dict = {}
    dir_path = Path(directory)
    if not dir_path.exists() or not dir_path.is_dir():
        return results_dict
    for pdf_file in sorted(dir_path.glob("*.pdf")):
        pdf_path_str = str(pdf_file)
        result = find_non_cyrillic_in_cyrillic_tokens(pdf_path_str)
        results_dict[pdf_path_str] = result
    return results_dict


def main():
    result = process_all_pdfs_in_directory()
    print("\n📊 РЕЗУЛЬТАТЫ АНАЛИЗА PDF-ФАЙЛОВ\n" + "="*50)
    if not result:
        print("📭 Нет PDF-файлов в директории 'files'.")
        return

    any_found = False
    for pdf_path, (found, matches) in result.items():
        print(f"\n📄 {os.path.basename(pdf_path)}")
        if found:
            any_found = True
            for token, (page, line) in matches:
                print(f"   • '{token}' — стр. {page}, строка {line}")
        else:
            print("   ✔ Некириллических символов в кириллических словах не найдено.")
    
    if not any_found:
        print("\n✅ Все файлы соответствуют требованиям.")
    else:
        print("\n❗ Обнаружены кириллические токены с некириллическими символами.")


if __name__ == "__main__":
    main()