#!/bin/sh

###
# $Date: 2009-03-28 10:11:23 +0300 (Сбт, 28 Мар 2009) $
# $Author: antonbatenev.ya.ru $
# $Revision: 193 $
# $URL: svn://opensvn.ru/avalon/test/zip_raito/build.sh $
###

function clean {

	echo "удаление временных файлов"

	rm -f *.o
	rm -f moc_*.cpp
	rm -f qrc_*.cpp
	rm -f Makefile
	rm -f ${PROJECT_NAME}.pro
	rm -f debug.out
	rm -f release.out
}

# путь до Qt4.4 без слеша в конце
QT_PATH="/usr/lib/qt4"

# имя проекта
PROJECT_NAME="test"

# удаление старой копии
rm -f ${PROJECT_NAME}

if [ "x$1" == "xclean" ]
then
	# удаление временных файлов
	clean
	exit 0
fi

# сборка
if [ "x$1" == "xdebug" ]
then

	if [ -f "release.out" ]
	then
		clean
	fi

	${QT_PATH}/bin/qmake -project -recursive -Wall -o ${PROJECT_NAME}.pro "QT += network sql webkit" "CONFIG += debug console"

	touch "debug.out"

else

	if [ -f "debug.out" ]
	then
		clean
	fi

	${QT_PATH}/bin/qmake -project -recursive -Wall -o ${PROJECT_NAME}.pro "QT += network sql webkit" "CONFIG += console"

	touch "release.out"

fi

${QT_PATH}/bin/qmake ${PROJECT_NAME}.pro

make

# запуск
./${PROJECT_NAME}
