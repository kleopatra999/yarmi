
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += \
	-std=c++11

INCLUDEPATH += \
	../ \
	../../../include

LIBS += \
	-lboost_system

win32:LIBS += \
	-lws2_32 \
	-lmswsock

SOURCES += \
	main.cpp \
	 user_context.cpp \
	 global_context.cpp \
    ../../../src/session_base.cpp \
    ../../../src/global_context_base.cpp

HEADERS += \
	 user_context.hpp \
	 global_context.hpp \
	 ../protocol.hpp \
	 ../../../include/yarmi/session_base.hpp \
	 ../../../include/yarmi/server.hpp \
	 ../../../include/yarmi/yarmi.hpp \
	 ../../../include/yarmi/global_context_base.hpp \
	 ../../../include/yarmi/client.hpp
