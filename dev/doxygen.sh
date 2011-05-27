#!/bin/sh

###
# $Date: 2011-04-07 11:20:38 +0400 (Чтв, 07 Апр 2011) $
# $Author: antonbatenev.ya.ru $
# $Revision: 407 $
# $URL: svn://opensvn.ru/avalon/trunk/dev/doxygen.sh $
###

###
# скрипт создания документации по проекту
###

doxygen doxygen/doxygen.conf

xdg-open file:$(pwd)/doxygen/html/index.html
