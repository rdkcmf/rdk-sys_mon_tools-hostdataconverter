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

#ifndef _OBJECTINPUTSTREAM_H
#define _OBJECTINPUTSTREAM_H

#include "simplecollections.h"
#include "objectstreamtypes.h"

typedef struct _ObjectInputStreamCtx ObjectInputStreamCtx;

#define READTYPE_DEFAULT              (0)
#define READTYPE_CUSTOM               (1)
#define READTYPE_DEFAULT_THEN_CUSTOM  (2)

typedef int (*ObjectInputStreamReadCheckCallback)( ObjectInputStreamCtx *pOISCtx, SCObject *obj, SCString *className );
typedef bool (*ObjectInputStreamReadObjectCallback)( ObjectInputStreamCtx *pOISCtx, SCObject *obj, SCString *className, int blockLen );

ObjectInputStreamCtx* ObjectInputStreamCtxCreate( ObjectInputStreamReadCheckCallback pCheckCallback, 
                                                  ObjectInputStreamReadObjectCallback pReadCallback );
void ObjectInputStreamCtxDestroy( ObjectInputStreamCtx *pOISCtx );
bool ObjectInputStreamCtxIsError( ObjectInputStreamCtx *pOISCtx );
void ObjectInputStreamCtxReset( ObjectInputStreamCtx *pOISCtx );
int32 ObjectInputStreamReadObject( ObjectInputStreamCtx *pOISCtx, char *data, int offset, int length, SCObject **obj );
bool ObjectInputStreamReadBoolean( ObjectInputStreamCtx *pOISCtx );
char ObjectInputStreamReadByte( ObjectInputStreamCtx *pOISCtx );
short ObjectInputStreamReadChar( ObjectInputStreamCtx *pOISCtx );
short ObjectInputStreamReadShort( ObjectInputStreamCtx *pOISCtx );
int ObjectInputStreamReadInt( ObjectInputStreamCtx *pOISCtx );
float ObjectInputStreamReadFloat( ObjectInputStreamCtx *pOISCtx );
long long ObjectInputStreamReadLong( ObjectInputStreamCtx *pOISCtx );
double ObjectInputStreamReadDouble( ObjectInputStreamCtx *pOISCtx );
SCString* ObjectInputStreamReadString( ObjectInputStreamCtx *pOISCtx );

#endif

