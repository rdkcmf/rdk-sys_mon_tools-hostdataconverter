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

#ifndef _OBJECTSTREAMTYPES_H
#define _OBJECTSTREAMTYPES_H

#include "object.h"
#include "simplecollections.h"

/*
 * Multi-byte big endian value builder macros.
 */
#define getUInt32(p) (uint32)( (uint32)(((uint8*)(p))[0])<<24 | (uint32)(((uint8*)(p))[1])<<16 | (uint32)(((uint8*)(p))[2])<<8 | (uint32)(((uint8*)(p))[3]) )
#define getUInt24(p) (uint32)( (uint32)(((uint8*)(p))[0])<<16 | (uint32)(((uint8*)(p))[1])<<8 | (uint32)(((uint8*)(p))[2]) )
#define getUInt16(p) (uint16)((((uint8*)(p))[0]<<8)|((uint8*)(p))[1])
#define getInt32(p) (int32)( (uint32)(((uint8*)(p))[0])<<24 | (uint32)(((uint8*)(p))[1])<<16 | (uint32)(((uint8*)(p))[2])<<8 | (uint32)(((uint8*)(p))[3]) )
#define getInt16(p) (int16)((((uint8*)(p))[0]<<8)|((uint8*)(p))[1])
#define getUInt8(p) (uint8)((p)[0])
#define getInt8(p) (int8)((p)[0])



// -----------------------------------------------------------------------------------------------------------------
// Java serialized data stream types


#define STREAM_MAGIC               0xaced
#define STREAM_VERSION             0x0005

#define TC_NULL                    0x70
#define TC_REFERENCE               0x71
#define TC_CLASSDESC               0x72
#define TC_OBJECT                  0x73
#define TC_STRING                  0x74
#define TC_ARRAY                   0x75
#define TC_CLASS                   0x76
#define TC_BLOCKDATA               0x77
#define TC_ENDBLOCKDATA            0x78
#define TC_RESET                   0x79
#define TC_BLOCKDATALONG           0x7a
#define TC_EXCEPTION               0x7b
#define TC_LONGSTRING              0x7c
#define TC_PROXYCLASSDESC          0x7d

#define SC_WRITE_METHOD            0x01
#define SC_BLOCK_DATA              0x08
#define SC_SERIALIZABLE            0x02
#define SC_EXTERNALIZABLE          0x04

#define BASEWIREHANDLE             0x7e0000

// -----------------------------------------------------------------------------------------------------------------
// Common types shared by serialized data reading and writing

typedef enum ObjStrm_ErrCode
{
   ObjStrm_ErrCode_syntax=                  1,
   ObjStrm_ErrCode_outOfMemory=             2,
   ObjStrm_ErrCode_stackUnderflow=          3,
   ObjStrm_ErrCode_badReference=            4,
   ObjStrm_ErrCode_badReferenceType=        5,
   ObjStrm_ErrCode_classNotFound=           6,
   ObjStrm_ErrCode_invalidClass=            7,
   ObjStrm_ErrCode_notSerializable=         8,
   ObjStrm_ErrCode_internal=                9,
   ObjStrm_ErrCode_EOS=                     10
} 
ObjStrm_ErrCode;

typedef enum ObjStrm_RefType
{
   ObjStrm_RefType_null=      (1<<0),
   ObjStrm_RefType_block=     (1<<1),
   ObjStrm_RefType_class=     (1<<2),
   ObjStrm_RefType_classdesc= (1<<3),
   ObjStrm_RefType_string=    (1<<4),
   ObjStrm_RefType_object=    (1<<5),
   ObjStrm_RefType_array=     (1<<6),
   ObjStrm_RefType_number=    (1<<7)
}
ObjStrm_RefType;

typedef struct _ObjStrmRef
{
   int32 lType;
   SCObject *obj;
   void *pObj;
}
ObjStrmRef;


#endif

