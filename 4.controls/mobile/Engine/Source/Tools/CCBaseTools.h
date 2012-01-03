/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCBaseTypes.h
 * Description : Contains base functions.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __GLOBALTOOLS_H__
#define __GLOBALTOOLS_H__


typedef	unsigned int uint;


// add GCC_PREPROCESSOR_DEFINITIONS DEBUGON in Project Settings
#define ALLOW_DEBUGLOG
#if defined DEBUGON && defined ALLOW_DEBUGLOG

#define LOG_FPS 1
#define DEBUGLOG printf

#define LOG_NEWMAX(string, currentMax, newMax) \
if( currentMax < newMax ) { currentMax = newMax; printf( "%s - %i \n", string, currentMax ); }

#else

#define DEBUGLOG
#define LOG_NEWMAX(string, currentMax, newMax)

#endif


#ifdef DEBUGON

void CCDebugAssert(const bool condition, const char *file, const int line, const char *message=NULL);

#define ASSERT(condition) CCDebugAssert( condition, __FILE__, __LINE__ );

#define ASSERT_MESSAGE(condition, message) CCDebugAssert( condition, __FILE__, __LINE__, message );

#define DEBUG_OPENGL() \
{                                   \
    GLenum error = glGetError();    \
    ASSERT( error ==  0 );          \
}

#else // DEBUGON

inline void DebugAssert(const bool condition, const char *message=NULL) {}
#define ASSERT(cond) {};
#define ASSERT_MESSAGE(cond,str) {}

#define DEBUG_OPENGL() {}

#endif


#define DELETE_OBJECT(object)       \
if( object != NULL )                \
{                                   \
    object->destruct();             \
    delete object;                  \
    object = NULL;                  \
}


#define DELETE_POINTER(pointer)     \
if( pointer != NULL )               \
{                                   \
    delete pointer;                 \
    pointer = NULL;                 \
}


#define FREE_POINTER(pointer)       \
if( pointer != NULL )               \
{                                   \
    free( pointer );                \
    pointer = NULL;                 \
}

const bool RemoveFromList(const void *object, void **list, int *length);


inline const bool HasFlag(const uint source, const uint flag)
{
    const uint result = source & flag;
    if( result != 0 )
    {
        return true;
    }

    return false;
}

inline void AddFlag(uint &source, const uint flag)
{
    if( HasFlag( source, flag ) == false )
    {
        source |= flag;
    }
}

inline void RemoveFlag(uint &source, const uint flag)
{
    if( HasFlag( source, flag ) )
    {
        source ^= flag;
    }
}


extern void CCEngineThreadLock();
extern void CCEngineThreadUnlock();


#endif // __GLOBALTOOLS_H__
