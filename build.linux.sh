#!/usr/bin/env bash

set -e

# версия qt
export QT_SELECT=5

# имя проекта
PROJECT_NAME="avalon"

# очистка
if [ -f "Makefile" ]; then
	make clean
fi

# опции (модули) qt
QT_OPTS="network sql"
if [ "${QT_SELECT}" -eq "4" ]; then
	QT_OPTS="${QT_OPTS} webkit"
else
	QT_OPTS="${QT_OPTS} widgets webkitwidgets"
fi

# создание pro-файла
qmake -project -recursive -Wall -nopwd -o ${PROJECT_NAME}.pro \
    "CONFIG += debug_and_release" \
    "QT += ${QT_OPTS}" \
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
