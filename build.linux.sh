#!/bin/sh

# имя проекта
PROJECT_NAME="avalon"

# очистка
make clean

# выбор компилятора
QMAKE_OPTS=""
if [ -n "${TRAVIS}" -a -n "${CC}" ]; then
	QMAKE_OPTS="${QMAKE_OPTS} QMAKE_CC=\"${CC}\""
	QMAKE_OPTS="${QMAKE_OPTS} QMAKE_CXX=\"${CC}\""
fi

# создание pro-файла
qmake -project -recursive -Wall -nopwd -o ${PROJECT_NAME}.pro ${QMAKE_OPTS} \
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
