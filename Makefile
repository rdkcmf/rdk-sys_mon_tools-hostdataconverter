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
ROOTFS =$(COMBINED_DIR)/sdk/fsroot/ramdisk/
RM          := rm -rf 
LIBNAME     := OCAPHostDataReader
LIBNAMEFULL := lib$(LIBNAME).so
INSTALL     := $(PWD)/install
OBJS        := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

INCLUDE     :=  -I$(PWD)                                        \
    		-I$(DS_PATH)/hal/include                            \
    		-I$(DS_PATH)/hal/src                                \
                -I./include					\
		-I.

LDFLAGS += -L./lib/ -lTasseograph

CFLAGS      := -g -fPIC -D_REENTRANT -Wall $(INCLUDE)


all: clean install
	@echo "Build Finished...."

hostData_to_2_0: library hostData_to_2_0.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -L. -l$(LIBNAME) hostData_to_2_0.o -o $@ 
	@echo "Build Finished....for $@"

hostData_to_1_3: hostData_to_1_3.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -L. -l$(LIBNAME) hostData_to_1_3.o -o $@ 
	@echo "Build Finished....for $@"

apps: hostData_to_2_0 hostData_to_1_3 
	@echo "Building $@ ...."
	
library: $(OBJS)
	@echo "Building $(LIBNAMEFULL) ...."
	$(CXX) $(OBJS) $(CFLAGS) $(LDFLAGS) -shared -Wl -o $(LIBNAMEFULL)

%.o: %.cpp 
	@echo "Building $@ ...."
	$(CXX) -c $<  $(CFLAGS) -o $@

%.o: %.c
	@echo "Building $@ ...."
	$(CXX) -c $<  $(CFLAGS) -o $@

install: apps
	@echo "installling...!"
	@cp $(LIBNAMEFULL) $(FSROOT)/usr/local/lib
	@cp hostData_to_2_0 $(FSROOT)/usr/local/bin
ifneq ($(BUILD_CONFIG), hybrid)
	@cp -f ./lib/libTasseograph.so $(FSROOT)/usr/local/lib
endif

uninstall:
	@$(RM) $(INSTALL)/lib/$(LIBNAMEFULL)
ifneq ($(BUILD_CONFIG), hybrid)
	@$(RM) $(INSTALL)/lib/libTasseograph.so
endif

clean:
	@echo "Cleaning the directory..."
	@$(RM) *.o 
	@$(RM) $(OBJS) $(LIBNAMEFULL)
	@$(RM) -rf $(INSTALL)


