
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += \
	-std=c++11

LIBS += \
	-lboost_system \
	-lboost_filesystem

win32:LIBS += \
	-lws2_32

INCLUDEPATH += \
	.. \
	../../../include

SOURCES += main.cpp \
	user_context.cpp \
	 ../../../src/global_context_base.cpp \
	 ../../../src/session_base.cpp

HEADERS += \
	user_context.hpp \
	global_context.hpp \
	../protocol.hpp
