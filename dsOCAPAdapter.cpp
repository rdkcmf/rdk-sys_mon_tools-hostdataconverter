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
#include "dsOCAPAdapter.h"
#include "OCAPHostData.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


using namespace std;

const char* _filename = NULL; //with null filename we could create new ocap ds file. 

dsError_t init(const char *filename)
{
    if(filename)
    {	
       struct stat stat_buf;
       if(stat(filename,  &stat_buf) == 0)
       {
          _filename = strdup(filename);
          return dsERR_NONE;
       }
       return dsERR_INVALID_PARAM;
    }
    return dsERR_NONE;
}

dsError_t getMainVideoPort(dsVideoPortType_t *pVideoPort)
{
    OCAPHostData* hsInstance = OCAPHostData::getInstance(_filename);

    if(hsInstance)
    {
        if(strcmp(hsInstance->getDefaultVideoPort().c_str(),"VL_hdmi") == 0)
        {
            *pVideoPort = dsVIDEOPORT_TYPE_HDMI;
        }
        else
        {
            *pVideoPort = dsVIDEOPORT_TYPE_COMPONENT;
        }

        return dsERR_NONE;
    }
    
    return dsERR_GENERAL; 
}

dsError_t setMainVideoPort(dsVideoPortType_t pVideoPort)
{
    OCAPHostData* hsInstance = OCAPHostData::getInstance(_filename);

    if(hsInstance)
    {
        if( pVideoPort == dsVIDEOPORT_TYPE_HDMI)
        {
           hsInstance->setDefaultVideoPort("VL_hdmi"); 
        }
        else
        {
           hsInstance->setDefaultVideoPort("VL_component"); 
        }

        return dsERR_NONE;
    }
    
    return dsERR_GENERAL; 
}

dsError_t getCurrentResolution(dsVideoPortType_t videoPort, dsVideoPortResolution_t* pVideoResolution )
{

    OCAPHostData* hsInstance = OCAPHostData::getInstance(_filename);

    if(hsInstance)
    {
        const char *port_name;

        if(videoPort == dsVIDEOPORT_TYPE_HDMI)
            port_name = "VL_hdmi";
        else
            port_name = "VL_component";

        string curResolution = hsInstance->getResolutionNameByName(port_name);
        
        strcpy(pVideoResolution->name,curResolution.c_str());

        if((hsInstance->getResolutionHeightByName(port_name) == 480) &&
            (hsInstance->getResolutionWidthByName(port_name) == 720))
        {
            pVideoResolution->pixelResolution = dsVIDEO_PIXELRES_720x480;    
        }
        else if((hsInstance->getResolutionHeightByName(port_name) == 576) &&
            (hsInstance->getResolutionWidthByName(port_name) == 720))
        {
            pVideoResolution->pixelResolution = dsVIDEO_PIXELRES_720x576;    
        }
        else if((hsInstance->getResolutionHeightByName(port_name) == 720) &&
            (hsInstance->getResolutionWidthByName(port_name) == 1280))
        {
            pVideoResolution->pixelResolution = dsVIDEO_PIXELRES_1280x720;    
        }
        else if((hsInstance->getResolutionHeightByName(port_name) == 1080) &&
            (hsInstance->getResolutionWidthByName(port_name) == 1920))
        {
            pVideoResolution->pixelResolution = dsVIDEO_PIXELRES_1920x1080;    
        }
        else
        {
            pVideoResolution->pixelResolution = dsVIDEO_PIXELRES_720x480;    
        }

        switch(hsInstance->getAspectRatioByName(port_name))
        {
            case VP_ASPECT_RATIO_4_3:
                pVideoResolution->aspectRatio = dsVIDEO_ASPECT_RATIO_4x3;
                break;
            case VP_ASPECT_RATIO_16_9:
                pVideoResolution->aspectRatio = dsVIDEO_ASPECT_RATIO_16x9;
                break;
            default:
                pVideoResolution->aspectRatio = dsVIDEO_ASPECT_RATIO_4x3;
        }

        switch(hsInstance->getStereoscopicModeByName(port_name))
        {
            case VP_STEREOSCOPIC_2D: 
                pVideoResolution->stereoScopicMode = dsVIDEO_SSMODE_2D;
                break;
            case VP_STEREOSCOPIC_3D_SBS:
                pVideoResolution->stereoScopicMode = dsVIDEO_SSMODE_3D_SIDE_BY_SIDE;
                break;
            case VP_STEREOSCOPIC_3D_TAB: 
                pVideoResolution->stereoScopicMode = dsVIDEO_SSMODE_3D_TOP_AND_BOTTOM;
                break;
            default:
                pVideoResolution->stereoScopicMode = dsVIDEO_SSMODE_2D;
                break;
        }
 
        switch(hsInstance->getRefreshRateByName(port_name))
        { 
            //we dont have the information on values stored for this property in OCAP format.
            default:
                pVideoResolution->frameRate = dsVIDEO_FRAMERATE_60;     

        }

        if( hsInstance->getScanByName(port_name) == VP_SCAN_INTERLACED)
            pVideoResolution->interlaced = true;
        else
            pVideoResolution->interlaced = false;
        return dsERR_NONE;
    }

    return dsERR_GENERAL;
}

dsError_t setCurrentResolution(dsVideoPortType_t videoPort, const dsVideoPortResolution_t* pVideoResolution) 
{

    OCAPHostData* hsInstance = OCAPHostData::getInstance(_filename);

    if(hsInstance)
    {
        const char *port_name;

        if(videoPort == dsVIDEOPORT_TYPE_HDMI)
            port_name = "VL_hdmi";
        else
            port_name = "VL_component";

        hsInstance->setResolutionNameByName(port_name, pVideoResolution->name);

        switch(pVideoResolution->pixelResolution)
        {
            case dsVIDEO_PIXELRES_720x480:
                hsInstance->setResolutionWidthByName(port_name,720);
                hsInstance->setResolutionHeightByName(port_name,480);
                break;
            case dsVIDEO_PIXELRES_720x576:
                hsInstance->setResolutionWidthByName(port_name,720);
                hsInstance->setResolutionHeightByName(port_name,576);
                break;
            case dsVIDEO_PIXELRES_1280x720:
                hsInstance->setResolutionWidthByName(port_name,1280);
                hsInstance->setResolutionHeightByName(port_name,720);
                break;
            case dsVIDEO_PIXELRES_1920x1080:
                hsInstance->setResolutionWidthByName(port_name,1920);
                hsInstance->setResolutionHeightByName(port_name,1080);
                break;
            default:
                hsInstance->setResolutionWidthByName(port_name, 720);
                hsInstance->setResolutionHeightByName(port_name,480);
                break;
        }

        switch(pVideoResolution->aspectRatio)
        {
            case dsVIDEO_ASPECT_RATIO_4x3:
                hsInstance->setAspectRatioByName(port_name,VP_ASPECT_RATIO_4_3);
                break;
            case dsVIDEO_ASPECT_RATIO_16x9:
                hsInstance->setAspectRatioByName(port_name,VP_ASPECT_RATIO_16_9);
                break;
            default:
                hsInstance->setAspectRatioByName(port_name,VP_ASPECT_RATIO_4_3);
        }

        switch(pVideoResolution->stereoScopicMode)
        {
            case dsVIDEO_SSMODE_2D: 
                hsInstance->setStereoscopicModeByName(port_name,VP_STEREOSCOPIC_2D);
                break;
            case dsVIDEO_SSMODE_3D_SIDE_BY_SIDE:
                hsInstance->setStereoscopicModeByName(port_name,VP_STEREOSCOPIC_3D_SBS);
                break;
            case dsVIDEO_SSMODE_3D_TOP_AND_BOTTOM: 
                hsInstance->setStereoscopicModeByName(port_name,VP_STEREOSCOPIC_2D);
                break;
            default:
                hsInstance->setStereoscopicModeByName(port_name,VP_STEREOSCOPIC_2D);
                break;
        }

        //Skip the framerate for now, as we don't have mapping on this now.

        if( pVideoResolution->interlaced  == VP_SCAN_INTERLACED)
            hsInstance->setScanByName(port_name, VP_SCAN_INTERLACED);
        else
            hsInstance->setScanByName(port_name, VP_SCAN_PROGRESSIVE);
        return dsERR_NONE;
    }
    return dsERR_GENERAL;
}

dsError_t getCurrentAudioMode(dsAudioPortType_t audioPort, dsAudioStereoMode_t* pStereoMode)
{
    OCAPHostData* hsInstance = OCAPHostData::getInstance(_filename);
    dsError_t ret = dsERR_GENERAL;

    if(hsInstance)
    {
        const char *port_name;

        if(audioPort == dsAUDIOPORT_TYPE_HDMI)
            port_name = "HDMI_PORT";
        else
            port_name = "SPDIF_PORT";

        switch(hsInstance->getAudioPortStereoModeByName(port_name))
        {
            case AP_STEREOMODE_STEREO:
                *pStereoMode = dsAUDIO_STEREO_STEREO;
                ret = dsERR_NONE;
                break;                 
            case AP_STEREOMODE_SURROUND:
                *pStereoMode = dsAUDIO_STEREO_SURROUND;
                ret = dsERR_NONE;
            default:; //skip any thing else 
        }
    }
    return ret;
}

dsError_t setCurrentAudioMode(dsAudioPortType_t audioPort, dsAudioStereoMode_t stereoMode)
{
    OCAPHostData* hsInstance = OCAPHostData::getInstance(_filename);
    dsError_t ret = dsERR_GENERAL;

    if(hsInstance)
    {
        const char *port_name;

        if(audioPort == dsAUDIOPORT_TYPE_HDMI)
            port_name = "HDMI_PORT";
        else
            port_name = "SPDIF_PORT";

        switch(stereoMode)
        {
            case dsAUDIO_STEREO_STEREO:
                hsInstance->setAudioPortStereoModeByName(port_name,AP_STEREOMODE_STEREO);
                ret = dsERR_NONE;
                break;                 
            case dsAUDIO_STEREO_SURROUND:
                hsInstance->setAudioPortStereoModeByName(port_name,AP_STEREOMODE_SURROUND);
                ret = dsERR_NONE;
            default:; //skip any thing else 
        }
    }
    return ret;

}

dsError_t commitChanges()
{

    OCAPHostData* hsInstance = OCAPHostData::getInstance(_filename);

    if(hsInstance)
    {
        return hsInstance->commit() ? dsERR_NONE:dsERR_GENERAL;
    }
    return dsERR_GENERAL; 
}
