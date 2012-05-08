#!/bin/bash

PATH_TO_DEV=../avalon.app/Contents/dev

mkdir $PATH_TO_DEV
cp -f avalon.sqlite.sql $PATH_TO_DEV

macdeployqt ../avalon.app -dmg
