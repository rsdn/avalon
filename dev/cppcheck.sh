#!/bin/sh

###
# скрипт статической проверки кода
###

OUT_FILE="./cppcheck.log"

:> "${OUT_FILE}"

cppcheck --enable=all --inline-suppr ../ &> "${OUT_FILE}"
