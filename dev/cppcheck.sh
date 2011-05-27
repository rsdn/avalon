#!/bin/sh

###
# $Date: 2010-12-03 03:18:48 +0300 (Птн, 03 Дек 2010) $
# $Author: antonbatenev.ya.ru $
# $Revision: 370 $
# $URL: svn://opensvn.ru/avalon/trunk/dev/cppcheck.sh $
###

###
# скрипт статической проверки кода
###

OUT_FILE="./cppcheck.log"

:> "${OUT_FILE}"

cppcheck --enable=all --inline-suppr ../ &> "${OUT_FILE}"
