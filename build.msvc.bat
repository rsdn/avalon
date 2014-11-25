@ECHO OFF

REM ***************
REM * им€ проекта *
REM ***************
SET PROJECT_NAME=avalon

REM **************************
REM * исполн€емый файл qmake *
REM **************************
SET QT_QMAKE=c:\projects\qt\bin\qmake.exe

REM **************************************************
REM * путь к подключаемым файлам и библиотеке aspell *
REM * домашн€€ страница: http://aspell.net/win32/    *
REM **************************************************
SET ASPELL_INC=c:\projects\aspell\include
SET ASPELL_LIB=c:\projects\aspell\lib\aspell-15.lib

REM *****************************************************************
REM * путь к подключаемым файлам и библиотеке zlib (статика)        *
REM * домашн€€ страница: http://www.winimage.com/zLibDll/index.html *
REM *****************************************************************
SET ZLIB_LIB=c:\projects\zlib\lib\static32\zlibstat.lib
SET ZLIB_INC=c:\projects\zlib\include

REM *******************************
REM * создание проекта и makefile *
REM *******************************
%QT_QMAKE% -project -recursive -Wall -nopwd -o %PROJECT_NAME%.pro "CONFIG += release" "QT += network sql webkit" "INCLUDEPATH += %ASPELL_INC% %ZLIB_INC% src" "LIBS += %ASPELL_LIB% %ZLIB_LIB%" "RC_FILE += src/resource.rc" src
%QT_QMAKE% %PROJECT_NAME%.pro

REM ******************************************
REM * сборка (консоль должна быть запущена в *
REM * Visual Studio 2005 Command Prompt)     *
REM ******************************************
nmake.exe
