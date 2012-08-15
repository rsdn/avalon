TEMPLATE     = app
TARGET       = avalon_test
CONFIG      += debug_and_release warn_on
LIBS        += -lcppunit
QT          += network sql webkit
DEPENDPATH  += .
INCLUDEPATH += .

build_pass:CONFIG(debug, debug|release) {
	DESTDIR = debug
}

build_pass:CONFIG(release, debug|release) {
	DESTDIR = release
}

HEADERS += parser_test.h
SOURCES += parser_test.cpp ../../src/parser.cpp
