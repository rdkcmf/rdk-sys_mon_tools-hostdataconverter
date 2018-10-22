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
#include "OCAPHostData.h"

OCAPHostData* OCAPHostData::instance = NULL;

OCAPHostData::OCAPHostData(string _fileName )
{
   fileName = _fileName;
   hostDeviceData = hostDeviceSettingsCreate();
   load();
}

OCAPHostData* OCAPHostData::getInstance(string _fileName)
{
    if(instance)
    {
        return instance;
    }
    else
    {
        instance = new OCAPHostData(_fileName);
    }
    return instance;
}

HostDeviceSettings* OCAPHostData::hostDeviceSettingsCreate()
{
   hostDeviceData= new HostDeviceSettings();
   if ( hostDeviceData )
   {
      hostDeviceData->videoPorts= (VideoPort**)calloc( 2, sizeof(VideoPort*) );
      if ( !hostDeviceData->videoPorts )
      {
         delete hostDeviceData;
         hostDeviceData= 0;
         goto exit;
      }
      hostDeviceData->videoPortCapacity= 2;
      hostDeviceData->videoPortCount= 0;

      hostDeviceData->audioPorts= (AudioPort**)calloc( 2, sizeof(AudioPort*) );
      if ( !hostDeviceData->audioPorts )
      {
         free( hostDeviceData->videoPorts );
         delete hostDeviceData;
         hostDeviceData= 0;
         goto exit;
      }
      hostDeviceData->audioPortCapacity= 2;
      hostDeviceData->audioPortCount= 0;
   }

exit:

   return hostDeviceData;
}    

void OCAPHostData::load()
{
   SCObject *objDeviceSettingsHostData= 0;
   int videoPortCount, audioPortCount;
   VideoPort *videoPort= 0;
   AudioPort *audioPort= 0;
   bool error= false;

   objDeviceSettingsHostData= readHostData( fileName.c_str() );
   if ( objDeviceSettingsHostData )
   {
      if ( hostDeviceData )
      {
         hostDeviceData->mainVideoPortName= getMainVideoPort( objDeviceSettingsHostData );

         videoPortCount= getNumVideoPorts( objDeviceSettingsHostData );
         for( int i= 0; i < videoPortCount; ++i )
         {
            if ( !getVideoPortByIndex( objDeviceSettingsHostData, i, &videoPort ) )
            {
               error= true;
               goto exit;
            }
            if ( !hostDeviceSettingsAddVideoPort(videoPort ) )
            {
               error= true;
               goto exit;
            }
            videoPort= 0;
         }

         audioPortCount= getNumAudioPorts( objDeviceSettingsHostData );
         for( int i= 0; i < audioPortCount; ++i )
         {
            if ( !getAudioPortByIndex( objDeviceSettingsHostData, i, &audioPort ) )
            {
               error= true;
               goto exit;
            }
            if ( !hostDeviceSettingsAddAudioPort( audioPort ) )
            {
               error= true;
               goto exit;
            }
            audioPort= 0;
         }
      }
   }
   else
   {
       createDefaultSettings();
   }
   

exit:

   if ( error )
   {
      if ( videoPort )
      {
         delete videoPort;
      }
      if ( audioPort )
      {
         delete audioPort;
      }
   }
}
void OCAPHostData::createDefaultSettings()
{
    if(hostDeviceData)
    {
       int i = 0;

       for(i = 0;i < hostDeviceData->audioPortCount; i++)
       {
          delete hostDeviceData->audioPorts[i];
       }
       delete hostDeviceData->audioPorts;

       for(i = 0;i < hostDeviceData->videoPortCount; i++)
       { 
          delete hostDeviceData->videoPorts[i];
       }

       delete hostDeviceData->videoPorts;
       delete hostDeviceData;    

    }

    hostDeviceData = hostDeviceSettingsCreate();

    if(hostDeviceData)
    {
        VideoPort *videoPort= 0;
        videoPort= new VideoPort();
        videoPort->portName= std::string("VL_component");
        videoPort->modeName= std::string("720p");
        videoPort->ar= VP_ASPECT_RATIO_16_9;
        videoPort->enabled= true;
        videoPort->rate= 6.0;
        videoPort->rezWidth= 1280;
        videoPort->rezHeight= 720;
        videoPort->scan= VP_SCAN_PROGRESSIVE;
        videoPort->stereoscopicMode= VP_STEREOSCOPIC_2D;
        hostDeviceSettingsAddVideoPort( videoPort );

        videoPort= new VideoPort();
        videoPort->portName= std::string("VL_hdmi");
        videoPort->modeName= std::string("720p");
        videoPort->ar= VP_ASPECT_RATIO_16_9;
        videoPort->enabled= true;
        videoPort->rate= 6.0;
        videoPort->rezWidth= 1280;
        videoPort->rezHeight= 720;
        videoPort->scan= VP_SCAN_PROGRESSIVE;
        videoPort->stereoscopicMode= VP_STEREOSCOPIC_2D;
        hostDeviceSettingsAddVideoPort( videoPort );

        AudioPort *audioPort= 0;

        audioPort= new AudioPort();
        audioPort->portName= std::string("SPDIF_PORT");
        audioPort->compression= AP_COMPRESSION_NONE;
        audioPort->encoding= AP_ENCODING_NONE;
        audioPort->gain= 70.0;
        audioPort->level= 0.60;
        audioPort->loopThru= false;
        audioPort->muted= false;
        audioPort->stereoMode= AP_STEREOMODE_STEREO;
        hostDeviceSettingsAddAudioPort( audioPort );

        
        audioPort= new AudioPort();
        audioPort->portName= std::string("HDMI_PORT");
        audioPort->compression= AP_COMPRESSION_NONE;
        audioPort->encoding= AP_ENCODING_NONE;
        audioPort->gain= 70.0;
        audioPort->level= 0.60;
        audioPort->loopThru= false;
        audioPort->muted= false;
        audioPort->stereoMode= AP_STEREOMODE_STEREO;
        hostDeviceSettingsAddAudioPort( audioPort );

        hostDeviceData->mainVideoPortName= std::string("VL_component");

        commit();
    }
}


bool OCAPHostData::commit()
{
   bool result= false;
   SCObject *objPersistentData= 0;
   SCObject *objHostData= 0;
   SCObject *objProperties= 0;
   SCObject *objPHT= 0;
   SCString *strValue= 0;
   SCObject *objDeviceSettingsHostData= 0;
   SCObject *objVideoPort= 0;
   SCObject *objAudioPort= 0;
   
   if ( hostDeviceData && (fileName.c_str() != NULL) )
   {
      result= true;
      
      objPersistentData= ObjectCreate( "org.cablelabs.impl.persistent.PersistentData", SUID_PERSISTENTDATA );
      if ( !objPersistentData ) goto exit;

      if ( !ObjectAddFieldLong( objPersistentData, "uniqueId", 1LL ) )
      {
         result= false;
         goto exit;
      }
      
      objHostData= ObjectCreate( "org.cablelabs.impl.ocap.hardware.HostData", SUID_HOSTDATA, FLAGS_SERIALIZABLE, objPersistentData );
      if ( !objHostData )
      {
         result= false;
         goto exit;
      }   
      objPersistentData= 0;
      
      if ( !ObjectAddFieldBoolean( objHostData, "persisted", true ) )
      {
         result= false;
         goto exit;
      }
      
      objProperties= createPersistentProperties( 1, &objPHT );
      if ( !objProperties || !objPHT )
      {
         result= false;
         goto exit;
      }
      
      strValue= SCStringCreate( hostDeviceData->mainVideoPortName.c_str() );
      if ( !strValue )
      {
         result= false;
         goto exit;
      }
      
      if ( !addValueToPersistentHashTable( objPHT, 0, "OCAP.ds.hostSetting.mainVideoOutputPort.0", SCOBJECT(strValue) ) )
      {
         result= false;
         goto exit;
      }
      strValue= 0;
      
      if ( !ObjectAddFieldObject( objHostData, "data", "Ljava.util.Properties", objProperties ) )
      {
         result= false;
         goto exit;
      }
      objProperties= 0;
      objPHT= 0;
      
      objDeviceSettingsHostData= ObjectCreate( "org.cablelabs.impl.ocap.hardware.device.DeviceSettingsHostData", 
                                                SUID_DEVICESETTINGSHOSTDATA, FLAGS_SERIALIZABLE, objHostData );
      if ( !objDeviceSettingsHostData )
      {
         result= false;
         goto exit;
      }   
      objHostData= 0;
      
      
      // Create and populate video port hash table
      objPHT= createPersistentHashTable( hostDeviceData->videoPortCount );
      if ( !objPHT )
      {
         result= false;
         goto exit;
      }
      
      for( int i= 0; i < hostDeviceData->videoPortCount; ++i )
      {
         objVideoPort= createPersistentVideoPort( hostDeviceData->videoPorts[i] );
         if ( !objVideoPort )
         {
            result= false;
            goto exit;
         }
         
         if ( !addValueToPersistentHashTable( objPHT, i, hostDeviceData->videoPorts[i]->portName.c_str(), objVideoPort ) )
         {
            result= false;
            goto exit;
         }
         objVideoPort= 0;         
      }
      
      if( !ObjectAddFieldObject( objDeviceSettingsHostData, "videoPortToOutputConfig", "Ljava.util.Hashtable;", objPHT ) )
      {
         result= false;
         goto exit;
      }
      objPHT= 0;
      
      // Create and populate audio port hash table
      objPHT= createPersistentHashTable( hostDeviceData->audioPortCount );
      if ( !objPHT )
      {
         result= false;
         goto exit;
      }

      for( int i= 0; i < hostDeviceData->audioPortCount; ++i )
      {
         objAudioPort= createPersistentAudioPort( hostDeviceData->audioPorts[i] );
         if ( !objAudioPort )
         {
            result= false;
            goto exit;
         }
         
         if ( !addValueToPersistentHashTable( objPHT, i, hostDeviceData->audioPorts[i]->portName.c_str(), objAudioPort ) )
         {
            result= false;
            goto exit;
         }
         objAudioPort= 0;         
      }
      
      if( !ObjectAddFieldObject( objDeviceSettingsHostData, "audioPortToValues", "Ljava.util.Hashtable;", objPHT ) )
      {
         result= false;
         goto exit;
      }
      objPHT= 0;
      

      result= writeHostData( fileName.c_str(), true, objDeviceSettingsHostData );
   }
   
exit:   

   if ( !result )
   {
      if ( strValue )
      {
         SCStringDestroy( strValue );
      }
      
      if ( objDeviceSettingsHostData )
      {
         ObjectDestroy( objDeviceSettingsHostData );
      }
      
      if ( objHostData )
      {
         ObjectDestroy( objHostData );
      }
      
      if ( objPersistentData )
      {
         ObjectDestroy( objPersistentData );
      }

      if ( objProperties )
      {
         ObjectDestroy( objProperties );
      }
      
      if ( objPHT )
      {
         ObjectDestroy( objPHT );
      }
      
      if ( objVideoPort )
      {
         ObjectDestroy( objVideoPort );
      }
      
      if ( objAudioPort )
      {
         ObjectDestroy( objAudioPort );
      }
   }
   
   return result;
}

bool OCAPHostData::hostDeviceSettingsAddVideoPort(VideoPort *videoPort )
{
   bool result= false;

   if ( hostDeviceData && videoPort )
   {
      if ( hostDeviceData->videoPortCount >= hostDeviceData->videoPortCapacity )
      {
         int newCapacity= hostDeviceData->videoPortCapacity*2+1;
         VideoPort **videoPortsNew= (VideoPort**)calloc( newCapacity, sizeof(VideoPort*) );
         if ( !videoPortsNew )
         {
            printf( "error allocating memory to expand video port capacity from %d to %d\n", hostDeviceData->videoPortCapacity, newCapacity );
            goto exit;
         }
         memcpy( videoPortsNew, hostDeviceData->videoPorts, hostDeviceData->videoPortCount*sizeof(VideoPort*) );
         free( hostDeviceData->videoPorts );
         hostDeviceData->videoPorts= videoPortsNew;
         hostDeviceData->videoPortCapacity= newCapacity;
      }

      for( int i= 0; i < hostDeviceData->videoPortCount; ++i )
      {
         if ( hostDeviceData->videoPorts[i]->portName.compare( videoPort->portName ) == 0 )
         {
            printf( "duplicate video port name: %s\n", videoPort->portName.c_str() );
            goto exit;
         }
      }

      hostDeviceData->videoPorts[hostDeviceData->videoPortCount]= videoPort;

      ++hostDeviceData->videoPortCount;

      result= true;
   }

exit:
   return result;
}

bool OCAPHostData::hostDeviceSettingsAddAudioPort(AudioPort *audioPort )
{
   bool result= false;

   if ( hostDeviceData && audioPort )
   {
      if ( hostDeviceData->audioPortCount >= hostDeviceData->audioPortCapacity )
      {
         int newCapacity= hostDeviceData->audioPortCapacity*2+1;
         AudioPort **audioPortsNew= (AudioPort**)calloc( newCapacity, sizeof(AudioPort*) );
         if ( !audioPortsNew )
         {
            printf( "error allocating memory to expand audio port capacity from %d to %d\n", hostDeviceData->audioPortCapacity, newCapacity );
            goto exit;
         }
         memcpy( audioPortsNew, hostDeviceData->audioPorts, hostDeviceData->audioPortCount*sizeof(AudioPort*) );
         free( hostDeviceData->audioPorts );
         hostDeviceData->audioPorts= audioPortsNew;
         hostDeviceData->audioPortCapacity= newCapacity;
      }

      for( int i= 0; i < hostDeviceData->audioPortCount; ++i )
      {
         if ( hostDeviceData->audioPorts[i]->portName.compare( audioPort->portName ) == 0 )
         {
            printf( "duplicate audio port name: %s\n", audioPort->portName.c_str() );
            goto exit;
         }
      }

      hostDeviceData->audioPorts[hostDeviceData->audioPortCount]= audioPort;

      ++hostDeviceData->audioPortCount;

      result= true;
   }

exit:
   return result;
}
 
SCObject* OCAPHostData::createPersistentVideoPort( VideoPort *videoPort )
{
   SCObject *objVideoPort= 0;
   SCString *strValue= 0;
   bool error= false;

   objVideoPort= ObjectCreate( "org.cablelabs.impl.ocap.hardware.device.FixedVideoOutputConfigurationImpl", SUID_FIXEDVIDEOOUTPUTCONFIGURATIONIMPL );
   if ( objVideoPort )
   {
      if ( !ObjectAddFieldInt( objVideoPort, "ar", videoPort->ar ) )
      {
         error= true;
         goto exit;
      }
      
      if ( !ObjectAddFieldBoolean( objVideoPort, "enabled", videoPort->enabled ) )
      {
         error= true;
         goto exit;
      }
      
      if ( !ObjectAddFieldFloat( objVideoPort, "rate", videoPort->rate ) )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldInt( objVideoPort, "rezHeight", videoPort->rezHeight ) )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldInt( objVideoPort, "rezWidth", videoPort->rezWidth ) )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldInt( objVideoPort, "scan", videoPort->scan ) )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldInt( objVideoPort, "stereoscopicMode", videoPort->stereoscopicMode ) )
      {
         error= true;
         goto exit;
      }
      
      strValue= SCStringCreate( videoPort->modeName.c_str() );
      if ( !strValue )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldString( objVideoPort, "name", strValue ) )
      {
         error= true;
         goto exit;
      }
      strValue= 0;
   }
   
exit:

   if (  strValue )
   {
      SCStringDestroy( strValue );      
   }
      
   if ( error )
   {
      if ( objVideoPort )
      {
         ObjectDestroy( objVideoPort );
         objVideoPort= 0;
      }
   }
   
   return objVideoPort;
}

SCObject* OCAPHostData::createPersistentAudioPort( AudioPort *audioPort )
{
   SCObject *objAudioPort= 0;
   bool error= false;

   objAudioPort= ObjectCreate( "org.cablelabs.impl.ocap.hardware.device.DeviceSettingsHostData$AudioPortValues", SUID_DEVICESETTINGSHOSTDATAAUDIOPORTVALUES );
   if ( objAudioPort )
   {
      if ( !ObjectAddFieldInt( objAudioPort, "compression", audioPort->compression ) )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldInt( objAudioPort, "encoding", audioPort->encoding ) )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldFloat( objAudioPort, "gain", audioPort->gain ) )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldFloat( objAudioPort, "level", audioPort->level ) )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldBoolean( objAudioPort, "loopThru", audioPort->loopThru ) )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldBoolean( objAudioPort, "muted", audioPort->muted ) )
      {
         error= true;
         goto exit;
      }

      if ( !ObjectAddFieldInt( objAudioPort, "stereoMode", audioPort->stereoMode ) )
      {
         error= true;
         goto exit;
      }
   }
   
exit:

   if ( error )
   {
      if ( objAudioPort )
      {
         ObjectDestroy( objAudioPort );
         objAudioPort= 0;
      }
   }
   
   return objAudioPort;
}

SCObject* OCAPHostData::createPersistentProperties( int itemCount, SCObject **ppObjPHT )
{
   SCObject *objPProp= 0;
   SCObject *objPHT= 0;
   
   objPHT= createPersistentHashTable( itemCount );
   if ( objPHT )
   {
      objPProp= ObjectCreate( "java.util.Properties", SUID_PROPERTIES, FLAGS_SERIALIZABLE, objPHT );
      if ( !objPProp )
      {
         goto exit;
      }
      *ppObjPHT= objPHT;
      objPHT= 0;
      
      if ( !ObjectAddFieldObject( objPProp, "defaults", "Ljava.util.Properties;", 0 ) )
      {
         ObjectDestroy( objPProp );
         objPProp= 0;
         *ppObjPHT= 0;
         goto exit;
      }
   }
   
exit:

   if ( objPHT )
   {
      ObjectDestroy( objPHT );
      objPHT= 0;
   }
   
   return objPProp;
}

SCObject* OCAPHostData::createPersistentHashTable( int itemCount )
{
   SCObject *objPHT= 0;
   int tableLength= 3;
   
   objPHT= ObjectCreate( "java.util.Hashtable", 0 );
   if ( !objPHT ) goto exit;

   while( tableLength <= itemCount )
   {
      tableLength= tableLength*2 + 1;
   }
   
   if ( !ObjectAddFieldInt( objPHT, "$HashTableLength", tableLength) ) goto exit;

   if ( !ObjectAddFieldInt( objPHT, "$HashKeyCount", itemCount) ) goto exit;
   
   if ( !ObjectAddFieldFloat( objPHT, "loadFactor", 0.75f ) ) goto exit;

   if ( !ObjectAddFieldInt( objPHT, "threshold", 8) ) goto exit;
   
exit:
   
   return objPHT;
}

bool OCAPHostData::addValueToPersistentHashTable( SCObject *objPHT, int index, const char* key, SCObject *value )
{
   bool result= false;
   SCString *strKey, *strValue;
   char work[32];
   
   sprintf( work, "$HashKey%d", index );

   strKey= SCStringCreate( work );
   strValue= SCStringCreate( key );
   if ( strKey && strValue )
   {
      if ( SCHashTableAddValue( SCHASHTABLE(objPHT), strKey, SCOBJECT(strValue) ) == 0 )
      {
         strKey= SCStringCreate( key );
         if ( strKey )
         {
            if ( SCHashTableAddValue( SCHASHTABLE(objPHT), strKey, value ) == 0 )
            {
               result= true;
            }
         }
      }
      else
      {
         SCStringDestroy( strKey );
         SCStringDestroy( strValue );
      }
   }
   else
   {
      if ( strKey ) SCStringDestroy( strKey );
      if ( strValue ) SCStringDestroy( strValue );
   }
   
   return result;
}


std::string OCAPHostData::getMainVideoPort( SCObject *objHostData )
{
   std::string mainPort;
   
   // get java.utils.Properties data instance from org.cablelabs.impl.ocap.hardware.HostData
   SCObject *objData= ObjectGetFieldValueObject( objHostData, "data" );
   if ( objData )
   {
      SCString *str= ObjectGetFieldValueString( objData, "OCAP.ds.hostSetting.mainVideoOutputPort.0" );
      if ( str )
      {
         mainPort= std::string( str->text, str->len );
      }
   }
   
   return mainPort;
}

int OCAPHostData::getNumVideoPorts( SCObject *objHostData )
{
   int numVideoPorts= 0;
   
   SCObject *objHashTable= ObjectGetFieldValueObject( objHostData, "videoPortToOutputConfig" );
   if ( objHashTable )
   {
      numVideoPorts= ObjectGetFieldValueInt( objHashTable, "$HashKeyCount" );
   }
   return numVideoPorts;
}

bool OCAPHostData::getVideoPortByIndex( SCObject *objHostData, int index, VideoPort **ppVideoPort )
{
   bool result= false;
   
   *ppVideoPort= 0;
   
   SCObject *objHashTable= ObjectGetFieldValueObject( objHostData, "videoPortToOutputConfig" );
   if ( objHashTable )
   {
      char key[32];
      SCString *strKey;
      
      sprintf( key, "$HashKey%d", index );
      strKey= ObjectGetFieldValueString( objHashTable, key );
      if ( strKey )
      {
         SCObject *objValue= ObjectGetFieldValueObject( objHashTable, strKey );
         if ( objValue )
         {
            VideoPort *port= new VideoPort();
            
            port->ar= ObjectGetFieldValueInt( objValue, "ar" );
            port->enabled= ObjectGetFieldValueBoolean( objValue, "enabled" );         
            port->rate= ObjectGetFieldValueFloat( objValue, "rate" );
            port->rezHeight= ObjectGetFieldValueInt( objValue, "rezHeight" );
            port->rezWidth= ObjectGetFieldValueInt( objValue, "rezWidth" );
            port->scan= ObjectGetFieldValueInt( objValue, "scan" );
            port->stereoscopicMode= ObjectGetFieldValueInt( objValue, "stereoscopicMode" );
            SCString *name= ObjectGetFieldValueString( objValue, "name" );
            if ( name )
            {
               port->modeName= std::string( name->text, name->len );
            }
            port->portName= std::string( strKey->text, strKey->len );            
            
            *ppVideoPort= port;
            result= true;
         }
      }
   }
   
   return result;
}

int OCAPHostData::getNumAudioPorts( SCObject *objHostData )
{
   int numAudioPorts= 0;
   
   SCObject *objHashTable= ObjectGetFieldValueObject( objHostData, "audioPortToValues" );
   if ( objHashTable )
   {
      numAudioPorts= ObjectGetFieldValueInt( objHashTable, "$HashKeyCount" );
   }
   return numAudioPorts;
}

bool OCAPHostData::getAudioPortByIndex( SCObject *objHostData, int index, AudioPort **ppAudioPort )
{
   bool result= false;
   
   *ppAudioPort= 0;
   
   SCObject *objHashTable= ObjectGetFieldValueObject( objHostData, "audioPortToValues" );
   if ( objHashTable )
   {
      char key[32];
      SCString *strKey;
      
      sprintf( key, "$HashKey%d", index );
      strKey= ObjectGetFieldValueString( objHashTable, key );
      if ( strKey )
      {
         SCObject *objValue= ObjectGetFieldValueObject( objHashTable, strKey );
         if ( objValue )
         {
            AudioPort *port= new AudioPort();
            
            port->compression= ObjectGetFieldValueInt( objValue, "compression" );
            port->encoding= ObjectGetFieldValueInt( objValue, "encoding" );
            port->gain= ObjectGetFieldValueFloat( objValue, "gain" );
            port->level= ObjectGetFieldValueFloat( objValue, "level" );
            port->loopThru= ObjectGetFieldValueBoolean( objValue, "loopThru" );         
            port->muted= ObjectGetFieldValueBoolean( objValue, "muted" );         
            port->stereoMode= ObjectGetFieldValueInt( objValue, "stereoMode" );
            port->portName= std::string( strKey->text, strKey->len );            
            
            *ppAudioPort= port;
            result= true;
         }
      }
   }
   
   return result;
}

int OCAPHostData::getAudioPortCompressionByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            return hostDeviceData->audioPorts[i]->compression;
        }
    }
    return -1;
}

int OCAPHostData::getAudioPortEncodingByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            return hostDeviceData->audioPorts[i]->encoding;
        }
    }
    return -1;
}

float OCAPHostData::getAudioPortGainByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            return hostDeviceData->audioPorts[i]->gain;
        }
    }
    return -1;
}

bool OCAPHostData::getAudioPortLoopThruByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            return hostDeviceData->audioPorts[i]->loopThru;
        }
    }
    return false;
}   

bool OCAPHostData::getAudioMuteByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            return hostDeviceData->audioPorts[i]->muted;
        }
    }
    return false;
}

int OCAPHostData::getAudioPortStereoModeByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            return hostDeviceData->audioPorts[i]->stereoMode;
        }
    }
    return -1;
}

int OCAPHostData::getAspectRatioByName(string _portName)
{
    int i = 0;                       
    for(i = 0;i < hostDeviceData->videoPortCount; i++) 
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            return hostDeviceData->videoPorts[i]->ar;
        }
    }
    return -1;
}

bool OCAPHostData::getVideoPortEnabledByName(string _portName)
{
    int i = 0;                       
    for(i = 0 ;i < hostDeviceData->videoPortCount; i++) 
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            return hostDeviceData->videoPorts[i]->enabled;
        }
    }
    return false;
}

int OCAPHostData::getRefreshRateByName(string _portName)
{
    int i = 0;                       
    for(i = 0;i < hostDeviceData->videoPortCount; i++) 
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            return hostDeviceData->videoPorts[i]->rate;
        }
    }
    return -1;
}

int OCAPHostData::getResolutionHeightByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            return hostDeviceData->videoPorts[i]->rezHeight;
        }
    }
    return -1;
}

int OCAPHostData::getResolutionWidthByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            return hostDeviceData->videoPorts[i]->rezWidth;
        }
    }
    return -1;
}

int OCAPHostData::getScanByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            return hostDeviceData->videoPorts[i]->scan;
        }
    }
    return -1;
}

int OCAPHostData::getStereoscopicModeByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            return hostDeviceData->videoPorts[i]->stereoscopicMode;
        }
    }
    return -1;
}

string OCAPHostData::getResolutionNameByName(string _portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            return hostDeviceData->videoPorts[i]->modeName;
        }
    }
    return NULL;
}


string OCAPHostData::getDefaultVideoPort()
{
    return hostDeviceData->mainVideoPortName;   
}

void OCAPHostData::setDefaultVideoPort(string newPortName)
{
    hostDeviceData->mainVideoPortName = newPortName;   
}


void OCAPHostData::getVideoPortNames(string* portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        portName[i] = hostDeviceData->videoPorts[i]->portName; 
    }
}

void OCAPHostData::getAudioPortNames(string* portName)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        portName[i] = hostDeviceData->audioPorts[i]->portName; 
    }
}


void OCAPHostData::setAudioPortCompressionByName(string _portName , int compression)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            hostDeviceData->audioPorts[i]->compression = compression;
        }
    }
}

void OCAPHostData::setAudioPortEncodingByName(string _portName , int encoding)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            hostDeviceData->audioPorts[i]->encoding = encoding;
        }
    }
}

void OCAPHostData::setAudioPortGainByName(string _portName , float gain)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            hostDeviceData->audioPorts[i]->gain = gain;
        }
    }
}

void OCAPHostData::setAudioPortLoopThruByName(string _portName , bool loopThru)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            hostDeviceData->audioPorts[i]->loopThru = loopThru;
        }
    }
}   

void OCAPHostData::setAudioMuteByName(string _portName , bool Mute )
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            hostDeviceData->audioPorts[i]->muted = Mute;
        }
    }
}

void OCAPHostData::setAudioPortStereoModeByName(string _portName , int stereomode)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->audioPortCount; i++)
    {
        if(_portName == hostDeviceData->audioPorts[i]->portName)
        {
            hostDeviceData->audioPorts[i]->stereoMode = stereomode;
        }
    }
}

void OCAPHostData::setAspectRatioByName(string _portName , int ar)
{
    int i = 0;                       
    for(i = 0;i < hostDeviceData->videoPortCount; i++) 
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            hostDeviceData->videoPorts[i]->ar = ar;
        }
    }
}

void OCAPHostData::setVideoPortEnabledByName(string _portName , int enabled)
{
    int i = 0;                       
    for(i = 0;i < hostDeviceData->videoPortCount; i++) 
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            hostDeviceData->videoPorts[i]->enabled = enabled;
        }
    }
}

void OCAPHostData::setRefreshRateByName(string _portName , int refreshRate)
{
    int i = 0;                       
    for(i = 0;i < hostDeviceData->videoPortCount; i++) 
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            hostDeviceData->videoPorts[i]->rate = refreshRate;
        }
    }
}

void OCAPHostData::setResolutionHeightByName(string _portName , int height)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            hostDeviceData->videoPorts[i]->rezHeight = height;
        }
    }
}

void OCAPHostData::setResolutionWidthByName(string _portName , int width)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            hostDeviceData->videoPorts[i]->rezWidth = width;
        }
    }
}

void OCAPHostData::setScanByName(string _portName , int scan)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            hostDeviceData->videoPorts[i]->scan = scan;
        }
    }
}

void OCAPHostData::setStereoscopicModeByName(string _portName , int stereoscope)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            hostDeviceData->videoPorts[i]->stereoscopicMode = stereoscope;
        }
    }
}

void OCAPHostData::setResolutionNameByName(string _portName , string name)
{
    int i = 0;
    for(i = 0;i < hostDeviceData->videoPortCount; i++)
    {
        if(_portName == hostDeviceData->videoPorts[i]->portName)
        {
            hostDeviceData->videoPorts[i]->modeName = name;
        }
    }
}

int OCAPHostData::getVideoPortCount()
{
    return hostDeviceData->videoPortCount;
}

int OCAPHostData::getAudioPortCount()
{
    return hostDeviceData->audioPortCount;
}
