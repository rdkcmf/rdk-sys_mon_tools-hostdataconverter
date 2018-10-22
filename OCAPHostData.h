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
#ifndef _OCAP_HOSTDATA_H_
#define _OCAP_HOSTDATA_H
#include "readWriteJavaObj.h"

#define SUID_PERSISTENTDATA (0x5ffa5d4b5ad07272LL)                         
#define SUID_HOSTDATA (0xdcc46ea9435c2da8LL)                               
#define SUID_PROPERTIES (0x3912d07a70363e98LL)                             
#define SUID_DEVICESETTINGSHOSTDATA (0x33c79f97aef087dLL)                  
#define SUID_FIXEDVIDEOOUTPUTCONFIGURATIONIMPL (0x7aeb2f9745e6b639LL)      
#define SUID_DEVICESETTINGSHOSTDATAAUDIOPORTVALUES (0xae39b883d0bff946LL)  


typedef enum _VP_ASPECT_RATIO
{
   VP_ASPECT_RATIO_UNKNOWN= -1,
   VP_ASPECT_RATIO_4_3= 2,
   VP_ASPECT_RATIO_16_9= 3,
   VP_ASPECT_RATIO_2_21_1= 4
} VP_ASPECT_RATIO;

typedef enum _VP_SCAN
{
   VP_SCAN_UNKNOWN= 0,
   VP_SCAN_INTERLACED= 1,
   VP_SCAN_PROGRESSIVE= 2,
} VP_SCAN;

typedef enum _VP_STEREOSCOPIC
{
   VP_STEREOSCOPIC_UNKNOWN= 0,
   VP_STEREOSCOPIC_2D= 1,
   VP_STEREOSCOPIC_3D_SBS= 3,
   VP_STEREOSCOPIC_3D_TAB= 4,
} VP_STEREOSCOPIC;

typedef struct _VideoPort
{
   int ar;
   bool enabled;
   float rate;
   int rezHeight;
   int rezWidth;
   int scan;
   int stereoscopicMode;
   std::string modeName;
   std::string portName;
} VideoPort;

typedef enum _AP_COMPRESSION
{
   AP_COMPRESSION_NONE= 0,
   AP_COMPRESSION_LIGHT= 1,
   AP_COMPRESSION_MEDIUM= 2,
   AP_COMPRESSION_HEAVY= 3,
} AP_COMPRESSION;

typedef enum _AP_ENCODING
{
   AP_ENCODING_NONE= 0,
   AP_ENCODING_DISPLAY= 1,
   AP_ENCODING_PCM= 2,
   AP_ENCODING_AC3= 3,
} AP_ENCODING;

typedef enum _AP_STEREOMODE
{
   AP_STEREOMODE_MONO= 1,
   AP_STEREOMODE_STEREO= 2,
   AP_STEREOMODE_SURROUND= 3,
} AP_STEREOMODE;

typedef struct _AudioPort
{
   int compression;
   int encoding;
   float gain;
   float level;
   bool loopThru;
   bool muted;
   int stereoMode;
   std::string portName;
} AudioPort;

typedef struct _HostDeviceSettings
{
   std::string mainVideoPortName;
   int videoPortCount;
   int videoPortCapacity;
   VideoPort **videoPorts;
   int audioPortCount;
   int audioPortCapacity;
   AudioPort **audioPorts;
} HostDeviceSettings;

class OCAPHostData
{
    static OCAPHostData *instance; 
    
    string fileName; 

    HostDeviceSettings *hostDeviceData;

    HostDeviceSettings* hostDeviceSettingsCreate();
    
    OCAPHostData(string _fileName);

    SCObject* createPersistentVideoPort( VideoPort *videoPort );

    SCObject* createPersistentAudioPort( AudioPort *audioPort );

    SCObject* createPersistentProperties( int itemCount, SCObject **ppObjPHT );

    SCObject* createPersistentHashTable( int itemCount );

    bool addValueToPersistentHashTable( SCObject *objPHT, int index, const char* key, SCObject *value );

    std::string getMainVideoPort( SCObject *objHostData );

    int getNumVideoPorts( SCObject *objHostData );

    bool getVideoPortByIndex( SCObject *objHostData, int index, VideoPort **ppVideoPort );

    int getNumAudioPorts( SCObject *objHostData );

    bool getAudioPortByIndex( SCObject *objHostData, int index, AudioPort **ppAudioPort );

    void load();

    void createDefaultSettings();
 
public:
    
    static OCAPHostData* getInstance(string _fileName);

    bool commit();

    string getDefaultVideoPort();
   
    int getAudioPortCompressionByName(string _portName);

    int getAudioPortEncodingByName(string _portName);

    float getAudioPortGainByName(string _portName);

    bool getAudioPortLoopThruByName(string _portName);

    bool getAudioMuteByName(string _portName);

    int getAudioPortStereoModeByName(string _portName);
 
    int getAspectRatioByName(string _portName);    
         
    bool getVideoPortEnabledByName(string _portName);   

    int getRefreshRateByName(string _portName);   
 
    int getResolutionHeightByName(string _portName); 
    
    int getResolutionWidthByName(string _portName); 
    
    int getScanByName(string _portName);

    int getStereoscopicModeByName(string _portName);

    string getResolutionNameByName(string _portName);
 
    void getVideoPortNames(string* portName);
    
    void getAudioPortNames(string* portName);

    int getVideoPortCount();
    
    int getAudioPortCount();

    bool hostDeviceSettingsAddVideoPort(VideoPort *videoPort);

    bool hostDeviceSettingsAddAudioPort(AudioPort *audioPort);
 
    void setAudioPortCompressionByName(string _portName , int compression);
   
    void setAudioPortEncodingByName(string _portName , int encoding);
   
    void setAudioPortGainByName(string _portName , float gain);
   
    void setAudioPortLoopThruByName(string _portName , bool loopThru);
   
    void setAudioMuteByName(string _portName , bool Mute);
   
    void setAudioPortStereoModeByName(string _portName , int stereomode);
   
    void setAspectRatioByName(string _portName , int ar);    
         
    void setVideoPortEnabledByName(string _portName , int enabled);   
   
    void setRefreshRateByName(string _portName , int refreshRate);   
   
    void setResolutionHeightByName(string _portName , int height); 
    
    void setResolutionWidthByName(string _portName , int width); 

    void setScanByName(string _portName , int scan);

    void setStereoscopicModeByName(string _portName , int stereoscope);

    void setResolutionNameByName(string _portName , string name);

    void setDefaultVideoPort(string _portName);
};

#endif //_OCAP_HOSTDATA_H_ 

