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
#include <unistd.h>

typedef struct _properties_t
{
    char *key;
    char *val;
    struct _properties_t *pNext;        
} properties;

properties *gProps = NULL;
bool modified = false;

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


#define MAX_LINE_LEN 2050

int main(int argc, char* argv[])
{

    dsError_t ret;
    dsVideoPortResolution_t resolution;
    dsAudioStereoMode_t stereoMode;
    FILE *file = NULL;
    properties *tmpPtr = NULL;

    if(argc < 3)
    {
        printf("Invalid arguments..\n");
        printf("Usage is:\n");
        printf("\t%s <path_to_OCAP_host_data_file> <path_to_DS_host_data_file>\n",argv[0]);
        return -1;
    }
    
    if((ret = init(argv[1])) != 0 )
    {
        printf("Error initiliazing.. error code : %d\n",ret - dsERR_GENERAL);
        return -2;
    }

    file = fopen (argv[2],"r");

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

    if((ret = getCurrentResolution(dsVIDEOPORT_TYPE_HDMI,&resolution )) != 0)
    {
        
        printf("Error retriving resoultion.. error code : %d\n",ret);
        return -3;
    }

    if(!get_property("HDMI0.resolution"))
    {
        add_property("HDMI0.resolution", resolution.name);
        modified = true;
    }

    if((ret = getCurrentResolution(dsVIDEOPORT_TYPE_COMPONENT,&resolution )) != 0)
    {
        
        printf("Error retriving resoultion.. error code : %d\n",ret);
        return -3;
    }

    if(!get_property("COMPONENT0.resolution"))
    {
        add_property("COMPONENT0.resolution", resolution.name);
		add_property("Baseband0.resolution", resolution.name);
        modified = true;
    }
    
    if((ret = getCurrentAudioMode(dsAUDIOPORT_TYPE_HDMI,&stereoMode)) != 0)
    {
        
        printf("Error retriving HDMI stereoMode.. error code : %d\n",ret);
        return -3;
    }

    if(!get_property("HDMI0.AudioMode"))
    {
        switch(stereoMode)
        {
            case dsAUDIO_STEREO_STEREO:
                add_property("HDMI0.AudioMode", "STEREO");
                break;
            case dsAUDIO_STEREO_SURROUND:
                add_property("HDMI0.AudioMode", "SURROUND");
                break;
            default:; //skip others		
        }
        modified = true;
    }

    if((ret = getCurrentAudioMode(dsAUDIOPORT_TYPE_SPDIF,&stereoMode)) != 0)
    {
        
        printf("Error retriving SPDIF stereoMode.. error code : %d\n",ret);
        return -3;
    }

    if(!get_property("SPDIF0.AudioMode"))
    {
        switch(stereoMode)
        {
            case dsAUDIO_STEREO_STEREO:
                add_property("SPDIF0.AudioMode", "STEREO");
                break;
            case dsAUDIO_STEREO_SURROUND:
                add_property("SPDIF0.AudioMode", "SURROUND");
                break;
            default:; //skip others
        }
        modified = true;
    }

    if(modified)
    {
        unlink(argv[2]);
        file = fopen (argv[2],"w");

        if (file > 0)
        {

            char tmp[MAX_LINE_LEN];
            unsigned int size;
            tmpPtr = gProps;
            while(tmpPtr)
            {
                sprintf(tmp,"%s\t%s\n",tmpPtr->key, tmpPtr->val);
                size = strlen(tmp); 
                fwrite(tmp,1,size,file);
                tmpPtr = tmpPtr->pNext;
            }
            fflush (file); //Flush buffers to FS
            fsync(fileno(file)); // Flush file to HDD
            fclose (file);
            printf("successfully saved device settings hostData to file : %s\n",argv[2]);
        }
        else
        {
           printf("Unable to open file: %s for writing..\n",argv[2]);
           return -4;
        }
    }
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
