#!/bin/sh

###
# $Date: 2010-12-22 13:06:11 +0300 (Срд, 22 Дек 2010) $
# $Author: antonbatenev.ya.ru $
# $Revision: 381 $
# $URL: svn://opensvn.ru/avalon/trunk/dev/build.sh $
###

# имя проекта
PROJECT_NAME="avalon"

# переход в директорию проекта
cd ../

# очистка
make clean

# создание pro-файла
qmake -project -recursive -Wall -o ${PROJECT_NAME}.pro "CONFIG += debug_and_release" "QT += network sql webkit" "LIBS += -laspell -lz"

# создание make-файлов
qmake ${PROJECT_NAME}.pro

# создание бинарника
make $1

# удаление символов
strip -s ${PROJECT_NAME}

# запуск
./${PROJECT_NAME}
