
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
	../../../include/yarmi/client/client_base.hpp \
	../../../include/yarmi/client/qt_client_base.hpp \
	../../../include/yarmi/detail/cthash/crc32.hpp \
	../../../include/yarmi/detail/cthash/fnv1a.hpp \
	../../../include/yarmi/detail/generate_struct/decorators.hpp \
	../../../include/yarmi/detail/generate_struct/jsonify.hpp \
	../../../include/yarmi/detail/generate_struct/list.hpp \
	../../../include/yarmi/detail/generate_struct/map.hpp \
	../../../include/yarmi/detail/generate_struct/pair.hpp \
	../../../include/yarmi/detail/generate_struct/set.hpp \
	../../../include/yarmi/detail/generate_struct/tuple.hpp \
	../../../include/yarmi/detail/generate_struct/unordered_map.hpp \
	../../../include/yarmi/detail/generate_struct/unordered_set.hpp \
	../../../include/yarmi/detail/generate_struct/vector.hpp \
	../../../include/yarmi/detail/pp/generate_callers.hpp \
	../../../include/yarmi/detail/pp/generate_invoker.hpp \
	../../../include/yarmi/detail/pp/generate_invokers_sfinae.hpp \
	../../../include/yarmi/detail/pp/generate_lazy_if.hpp \
	../../../include/yarmi/detail/pp/generate_metacode.hpp \
	../../../include/yarmi/detail/pp/generate_ns.hpp \
	../../../include/yarmi/detail/pp/generate_ns_to_string.hpp \
	../../../include/yarmi/detail/pp/generate_proc_helper.hpp \
	../../../include/yarmi/detail/pp/generate_tuple_is_empty.hpp \
	../../../include/yarmi/detail/throw/throw.hpp \
	../../../include/yarmi/serializers/boost_serialization.hpp \
	../../../include/yarmi/serializers/yas_serialization.hpp \
	../../../include/yarmi/server/global_context_base.hpp \
	../../../include/yarmi/server/handler_allocator.hpp \
	../../../include/yarmi/server/make_preallocated_handler.hpp \
	../../../include/yarmi/server/os_resources.hpp \
	../../../include/yarmi/server/preallocated_handler_invoker.hpp \
	../../../include/yarmi/server/server.hpp \
	../../../include/yarmi/server/server_base.hpp \
	../../../include/yarmi/server/server_statistic.hpp \
	../../../include/yarmi/server/session.hpp \
	../../../include/yarmi/generate_enum.hpp \
	../../../include/yarmi/generate_struct.hpp \
	../../../include/yarmi/invoke.hpp \
	../../../include/yarmi/procname.hpp \
	../../../include/yarmi/yarmi.hpp \
	../../../include/yarmi/yarmi_fwd.hpp \
    ../../../include/yarmi/serializers/binary_serializer_base.hpp \
    ../../../include/yarmi/detail/endian/endian.hpp \
    ../../../include/yarmi/detail/pp/generate_tools.hpp \
    ../../../include/yarmi/server/run.hpp
