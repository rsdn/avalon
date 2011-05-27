#!/bin/sh

valgrind -v --log-file=./out.log --leak-check=full --show-reachable=yes --leak-resolution=high --undef-value-errors=no --num-callers=50 ./../avalon