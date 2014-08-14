
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += \
	-std=c++1y

LIBS += \
	-lboost_system

win32:LIBS += \
	-lws2_32 \
	-lmswsock

INCLUDEPATH += \
	../ \
	../../../include

SOURCES += \
	main.cpp \
	../../../src/client_base.cpp \
    client_first_invoker.cpp \
    client_second_invoker.cpp \
    client.cpp

HEADERS += \
    ../first_proto.hpp \
    ../second_proto.hpp \
    client_first_invoker.hpp \
    client_second_invoker.hpp \
    client.hpp
