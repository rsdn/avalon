#!/usr/bin/env bash

set -e

# имя проекта
PROJECT_NAME="avalon"

# очистка
make clean

# создание pro-файла
qmake -project -recursive -Wall -nopwd -o ${PROJECT_NAME}.pro \
    "CONFIG += debug_and_release" \
    "QT += network sql webkit" \
    "LIBS += -laspell -lz" \
    "macx { QMAKE_CXX = clang }" \
    "macx { ICON = icons/avalon.icns }" \
    src

# создание make-файлов
qmake ${PROJECT_NAME}.pro

# создание бинарника
make $1

# удаление символов
strip -s ${PROJECT_NAME}

# запуск
if [ -z "${TRAVIS}" ]; then
	./${PROJECT_NAME}
fi
