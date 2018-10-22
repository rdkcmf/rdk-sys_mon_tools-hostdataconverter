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

#ifndef SIMPLECOLLECTIONS_H
#define SIMPLECOLLECTIONS_H

#define SC_UNUSED(x) ((void)x)

#define SCOBJECT(o) ((SCObject*)o)
#define SCSTRING(o) ((SCString*)o)
#define SCINTEGER(o) ((SCInteger*)o)
#define SCHASHTABLE(o) ((SCHashTable*)o)
#define SCLIST(o) ((SCList*)o)
#define SCARRAY(o) ((SCArray*)o)

#define SCVALID(o) ((((SCObject*)o)->type > SCObjectType_none)&&(((SCObject*)o)->type < SCObjectType_array))

typedef enum _SCObjectType
{
    SCObjectType_none,
    SCObjectType_string,
    SCObjectType_encodedScalar,
    SCObjectType_integer,
    SCObjectType_hashTable,
    SCObjectType_list,
    SCObjectType_array
} SCObjectType;

typedef struct _SCObject
{
    SCObjectType type;
    struct _SCObject *next;
    struct _SCObject *prev;
} SCObject;

typedef struct _SCInteger
{
   SCObject object;
   int value;
 } SCInteger;

typedef struct _SCString
{
    SCObject object;
    int len;
    int ownMem;
    unsigned int hash;
    const char *text;
} SCString;

typedef struct _SCHashElement
{
    struct _SCHashElement *next;
    struct _SCHashElement *prev;
    SCString *key;
    SCObject *value;
} SCHashElement;

typedef struct _SCHashElementBin
{
    struct _SCHashElement *first;
    struct _SCHashElement *last;    
} SCHashElementBin;

#define SC_HASH_BINS (63)
typedef struct _SCHashTable
{
    SCObject object;
    unsigned int count;
    SCHashElementBin elements[SC_HASH_BINS];
} SCHashTable;

typedef struct _SCHashTableEnum
{
    SCHashTable *hashTable;
    int bin;
    SCHashElement *element;
} SCHashTableEnum;

typedef struct _SCList
{
    SCObject object;
    int count;
    SCObject *first;
    SCObject *last;
    int indexLastAccessed;
    SCObject *objLastAccessed;
} SCList;

typedef struct _SCArray
{
    SCObject object;
    int elementType;
    int elementSize;
    int capacity;
    int count;
    bool elementsAreObjects;
    void *elementData;
} SCArray;

//------------------------------------------------------------------
// String functions
// Strings refer to the text in-place
//------------------------------------------------------------------
SCString *SCStringCreate(const char *text, int len= -1);
void SCStringDestroy( SCString *str );
SCString *SCStringInit( SCString *str, const char *text, int len= -1 );
void SCStringTerm( SCString *str );
int SCStringCompare( SCString *str1, SCString *str2 );
int SCStringCompare( SCString *str, const char *str2 );
int SCStringToInt( SCString *str );

//------------------------------------------------------------------
// Integer functions
//------------------------------------------------------------------
SCInteger *SCIntegerCreate( int value );
void SCIntegerDestroy( SCInteger *integer );
SCInteger *SCIntegerInit( SCInteger *integer, int value );
void SCIntegerTerm( SCInteger *integer );

//------------------------------------------------------------------
// Hashtable functions
//------------------------------------------------------------------
SCHashTable *SCHashTableCreate();
void SCHashTableDestroy( SCHashTable *hashtable );
SCHashTable *SCHashTableInit( SCHashTable *hashtable );
void SCHashTableTerm( SCHashTable *hashtable );
inline unsigned int SCHashTableGetElementCount( SCHashTable *hashtable )
{
    return hashtable->count;
}
int SCHashTableAddValue( SCHashTable *hashtable, SCString *key, SCObject *value );
bool SCHashTableContainsKey( SCHashTable *hashtable, SCString *key );
SCObject *SCHashTableGetValue( SCHashTable *hastable, SCString *key );
SCHashTableEnum *SCHashTableEnumInit( SCHashTable *hashtable, SCHashTableEnum *en );
bool SCHashTableEnumNext( SCHashTableEnum *en, SCString **key, SCObject **value );
void SCHashTableDump( SCHashTable *hashtable );

//------------------------------------------------------------------
// List functions
//------------------------------------------------------------------
SCList *SCListCreate();
void SCListDestroy( SCList *list );
SCList *SCListInit( SCList *list );
void SCListTerm( SCList *list );
inline unsigned int SCListGetElementCount( SCList *list )
{
    return list->count;
}
void SCListAppendValue( SCList *list, SCObject *value );
SCObject* SCListGetElementAtIndex( SCList *list, int index );
void SCListDump( SCList *list );

//------------------------------------------------------------------
// Array functions
//------------------------------------------------------------------
SCArray *SCArrayCreate( int elementSize, int initialCapacity );
SCArray *SCArrayCreateForObjects( int initialCapacity );
void SCArrayDestroy( SCArray *array );
void SCArrayTerm( SCArray *array );
inline unsigned int SCArrayGetElementCount( SCArray *array )
{
   return array->count;
}
void SCArraySetElementAtIndex( SCArray *array, int index, void *elementData );
void SCArrayGetElementAtIndex( SCArray *array, int index, void *elementData );
void SCArrayDump( SCArray *array );

#endif

