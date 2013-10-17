
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
	main.cpp \
	user_context.cpp \
    ../../../src/session_base.cpp \
    ../../../src/global_context_base.cpp

LIBS += \
	-lboost_system

win32:LIBS += \
	-lws2_32 \
	-lmswsock

HEADERS += \
	 ../protocol.hpp \
	 user_context.hpp \
	 global_context.hpp \
	 ../../../include/yarmi/session_base.hpp \
	 ../../../include/yarmi/server.hpp \
	 ../../../include/yarmi/global_context_base.hpp \
	 ../../../include/yarmi/client.hpp \
	 ../protocol.hpp \
	 ../../../include/yarmi/config.hpp \
    ../../../include/yarmi/yarmi.hpp
