#!/bin/bash
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
#
export DS_PATH=$COMBINED_ROOT/devicesettings



if [ $RDK_PLATFORM_SOC = "intel" ]; then
	export TOOLCHAIN_DIR=$COMBINED_ROOT/sdk/toolchain/staging_dir
	export CROSS_COMPILE=$TOOLCHAIN_DIR/bin/i686-cm-linux
	export CC=$CROSS_COMPILE-gcc
	export CXX=$CROSS_COMPILE-g++
elif [ $RDK_PLATFORM_SOC = "broadcom" ]; then
	export WORK_DIR=$COMBINED_ROOT/work${RDK_PLATFORM_DEVICE^^}
	if [ ${RDK_PLATFORM_DEVICE} = "rng150" ];then
                        if [ -f $COMBINED_ROOT/sdk/scripts/setBcmEnv.sh ]; then
                                . $COMBINED_ROOT/sdk/scripts/setBcmEnv.sh
                        fi
                        if [ -f $COMBINED_ROOT/sdk/scripts/setBCMenv.sh ]; then
                                . $COMBINED_ROOT/sdk/scripts/setBCMenv.sh
                        fi
                else
                        . $COMBINED_ROOT/build_scripts/setBCMenv.sh
                fi

	CROSS_COMPILE=mipsel-linux
	export CC=$CROSS_COMPILE-gcc
	export CXX=$CROSS_COMPILE-g++
fi
make
if [ $? -ne 0 ] ; then
  echo hostDataConverter Build Failed
  exit 1
else
  echo hostDataConverter Build Success
  exit 0
fi
