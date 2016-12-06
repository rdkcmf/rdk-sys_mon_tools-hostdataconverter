##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2016 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
SUBDIRS =
DIST_SUBDIRS =

# the list of header files that belong to the library (to be installed later)
AM_CFLAGS = -I./ \
           -I$(PKG_CONFIG_SYSROOT_DIR)${includedir}/rdk/ds \
           -I$(PKG_CONFIG_SYSROOT_DIR)${includedir}/rdk/ds-hal \
           -I$(PKG_CONFIG_SYSROOT_DIR)${includedir}/rdk/ds-rpc

AM_CXXFLAGS = -I./include -I./ \
           -I$(PKG_CONFIG_SYSROOT_DIR)${includedir}/rdk/ds \
           -I$(PKG_CONFIG_SYSROOT_DIR)${includedir}/rdk/ds-hal \
           -I$(PKG_CONFIG_SYSROOT_DIR)${includedir}/rdk/ds-rpc

include_HEADERS = $(top_srcdir)/dsOCAPAdapter.h \
               $(top_srcdir)/OCAPHostData.h \
               $(top_srcdir)/readWriteJavaObj.h

lib_LTLIBRARIES = libOCAPHostDataReader.la

libOCAPHostDataReader_la_SOURCES=dsOCAPAdapter.cpp OCAPHostData.cpp readWriteJavaObj.cpp
libOCAPHostDataReader_la_LIBADD=-lTasseograph

bin_PROGRAMS = hostData_to_2_0

hostData_to_2_0_SOURCES=hostData_to_2_0.c
hostData_to_2_0_LDADD=$(lib_LTLIBRARIES)
