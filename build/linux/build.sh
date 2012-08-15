#!/bin/sh

# имя проекта
PROJECT_NAME="avalon"

# переход в директорию проекта
cd ../../

# очистка
make clean

# создание pro-файла
qmake -project -recursive -Wall -nopwd -o ${PROJECT_NAME}.pro \
    "CONFIG += debug_and_release" \
    "QT += network sql webkit" \
    "LIBS += -laspell -lz" \
    "ICON = icons/Main-Logo.icns" \
    "macx { QMAKE_CXX = clang }" \
    src

# создание make-файлов
qmake ${PROJECT_NAME}.pro

# создание бинарника
make $1

# удаление символов
strip -s ${PROJECT_NAME}

# запуск
./${PROJECT_NAME}
