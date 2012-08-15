#!/bin/sh

qmake avalon_test.pro
make clean
make debug
debug/avalon_test
