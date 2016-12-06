/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#ifndef _DS_OCAP_ADAPTER_H_
#define _DS_OCAP_ADAPTER_H_

#include "dsTypes.h"
#include "dsError.h"

#ifdef __cplusplus
extern "C" {
#endif

dsError_t init(const char *);

dsError_t getMainVideoPort(dsVideoPortType_t *);

dsError_t setMainVideoPort(dsVideoPortType_t);

dsError_t getCurrentResolution(dsVideoPortType_t, dsVideoPortResolution_t* );

dsError_t setCurrentResolution(dsVideoPortType_t, const dsVideoPortResolution_t* );

dsError_t getCurrentAudioMode(dsAudioPortType_t audioPort, dsAudioStereoMode_t* pStereoMode);

dsError_t setCurrentAudioMode(dsAudioPortType_t audioPort, dsAudioStereoMode_t stereoMode);
 
dsError_t commitChanges();

#ifdef __cplusplus
};
#endif


#endif //_DS_OCAP_ADAPTER_H_
