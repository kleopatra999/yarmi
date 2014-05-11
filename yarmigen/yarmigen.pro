TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += \
	-std=c++11

LIBS += \
	-lboost_system \
	-lboost_filesystem \
	-lboost_program_options

SOURCES += main.cpp \
	 cmdline.cpp \
    tools.cpp \
    c_generator.cpp \
    cpp_generator.cpp \
    java_generator.cpp \
    js_generator.cpp \
    python_generator.cpp \
    protoinfo.cpp \
    reader.cpp \
    dump_info.cpp

HEADERS += \
	 cmdline.hpp \
	 options.hpp \
	 throw.hpp \
    tools.hpp \
    c_generator.hpp \
    cpp_generator.hpp \
    java_generator.hpp \
    js_generator.hpp \
    python_generator.hpp \
    generator_base.hpp \
    protoinfo.hpp \
    reader.hpp \
    dump_info.hpp

