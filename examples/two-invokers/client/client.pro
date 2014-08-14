
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
    client.hpp \
    ../../../include/yarmi/generate_struct/decorators.hpp \
    ../../../include/yarmi/generate_struct/jsonify.hpp \
    ../../../include/yarmi/generate_struct/list.hpp \
    ../../../include/yarmi/generate_struct/map.hpp \
    ../../../include/yarmi/generate_struct/pair.hpp \
    ../../../include/yarmi/generate_struct/set.hpp \
    ../../../include/yarmi/generate_struct/tuple.hpp \
    ../../../include/yarmi/generate_struct/unordered_map.hpp \
    ../../../include/yarmi/generate_struct/unordered_set.hpp \
    ../../../include/yarmi/generate_struct/vector.hpp \
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
    ../../../include/yarmi/get_proc_name.hpp \
    ../../../include/yarmi/global_context_base.hpp \
    ../../../include/yarmi/handler_allocator.hpp \
    ../../../include/yarmi/invoke.hpp \
    ../../../include/yarmi/make_preallocated_handler.hpp \
    ../../../include/yarmi/os_resources.hpp \
    ../../../include/yarmi/preallocated_handler_invoker.hpp \
    ../../../include/yarmi/qt_client_base.hpp \
    ../../../include/yarmi/server_base.hpp \
    ../../../include/yarmi/server_statistic.hpp \
    ../../../include/yarmi/server.hpp \
    ../../../include/yarmi/session_base.hpp \
    ../../../include/yarmi/throw.hpp \
    ../../../include/yarmi/yarmi_fwd.hpp \
    ../../../include/yarmi/yarmi.hpp \
    ../../../include/yarmi/yas_serialization.hpp
