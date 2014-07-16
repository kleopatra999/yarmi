TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += \
	-std=c++11

INCLUDEPATH += \
	../include

LIBS += \
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
	dump_info.cpp \
	parser.cpp \
	type_id.cpp \
	records.cpp \
    checks.cpp

HEADERS += \
	 cmdline.hpp \
	 options.hpp \
	tools.hpp \
	c_generator.hpp \
	cpp_generator.hpp \
	java_generator.hpp \
	js_generator.hpp \
	python_generator.hpp \
	generator_base.hpp \
	protoinfo.hpp \
	dump_info.hpp \
	cursor.hpp \
	parser.hpp \
	type_id.hpp \
	tokens.hpp \
	records.hpp \
    visitors.hpp \
    checks.hpp

OTHER_FILES += \
	test/protocol.yarmi

