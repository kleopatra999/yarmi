
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
	-lboost_system \
	-lboost_thread

win32:LIBS += \
	-lws2_32 \
	-lmswsock

SOURCES += \
	main.cpp \
	 command_processor.cpp \
	 client_session.cpp

HEADERS += \
	 ../protocol.hpp \
	 ../../../include/yarmi/session_base.hpp \
	 ../../../include/yarmi/server.hpp \
	 ../../../include/yarmi/yarmi.hpp \
	 ../../../include/yarmi/global_context_base.hpp \
	 ../../../include/yarmi/client.hpp \
	 command_processor.hpp \
	 client_session.hpp
