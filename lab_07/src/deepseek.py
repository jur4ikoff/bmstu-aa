import os
import re
import sys
from typing import List, Tuple
import PyPDF2


def find_non_cyrillic_in_tokens(pdf_path: str) -> Tuple[bool, List[Tuple[str, Tuple[int, int]]]]:
    """
    Функция для поиска некириллических символов внутри кириллических токенов в PDF-файле.
    
    Args:
        pdf_path: Путь к PDF-файлу
        
    Returns:
        Кортеж: (найдены_ли_совпадения, список_совпадений)
        Каждое совпадение: (найденная_строка, (страница, номер_строки))
    """
    
    # Регулярное выражение для поиска кириллических токенов, содержащих некириллические символы
    # Кириллический токен должен содержать хотя бы одну кириллическую букву
    pattern = re.compile(
        r'''
        (?:^|\s)                    # Начало строки или пробел перед токеном
        (                           # Начало группы токена
          (?:                       # Неcapturing группа для начала токена
            [а-яА-ЯёЁ]+             # Должна быть хотя бы одна кириллическая буква
            |                       # ИЛИ
            [а-яА-ЯёЁ][0-9\-.,:;!?()"'%]*  # Кириллическая буква + цифры/знаки
          )
          [^а-яА-ЯёЁ0-9\s\-.,:;!?()"'%]  # НЕкириллический символ (то, что мы ищем)
          .*?                       # Любые символы после (ленивое совпадение)
        )                           # Конец группы токена
        (?=\s|$)                    # Пробел после токена или конец строки
        ''', 
        re.VERBOSE | re.UNICODE
    )
    
    # Альтернативное регулярное выражение:
    # Ищем токены, которые начинаются с кириллических символов и содержат некириллические
    pattern2 = re.compile(
        r'''
        (?:^|\s)                    # Начало строки или пробел
        (                           # Группа токена
          [а-яА-ЯёЁ]                # Начинается с кириллической буквы
          [а-яА-ЯёЁ0-9\-.,:;!?()"'%]*  # Продолжение кириллическими символами/цифрами/знаками
          [^а-яА-ЯёЁ0-9\s\-.,:;!?()"'%]  # Найден некириллический символ!
          [а-яА-ЯёЁ0-9\-.,:;!?()"'%]*  # Может продолжаться дальше
        )
        (?=\s|$)                    # Конец токена
        ''',
        re.VERBOSE | re.UNICODE
    )
    
    # Третья попытка: более простое и понятное решение
    # 1. Сначала находим все потенциальные токены (разделенные пробелами)
    # 2. Потом проверяем каждый токен отдельно
    simple_pattern = re.compile(
        r'''
        (?:^|\s)                    # Начало строки или пробел
        (                           # Группа токена
          [^\s]+                    # Любые не-пробельные символы
        )
        (?=\s|$)                    # Конец строки или пробел
        ''',
        re.VERBOSE | re.UNICODE
    )
    
    results = []
    
    try:
        with open(pdf_path, 'rb') as file:
            pdf_reader = PyPDF2.PdfReader(file)
            
            for page_num in range(len(pdf_reader.pages)):
                page = pdf_reader.pages[page_num]
                page_text = page.extract_text()
                
                if page_text:
                    # Разбиваем текст страницы на строки
                    lines = page_text.split('\n')
                    
                    for line_num, line in enumerate(lines, 1):
                        # Ищем все потенциальные токены в строке
                        potential_tokens = simple_pattern.findall(line)
                        
                        for token in potential_tokens:
                            # Проверяем, является ли токен кириллическим
                            # Кириллический токен должен содержать хотя бы одну кириллическую букву
                            has_cyrillic = bool(re.search(r'[а-яА-ЯёЁ]', token))
                            
                            if has_cyrillic:
                                # Проверяем, содержит ли токен некириллические символы
                                # (кроме разрешенных: цифр и знаков препинания)
                                has_non_cyrillic = bool(
                                    re.search(
                                        r'[^а-яА-ЯёЁ0-9\s\-.,:;!?()"'']',
                                        token
                                    )
                                )
                                
                                if has_non_cyrillic:
                                    # Дополнительная проверка: токен должен быть достаточно "кириллическим"
                                    # Считаем процент кириллических символов
                                    cyrillic_chars = re.findall(r'[а-яА-ЯёЁ]', token)
                                    total_chars = len(token)
                                    
                                    # Если хотя бы 50% символов - кириллические, считаем токен кириллическим
                                    if len(cyrillic_chars) / total_chars >= 0.3:  # 30% минимум
                                        results.append((token, (page_num + 1, line_num)))
    
    except Exception as e:
        print(f"Ошибка при обработке файла {pdf_path}: {e}")
        return False, []
    
    found = len(results) > 0
    return found, results


def process_pdf_files_in_directory(directory_path: str = "files") -> None:
    """
    Функция для поиска всех PDF-файлов в директории и их обработки.
    
    Args:
        directory_path: Путь к директории с PDF-файлами (по умолчанию "files")
    """
    
    print("Программа для поиска некириллических символов в кириллических токенах")
    print("=" * 60)
    
    # Проверяем существование директории
    if not os.path.exists(directory_path):
        print(f"Ошибка: Директория '{directory_path}' не найдена.")
        print("Создайте директорию 'files' и поместите в нее PDF-файлы для обработки.")
        return
    
    # Ищем все PDF-файлы в директории
    pdf_files = []
    for file in os.listdir(directory_path):
        if file.lower().endswith('.pdf'):
            pdf_files.append(os.path.join(directory_path, file))
    
    if not pdf_files:
        print(f"В директории '{directory_path}' не найдено PDF-файлов.")
        print("Поместите PDF-файлы в директорию 'files'.")
        return
    
    print(f"Найдено {len(pdf_files)} PDF-файлов для обработки:")
    for i, file in enumerate(pdf_files, 1):
        print(f"  {i}. {os.path.basename(file)}")
    print("=" * 60)
    
    # Обрабатываем каждый файл
    all_results = []
    for pdf_file in pdf_files:
        filename = os.path.basename(pdf_file)
        print(f"\nОбработка файла: {filename}")
        print("-" * 40)
        
        found, matches = find_non_cyrillic_in_tokens(pdf_file)
        
        if found:
            print(f"✓ Найдены кириллические токены с некириллическими символами: {len(matches)} шт.")
            for match, (page, line) in matches:
                print(f"  - '{match}' (страница {page}, строка {line})")
            all_results.append((filename, matches))
        else:
            print("✓ Кириллических токенов с некириллическими символами не найдено.")
    
    # Итоговый отчет
    print("\n" + "=" * 60)
    print("ИТОГОВЫЙ ОТЧЕТ")
    print("=" * 60)
    
    if all_results:
        total_tokens = sum(len(matches) for _, matches in all_results)
        print(f"Всего найдено кириллических токенов с некириллическими символами: {total_tokens}")
        print("Файлы с найденными токенами:")
        for filename, matches in all_results:
            print(f"  - {filename}: {len(matches)} токенов")
    else:
        print("Во всех файлах кириллические токены с некириллическими символами отсутствуют.")
    
    print("\nОбработка завершена.")


def main():
    """
    Основная функция программы.
    Запускает обработку всех PDF-файлов в директории 'files'.
    """
    process_pdf_files_in_directory()


if __name__ == "__main__":
    main()