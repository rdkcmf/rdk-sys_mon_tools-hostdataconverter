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

#ifndef _OBJECT_H
#define _OBJECT_H

#include "simplecollections.h"

// -----------------------------------------------------------------------------------------------------------------
// Required basic types

typedef char int8;
typedef unsigned char uint8;
typedef short int int16;
typedef unsigned short uint16;
typedef long int int32;
typedef unsigned long int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef struct _UtfString
{
   const int8 *str;
   int len;
} UtfString;


typedef enum _ObjectType
{
   ObjectType_noObject,
   ObjectType_boolean,
   ObjectType_byte,
   ObjectType_char,
   ObjectType_short,
   ObjectType_int,
   ObjectType_float,
   ObjectType_long,
   ObjectType_double,
   ObjectType_string,
   ObjectType_array,
   ObjectType_object
} ObjectType;

typedef SCHashTableEnum ObjectEnum;

#define FLAGS_WRITE_METHOD         0x01
#define FLAGS_SERIALIZABLE         0x02
#define FLAGS_EXTERNALIZABLE       0x04
#define FLAGS_BLOCK_DATA           0x08

SCObject *ObjectCreate( const char *className, long long SUID, unsigned char flags= FLAGS_SERIALIZABLE, SCObject *superClass= 0 );
SCObject *ObjectCreate( SCString *strClassName, long long SUID, unsigned char flags= FLAGS_SERIALIZABLE, SCObject *superClass= 0 );
void ObjectDestroy( SCObject *obj );
SCString* ObjectGetClassName( SCObject *obj );
long long ObjectGetClassSUID( SCObject *obj );
unsigned char ObjectGetClassFlags( SCObject *obj );
SCObject* ObjectGetSuperClass( SCObject *obj );
SCObject* ObjectGetSuperClassByIndex( SCObject *obj, int index );
bool ObjectCompatibleClass( SCObject *obj, const char *className );
bool ObjectIsArray( SCObject *obj );
int ObjectGetFieldCount( SCObject *obj );
bool ObjectContainsField( SCObject *obj, const char *fieldName );
int32 ObjectGetFieldType( SCObject *obj, const char *fieldName );
int32 ObjectGetFieldType( SCObject *obj, SCString *stFieldName );
bool ObjectGetFieldValueBoolean( SCObject *obj, const char *fieldName, bool noinherit= false, bool *error= 0 );
bool ObjectGetFieldValueBoolean( SCObject *obj, SCString *strFieldName, bool noinherit= false, bool *error= 0 );
char ObjectGetFieldValueByte( SCObject *obj, const char *fieldName, bool noinherit= false, bool *error= 0 );
char ObjectGetFieldValueByte( SCObject *obj, SCString *strFieldName, bool noinherit= false, bool *error= 0 );
unsigned short ObjectGetFieldValueChar( SCObject *obj, const char *fieldName, bool noinherit= false, bool *error= 0 );
unsigned short ObjectGetFieldValueChar( SCObject *obj, SCString *strFieldName, bool noinherit= false, bool *error= 0 );
short ObjectGetFieldValueShort( SCObject *obj, const char *fieldName, bool noinherit= false, bool *error= 0 );
short ObjectGetFieldValueShort( SCObject *obj, SCString *strFieldName, bool noinherit= false, bool *error= 0 );
int ObjectGetFieldValueInt( SCObject *obj, const char *fieldName, bool noinherit= false, bool *error= 0 );
int ObjectGetFieldValueInt( SCObject *obj, SCString *strFieldName, bool noinherit= false, bool *error= 0 );
float ObjectGetFieldValueFloat( SCObject *obj, const char *fieldName, bool noinherit= false, bool *error= 0 );
float ObjectGetFieldValueFloat( SCObject *obj, SCString *strFieldName, bool noinherit= false, bool *error= 0 );
long long ObjectGetFieldValueLong( SCObject *obj, const char *fieldName, bool noinherit= false, bool *error= 0 );
long long ObjectGetFieldValueLong( SCObject *obj, SCString *strFieldName, bool noinherit= false, bool *error= 0 );
double ObjectGetFieldValueDouble( SCObject *obj, const char *fieldName, bool noinherit= false, bool *error= 0 );
double ObjectGetFieldValueDouble( SCObject *obj, SCString *strFieldName, bool noinherit= false, bool *error= 0 );
SCString* ObjectGetFieldValueString( SCObject *obj, const char *fieldName, bool noinherit= false, bool *error= 0 );
SCString* ObjectGetFieldValueString( SCObject *obj, SCString *strFieldName, bool noinherit= false, bool *error= 0 );
SCObject* ObjectGetFieldValueObject( SCObject *obj, const char *fieldName, bool noinherit= false, bool *error= 0 );
SCObject* ObjectGetFieldValueObject( SCObject *obj, SCString *strFieldName, bool noinherit= false, bool *error= 0 );
bool ObjectGetFieldByIndex( SCObject *obj, int index, SCString **fieldName, SCString **fieldType, SCObject **fieldValue );
ObjectEnum *ObjectEnumFieldsInit( SCObject *obj, ObjectEnum *objEnum );
bool ObjectEnumFieldsNext( ObjectEnum *objEnum, SCString **name, SCObject **obj );
int ObjectGetArrayElementCount( SCObject *array );
int ObjectGetArrayElementDataType( SCObject *array );
int ObjectGetArrayElementDataSize( SCObject *array );
bool ObjectGetArrayElementData( SCObject *array, void *buffer, int len );
bool ObjectGetArrayElementAtIndex( SCObject *array, int index, void *element );
bool ObjectSetArrayElementAtIndex( SCObject *array, int index, void *element );
bool ObjectAddFieldBoolean( SCObject *obj, const char *name, bool value );
bool ObjectAddFieldBoolean( SCObject *obj, SCString *strName, bool value );
bool ObjectAddFieldByte( SCObject *obj, const char *name, char value );
bool ObjectAddFieldByte( SCObject *obj, SCString *strName, char value );
bool ObjectAddFieldChar( SCObject *obj, const char *name, short value );
bool ObjectAddFieldChar( SCObject *obj, SCString *strName, short value );
bool ObjectAddFieldShort( SCObject *obj, const char *name, short value );
bool ObjectAddFieldShort( SCObject *obj, SCString *strName, short value );
bool ObjectAddFieldInt( SCObject *obj, const char *name, int value );
bool ObjectAddFieldInt( SCObject *obj, SCString *strName, int value );
bool ObjectAddFieldFloat( SCObject *obj, const char *name, float value );
bool ObjectAddFieldFloat( SCObject *obj, SCString *strName, float value );
bool ObjectAddFieldLong( SCObject *obj, const char *name, long long value );
bool ObjectAddFieldLong( SCObject *obj, SCString *strName, long long value );
bool ObjectAddFieldDouble( SCObject *obj, const char *name, double value );
bool ObjectAddFieldDouble( SCObject *obj, SCString *strName, double value );
bool ObjectAddFieldString( SCObject *obj, const char *name, SCString *value );
bool ObjectAddFieldString( SCObject *obj, SCString *strName, SCString *value );
bool ObjectAddFieldArray( SCObject *obj, const char *name, ObjectType type, int count, const char *typeName= 0 );
bool ObjectAddFieldArray( SCObject *obj, SCString *strName, ObjectType type, int count, SCString *strType= 0 );
bool ObjectAddFieldObject( SCObject *obj, const char *name, const char *type, SCObject *value );
bool ObjectAddFieldObject( SCObject *obj, SCString *strName, SCString *strType, SCObject *value );
void ObjectDump( SCObject *obj );

#endif

