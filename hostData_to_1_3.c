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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dsOCAPAdapter.h>
#include <dsVideoResolutionSettings.h>
#include <unistd.h>

typedef struct _properties_t
{
    char *key;
    char *val;
    struct _properties_t *pNext;        
} properties;

properties *gProps = NULL;

void add_property(const char *key, const char *val)
{
    properties *tmp = (properties *)malloc(sizeof(properties));
    
    tmp->key = strdup(key);
    tmp->val = strdup(val);
    
    tmp->pNext = gProps;
    gProps = tmp;
}

char* get_property(const char *key)
{
    properties *tmpPtr = gProps;

    while(tmpPtr)
    {
        if(strcmp(tmpPtr->key,key) == 0)
            return tmpPtr->val;
        tmpPtr = tmpPtr->pNext;
    }

    return NULL;
}

int fill_resolution(char *res_string, dsVideoPortResolution_t *pResolution)
{
    int available_resolutions = sizeof(kResolutions)/sizeof(kResolutions[0]);

    int i;

    if(!res_string)
        return -1;

    for(i = 0; i < available_resolutions; i++)
    {
        if(strcmp(kResolutions[i].name,res_string) == 0)
        {
            memcpy(pResolution,&kResolutions[i],sizeof(dsVideoPortResolution_t));
            return 0;
        }
    }

    return -1;
}

int main(int argc, char* argv[])
{
    dsError_t ret;
    dsVideoPortResolution_t resolution;
    FILE *file = NULL;
    properties *tmpPtr = NULL;

    char *pTmpVal = NULL;

    if(argc < 3)
    {
        printf("Invalid arguments..\n");
        printf("Usage is:\n");
        printf("\t%s <path_to_DS_host_data_file> <path_to_OCAP_host_data_file>\n",argv[0]);
        return -1;
    }

    unlink(argv[2]);
    file = fopen (argv[2],"w");

    fclose(file);  //we just want to touch it.

    if((ret = init(argv[2])) != 0 )
    {
        printf("Error initiliazing.. error code : %d\n",ret - dsERR_GENERAL);
        return -2;
    }

    memset(&resolution,0,sizeof(dsVideoPortResolution_t));

    file = fopen (argv[1],"r");

    if(file > 0)
    {
        char keyValue[1024]  = "";
        char key[1024] = "";

        while (!feof(file))
        {
            fscanf (file, "%s\t%s\n", key, keyValue);
            /* Check the TypeOfInput variable and then call the appropriate insert function */
            add_property(key, keyValue);
        }
        
        fclose(file);
    }
    else
    {
       printf("Unable to find the DS persistant file : %s\n", argv[1]);
       return -3;
    }
    
    if((pTmpVal = get_property("HDMI0.resolution")) != NULL)
    {
        if(fill_resolution(pTmpVal,&resolution) == 0)
        {
           if(setCurrentResolution(dsVIDEOPORT_TYPE_HDMI, &resolution) != dsERR_NONE)
            {
                printf("Some error in saving HDMI resolution..");
            } 
        }
        else
        {
            printf("Invalid resolution for HDMI..\n");
        }
    }

    if((pTmpVal = get_property("COMPONENT0.resolution")) != NULL)
    {
        if(fill_resolution(pTmpVal,&resolution) == 0)
        {
           if(setCurrentResolution(dsVIDEOPORT_TYPE_COMPONENT, &resolution) != dsERR_NONE)
            {
                printf("Some error in saving Component resolution..");
            } 
        }
        else
        {
            printf("Invalid resolution for Component..\n");
        }
    }

    if((pTmpVal = get_property("HDMI0.AudioMode")) != NULL)
    {
        if(strcmp(pTmpVal,"STEREO") == 0)
        {
            setCurrentAudioMode(dsAUDIOPORT_TYPE_HDMI,dsAUDIO_STEREO_STEREO);
        }
    
        if(strcmp(pTmpVal,"SURROUND") == 0)
        {
            setCurrentAudioMode(dsAUDIOPORT_TYPE_HDMI,dsAUDIO_STEREO_SURROUND);
        }
    }

    if((pTmpVal = get_property("SPDIF0.AudioMode")) != NULL)
    {
        if(strcmp(pTmpVal,"STEREO") == 0)
        {
            setCurrentAudioMode(dsAUDIOPORT_TYPE_SPDIF,dsAUDIO_STEREO_STEREO);
        }
    
        if(strcmp(pTmpVal,"SURROUND") == 0)
        {
            setCurrentAudioMode(dsAUDIOPORT_TYPE_SPDIF,dsAUDIO_STEREO_SURROUND);
        }
    }

    commitChanges();
    
    tmpPtr = gProps;
	
    while(tmpPtr)
    {
        gProps = tmpPtr;
        tmpPtr = tmpPtr->pNext;
        free(gProps->key);
        free(gProps->val);
        free(gProps);
    }
    return 0;
}
