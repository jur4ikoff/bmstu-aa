#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Использование: $0 путь_к_pdf"
    exit 1
fi

input_pdf="$1"

# Проверка, что файл существует и является обычным файлом
if [ ! -f "$input_pdf" ]; then
    echo "Ошибка: файл '$input_pdf' не найден или это не файл"
    exit 2
fi

# Проверка расширения файла .pdf (регистр не важен)
if [[ ! "$input_pdf" =~ \.pdf$|\.PDF$ ]]; then
    echo "Ошибка: файл $input_pdf имеет расширение не .pdf"
    exit 3
fi

temp_pdf="${input_pdf%.pdf}_cleaned.pdf"

# gs -sDEVICE=pdfwrite \
#    -dCompatibilityLevel=1.4 \
#    -dNOPAUSE \
#    -dOptimize=true \
#    -dQUIET \
#    -dBATCH \
#    -dRemoveUnusedFonts=true \
#    -dRemoveUnusedImages=true \
#    -dOptimizeResources=true \
#    -dDetectDuplicateImages \
#    -dCompressFonts=true \
#    -dEmbedAllFonts=true \
#    -dSubsetFonts=true \
#    -dPreserveAnnots=true \
#    -dPreserveMarkedContent=true \
#    -dPreserveOverprintSettings=true \
#    -dPreserveHalftoneInfo=true \
#    -dPreserveOPIComments=true \
#    -dPreserveDeviceN=true \
#    -dMaxInlineImageSize=0 \
#    -sOutputFile="$temp_pdf" \
#    "$input_pdf"

# if [ $? -ne 0 ]; then
#     echo "Ошибка очистки файла"
#     rm -f "$temp_pdf"
#     exit 2
# fi

if ! gs -sDEVICE=pdfwrite \
        -dCompatibilityLevel=1.4 \
        -dNOPAUSE \
        -dOptimize=true \
        -dQUIET \
        -dBATCH \
        -dRemoveUnusedFonts=true \
        -dRemoveUnusedImages=true \
        -dOptimizeResources=true \
        -dDetectDuplicateImages \
        -dCompressFonts=true \
        -dEmbedAllFonts=true \
        -dSubsetFonts=true \
        -dPreserveAnnots=true \
        -dPreserveMarkedContent=true \
        -dPreserveOverprintSettings=true \
        -dPreserveHalftoneInfo=true \
        -dPreserveOPIComments=true \
        -dPreserveDeviceN=true \
        -dMaxInlineImageSize=0 \
        -sOutputFile="$temp_pdf" \
        "$input_pdf"; then
    echo "Ошибка очистки файла" >&2
    rm -f "$temp_pdf"
    exit 2
fi


orig_size=$(stat -f%z "$input_pdf")
clean_size=$(stat -f%z "$temp_pdf")

# Проверяем разницу в размере (очищенный должен быть меньше на 10% или больше)
threshold=$(( orig_size * 90 / 100 ))
echo $threshold

if [ "$clean_size" -lt "$threshold" ]; then
    mv "$temp_pdf" "$input_pdf"
    echo "Очищен: $input_pdf"
else
    echo "Недостаточное сжатие; нетронут: $input_pdf"
    rm -f "$temp_pdf"
fi