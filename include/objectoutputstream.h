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

#ifndef _OBJECTOUTPUTSTREAM_H
#define _OBJECTOUTPUTSTREAM_H

#include "simplecollections.h"
#include "objectstreamtypes.h"

typedef struct _ObjectOutputStreamCtx ObjectOutputStreamCtx;

#define WRITETYPE_DEFAULT              (0)
#define WRITETYPE_CUSTOM               (1)
#define WRITETYPE_DEFAULT_THEN_CUSTOM  (2)

typedef int (*ObjectInputStreamWriteCheckCallback)( ObjectOutputStreamCtx *pOOSCtx, SCObject *obj, SCString *className );
typedef bool (*ObjectOutputStreamWriteObjectCallback)( ObjectOutputStreamCtx *pOOSCtx, SCObject *obj, SCString *className );

ObjectOutputStreamCtx* ObjectOutputStreamCtxCreate( ObjectInputStreamWriteCheckCallback pCheckCallback, 
                                                    ObjectOutputStreamWriteObjectCallback pWriteCallback );
void ObjectOutputStreamCtxDestroy( ObjectOutputStreamCtx *pOOSCtx );
bool ObjectOutputStreamCtxIsError( ObjectOutputStreamCtx *pOOSCtx );
bool ObjectOutputStreamWriteObject( ObjectOutputStreamCtx *pOOSCtx, SCObject *obj );
int ObjectOutputStreamGetDataLength( ObjectOutputStreamCtx *pOOSCtx );
int ObjectOutputStreamGetData( ObjectOutputStreamCtx *pOOSCtx, unsigned char *buffer, int offset, int length );
bool ObjectOutputStreamWriteBoolean( ObjectOutputStreamCtx *pOOSCtx, bool value );
bool ObjectOutputStreamWriteByte( ObjectOutputStreamCtx *pOOSCtx, unsigned char value );
bool ObjectOutputStreamWriteChar( ObjectOutputStreamCtx *pOOSCtx, unsigned short value );
bool ObjectOutputStreamWriteShort( ObjectOutputStreamCtx *pOOSCtx, short value );
bool ObjectOutputStreamWriteInt( ObjectOutputStreamCtx *pOOSCtx, int value );
bool ObjectOutputStreamWriteFloat( ObjectOutputStreamCtx *pOOSCtx, float value );
bool ObjectOutputStreamWriteLong( ObjectOutputStreamCtx *pOOSCtx, long long value );
bool ObjectOutputStreamWriteDouble( ObjectOutputStreamCtx *pOOSCtx, double value );
bool ObjectOutputStreamWriteUTF( ObjectOutputStreamCtx *pOOSCtx, UtfString *pUtf );
bool ObjectOutputStreamWriteUTF( ObjectOutputStreamCtx *pOOSCtx, SCString *str );

#endif

