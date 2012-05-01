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

# обновление даты и номера билда
AVALON_DATE=$(date "+%Y-%m-%dT%H:%M:%S%z")
AVALON_BUILD=$(awk '/AVALON_BUILD/ { print $3+1; }' version.h)
cp -f version.h version.tmp
cat version.tmp | sed "s/AVALON_BUILD .*/AVALON_BUILD ${AVALON_BUILD}/" | sed "s/AVALON_DATE \".*\"/AVALON_DATE \"${AVALON_DATE}\"/" > version.h
rm -f version.tmp

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
