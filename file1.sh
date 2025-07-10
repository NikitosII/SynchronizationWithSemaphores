#!/bin/bash

SOURCE_FILE="file1.c"
EXECUTABLE="file1"

gcc -o $EXECUTABLE $SOURCE_FILE -lpthread

if [ $? -eq 0 ]; then
    echo "Компиляция программы 1 успешно завершена."
else
    echo "Ошибка компиляции программы 1."
    exit 1
fi

./$EXECUTABLE

if [ $? -eq 0 ]; then
    echo "Программа 1 завершена."
else
    echo "Программа 1 завершилась с ошибкой."
fi
