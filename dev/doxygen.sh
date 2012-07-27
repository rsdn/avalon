#!/bin/sh

###
# скрипт создания документации по проекту
###

doxygen doxygen/doxygen.conf

xdg-open file:$(pwd)/doxygen/html/index.html
