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

SOURCES += main.cpp

HEADERS += \
    ../protocol.hpp
