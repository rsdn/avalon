#!/bin/sh

###
# $Date: 2010-12-22 13:06:11 +0300 (Срд, 22 Дек 2010) $
# $Author: antonbatenev.ya.ru $
# $Revision: 381 $
# $URL: svn://opensvn.ru/avalon/trunk/dev/build.sh $
###

# путь до Qt4.4 без слеша в конце
QT_PATH="/usr/lib/qt4"

# имя проекта
PROJECT_NAME="avalon"

# переход в директорию проекта
cd ../

# обновление даты билда
cp -f version.h version.tpl
sed "s/AVALON_DATE \".*\"/AVALON_DATE \"$(date)\"/" version.tpl > version.h
rm -f version.tpl

# очистка
make clean

# создание pro-файла
${QT_PATH}/bin/qmake -project -recursive -Wall -o ${PROJECT_NAME}.pro "CONFIG += debug_and_release" "QT += network sql webkit" "LIBS += -laspell"

# создание make-файлов
${QT_PATH}/bin/qmake ${PROJECT_NAME}.pro

# создание бинарника
make $1

# удаление символов
strip -s ${PROJECT_NAME}

# запуск
./${PROJECT_NAME}
