
# Copyright (c) 2013, niXman (i dotty nixman doggy gmail dotty com)
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
	-std=c++11

INCLUDEPATH += \
	../ \
	../../../include

SOURCES += \
	main.cpp

LIBS += \
	-lboost_system

win32:LIBS += \
	-lws2_32

HEADERS += \
	 ../protocol.hpp \
	 ../../../include/yarmi/client_base.hpp \
	 ../../../include/yarmi/server.hpp \
	 ../../../include/yarmi/yarmi.hpp \
	 ../../../include/yarmi/session_base.hpp \
	 ../../../include/yarmi/global_context_base.hpp \
    ../../../include/yarmi/formatters/decorators.hpp \
    ../../../include/yarmi/formatters/list.hpp \
    ../../../include/yarmi/formatters/map.hpp \
    ../../../include/yarmi/formatters/pair.hpp \
    ../../../include/yarmi/formatters/set.hpp \
    ../../../include/yarmi/formatters/tuple.hpp \
    ../../../include/yarmi/formatters/unordered_map.hpp \
    ../../../include/yarmi/formatters/unordered_set.hpp \
    ../../../include/yarmi/formatters/vector.hpp \
    ../../../include/yarmi/formatters/quoting.hpp