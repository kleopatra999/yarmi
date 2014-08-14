QT       += core
QT       -= gui

TARGET = qt-client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += \
	-std=c++11

SOURCES += main.cpp \
	../../../src/qt_client_base.cpp

HEADERS += \
	../../../include/yarmi/client_base.hpp \
	../../../include/yarmi/fnv1a.hpp \
	../../../include/yarmi/generate_callers.hpp \
	../../../include/yarmi/generate_enum.hpp \
	../../../include/yarmi/generate_invoker.hpp \
	../../../include/yarmi/generate_invokers_sfinae.hpp \
	../../../include/yarmi/generate_lazy_if.hpp \
	../../../include/yarmi/generate_metacode.hpp \
	../../../include/yarmi/generate_ns_to_string.hpp \
	../../../include/yarmi/generate_ns.hpp \
	../../../include/yarmi/generate_proc_helper.hpp \
	../../../include/yarmi/generate_struct.hpp \
	../../../include/yarmi/generate_tuple_is_empty.hpp \
	../../../include/yarmi/global_context_base.hpp \
	../../../include/yarmi/handler_allocator.hpp \
	../../../include/yarmi/make_preallocated_handler.hpp \
	../../../include/yarmi/os_resources.hpp \
	../../../include/yarmi/preallocated_handler_invoker.hpp \
	../../../include/yarmi/qt_client_base.hpp \
	../../../include/yarmi/serialization.hpp \
	../../../include/yarmi/server_base.hpp \
	../../../include/yarmi/server_statistic.hpp \
	../../../include/yarmi/server.hpp \
	../../../include/yarmi/session_base.hpp \
	../../../include/yarmi/throw.hpp \
	../../../include/yarmi/yarmi.hpp \
	../../../include/yarmi/generate_struct/decorators.hpp \
	../../../include/yarmi/generate_struct/jsonify.hpp \
	../../../include/yarmi/generate_struct/list.hpp \
	../../../include/yarmi/generate_struct/map.hpp \
	../../../include/yarmi/generate_struct/pair.hpp \
	../../../include/yarmi/generate_struct/set.hpp \
	../../../include/yarmi/generate_struct/tuple.hpp \
	../../../include/yarmi/generate_struct/unordered_map.hpp \
	../../../include/yarmi/generate_struct/unordered_set.hpp \
	../../../include/yarmi/generate_struct/vector.hpp
