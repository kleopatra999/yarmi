
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += \
	-std=c++11

INCLUDEPATH += \
	../ \
	../../../include

SOURCES += \
	main.cpp

LIBS += \
	-lboost_system

win32:LIBS += \
	-lws2_32

HEADERS += \
	 ../protocol.hpp \
	 ../../../include/yarmi/client_base.hpp \
	 ../../../include/yarmi/server.hpp \
	 ../../../include/yarmi/yarmi.hpp \
    ../../../include/yarmi/session_base.hpp \
    ../../../include/yarmi/global_context_base.hpp
