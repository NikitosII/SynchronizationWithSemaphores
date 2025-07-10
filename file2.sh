#!/bin/bash

SOURCE_FILE="file2.c"
EXECUTABLE="file2"

gcc -o $EXECUTABLE $SOURCE_FILE -lpthread

if [ $? -eq 0 ]; then
    echo "Компиляция программы 2 успешно завершена."
else
    echo "Ошибка компиляции программы 2."
    exit 1
fi

./$EXECUTABLE

if [ $? -eq 0 ]; then
    echo "Программа 2 завершена."
else
    echo "Программа 2 завершилась с ошибкой."
fi
