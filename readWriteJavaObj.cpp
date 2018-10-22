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
#include "readWriteJavaObj.h"

using namespace std;


SCObject* readHostData( const char *fileName )
{
   FILE *pInput= 0;
   int lengthDidRead;
   int rc;
   char *fileData= 0;
   int fileSize;
   ObjectInputStreamCtx *pOISCtx= 0;
   SCObject *obj= 0;

   pInput= fopen( fileName, "rb" );
   if ( !pInput )
   {
      printf( "Error: unable to open input file %s\n", fileName );
      goto exit;
   }

   rc= fseek( pInput, 0, SEEK_END );
   if ( rc )
   {
      printf( "Error: unable to seek to end of input file\n");
      goto exit;
   }
   
   fileSize= ftell( pInput );
   
   rc= fseek( pInput, 0, SEEK_SET );
   if ( rc )
   {
      printf( "Error: unable to seek to start of input file\n");
      goto exit;
   }
   
   fileData= (char*)malloc( fileSize*sizeof(char) );
   if ( !fileData )
   {
      printf( "Error: unable to allocate buffer for input file data\n" );
      goto exit;
   }
   
   lengthDidRead= fread( fileData, sizeof(char), fileSize, pInput );
   if ( lengthDidRead != fileSize )
   {
      printf( "Error: unable to read input file data: read %d of %d\n", lengthDidRead, fileSize );
      goto exit;
   }

   pOISCtx= ObjectInputStreamCtxCreate( readCheckCallback, readObjectCallback );
   if (  pOISCtx )
   {
      int32 lUsed;
      int32 lOffset;

      lOffset= 0; 
      while( lOffset < fileSize )
      {
         obj= 0;
         
         lUsed= ObjectInputStreamReadObject( pOISCtx, fileData, lOffset, fileSize, &obj );
         lOffset += lUsed;
         
         if ( ObjectInputStreamCtxIsError( pOISCtx ) )
         {
             printf("Error encountered in object input stream\n" );
             break;
         }
         
         if ( obj )
         {
            goto exit;
         }
      }
               
      ObjectInputStreamCtxDestroy( pOISCtx );
   }
   
exit:

   if ( fileData )
   {
      free( fileData );
   }

   if ( pInput )
   {
      fclose( pInput );
      pInput= 0;
   }

   return obj;   
}

bool writeHostData( const char *fileName, bool appendCheckSum, SCObject *objHostData )
{
   bool result= false;
   FILE *pOutput= 0;
   ObjectOutputStreamCtx *pOOSCtx= 0;

   pOutput= fopen( fileName, "wb" );
   if ( pOutput )
   {
      pOOSCtx= ObjectOutputStreamCtxCreate( writeCheckCallback, writeObjectCallback );
      if ( pOOSCtx )
      {
         ObjectOutputStreamWriteObject( pOOSCtx, objHostData );

         int dataLen= ObjectOutputStreamGetDataLength( pOOSCtx );
         int dataAllocLen= dataLen;
         if ( appendCheckSum )
         {
            dataAllocLen += 8;
         }
         unsigned char* data= (unsigned char*)malloc( dataAllocLen );
         if ( data )
         {
            ObjectOutputStreamGetData( pOOSCtx, data, 0, dataLen );
            if ( appendCheckSum )
            {
               unsigned int A= 1;
               unsigned int B= 0;
               long long C;
               for( int i= 0; i < dataLen; ++i )
               {
                  A= ((A+data[i]) % 65521);
                  B= ((B+A) % 65521);
               }
               C= (((long long)B)<<16)+A;
               data[dataLen+0]= 0x0;
               data[dataLen+1]= 0x0;
               data[dataLen+2]= 0x0;
               data[dataLen+3]= 0x0;
               data[dataLen+4]= ((C >> 24)&0xFF);
               data[dataLen+5]= ((C >> 16)&0xFF);
               data[dataLen+6]= ((C >> 8)&0xFF);
               data[dataLen+7]= ((C)&0xFF);
               dataLen += 8;
            }
            fwrite( data, 1, dataLen, pOutput );
         }
         ObjectOutputStreamCtxDestroy( pOOSCtx );
         
         result= true;
      }     
      
      fclose( pOutput );
   }
   
   return result;
}

int readCheckCallback( ObjectInputStreamCtx *pOISCtx, SCObject *obj, SCString *className )
{
   int readType;

   printf( "readCheckCallback: obj %p className %s\n", obj, className->text );
   
   return readType;
}

bool readObjectCallback( ObjectInputStreamCtx *pOISCtx, SCObject *obj, SCString *className, int blockLen )
{
    bool result= false;
    
    {
       printf("Error: readObjectCallBack called for unknonw class (%.*s)\n", className->len, className->text );
    }
   
   return result;
}

int writeCheckCallback( ObjectOutputStreamCtx *pOOSCtx, SCObject *obj, SCString *className )
{
   int writeType;

   printf("writeCheckCallback: obj %p className (%.*s)\n", obj, className->len, className->text );
   
   return writeType;
}

bool writeObjectCallback( ObjectOutputStreamCtx *pOOSCtx, SCObject *obj, SCString *className )
{
   bool result= false;
   {
      printf( "writeObjectCallback called with unknown class (%.*s)\n", className->len, className->text );
   }
   
   return result;
}


