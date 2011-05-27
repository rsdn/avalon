@ECHO OFF

REM *********************************************************
REM * $Date: 2010-02-28 20:40:21 +0300 #$
REM * $Author: antonbatenev.ya.ru $
REM * $Revision: 317 $
REM * $URL: svn://svn.opensvn.ru/avalon/trunk/dev/build.bat $
REM *********************************************************

REM ***************
REM * им€ проекта *
REM ***************
SET PROJECT_NAME=avalon

REM **************************
REM * исполн€емый файл qmake *
REM **************************
SET QT_QMAKE=C:\Qt\4.6.3\bin\qmake.exe

REM **************************************************
REM * путь к подключаемым файлам и библиотеке aspell *
REM * домашн€€ страница: http://aspell.net/win32/    *
REM **************************************************
SET ASPELL_INC=C:\projects\aspell\include
SET ASPELL_LIB=C:\projects\aspell\lib\aspell-15.lib

REM *****************************************************************
REM * путь к подключаемым файлам и библиотеке zlib (статика)        *
REM * домашн€€ страница: http://www.winimage.com/zLibDll/index.html *
REM *****************************************************************
SET ZLIB_LIB=C:\projects\zlib\lib\static32\zlibstat.lib
SET ZLIB_INC=C:\projects\zlib\include

REM *******************************
REM * создание проекта и makefile *
REM *******************************
cd ../
%QT_QMAKE% -project -recursive -Wall -o %PROJECT_NAME%.pro "CONFIG += release" "QT += network sql webkit" "INCLUDEPATH += %ASPELL_INC% %ZLIB_INC%" "LIBS += %ASPELL_LIB% %ZLIB_LIB%" "RC_FILE += resource.rc"
%QT_QMAKE% %PROJECT_NAME%.pro

REM ******************************************
REM * сборка (консоль должна быть запущена в *
REM * Visual Studio 2005 Command Prompt)     *
REM ******************************************
nmake.exe
