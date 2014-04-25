TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += \
	-std=c++11

INCLUDEPATH += \
	.. \
	../../../include

LIBS += \
	-lboost_system

win32:LIBS += \
	-lws2_32

SOURCES += main.cpp \
    ../../../src/client_base.cpp

HEADERS += \
	 ../protocol.hpp
