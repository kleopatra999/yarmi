
# Copyright (c) 2013,2014, niXman (i dotty nixman doggy gmail dotty com)
# All rights reserved.
#
# This file is part of YARMI(https://github.com/niXman/yarmi) project.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
#   Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
#   Redistributions in binary form must reproduce the above copyright notice, this
#   list of conditions and the following disclaimer in the documentation and/or
#   other materials provided with the distribution.
#
#   Neither the name of the {organization} nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += \
	-std=c++1y

INCLUDEPATH += \
	../ \
	../../../include

gcc: {
} else:msvc {
} else {
	DEFINES += \
		BOOST_PP_VARIADICS=1
}

LIBS += \
	-lboost_system

win32:LIBS += \
	-lws2_32 \
	-lmswsock

SOURCES += \
	main.cpp \
	user_context.cpp \
	../../../src/global_context_base.cpp \
	../../../src/server_base.cpp \
	../../../src/server_statistic.cpp \
	server_first_invoker.cpp \
	server_second_invoker.cpp \
    ../../../src/session.cpp

win32:SOURCES += \
	../../../src/os_resources_win.cpp
unix:SOURCES += \
	../../../src/os_resources_nix.cpp

HEADERS += \
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
    ../../../include/yarmi/detail/endian/endian.hpp
