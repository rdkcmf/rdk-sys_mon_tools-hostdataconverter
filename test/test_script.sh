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

script_name=`basename $0`

if [ -z $1 ]; then
	echo "empty file" 
	echo "Usage is: ./$script_name <ocap_host_data_file>"
	exit -1
fi
if [ ! -f $1 ]; then
	echo "file $1 does not exists" 
	echo "Usage is: ./$script_name <ocap_host_data_file>"
	exit -2
fi

for (( i=0; i<1000; i++ ))
do
	./hostData_to_2_0 $1 dsHostData > /dev/null
	if [ $? -ne 0 ]; then
		echo "Error converting $1 to 2.0 format..\n"
		exit -3
	fi
	./hostData_to_1_3 dsHostData $1 > /dev/null
	if [ $? -ne 0 ]; then
		echo "Error converting 2.0 format back to ocap format ($1)"
		exit -4
	fi
done

if [ $? -eq 0 ]; then
	echo "Succefully converted $1 to 2.0 format and back 1000 times..!!"
	exit 0
else
	echo "Test failed.."
	exit $?
fi

