/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCBaseTypes.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include <assert.h>

#ifdef ANDROID
#include "CCJNI.h"
#endif


#ifdef DEBUGON

void CCDebugAssert(const bool condition, const char *file, const int line, const char *message)
{
    if( !condition )
    {
        DEBUGLOG( "%s %i \n", file, line );
        fflush( stdout );
        if( message )
        {
            DEBUGLOG( "ASSERT: %s \n", message );
        }

        // Root call to Java on Android for ease of debugging
#ifdef ANDROID
        CCJNI::Assert( file, line, message );
#endif
        assert( condition );
    }
}

#endif



// Depreciated, use CCList
const bool RemoveFromList(const void *object, void **list, int *length)
{
    int index = -1;
    for( int i=0; i<*length; ++i )
    {
        if( list[i] == object )
        {
            index = i;
            break;
        }
    }

    if( index != -1 )
    {
        ASSERT( *length >= 0 );
        (*length)--;
        for( int i=index; i<*length; ++i )
        {
            list[i] = list[i+1];
        }

        return true;
    }

        return false;
}
