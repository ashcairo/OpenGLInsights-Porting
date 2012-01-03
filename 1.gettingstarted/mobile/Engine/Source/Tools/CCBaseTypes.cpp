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


// CCBaseType
CCBaseType::~CCBaseType()
{
}



// CCData
CCData::CCData()
{
    zero();
}


void CCData::zero()
{
    length = 0;
    buffer = NULL;
    bufferSize = 0;
}


CCData::~CCData()
{
    if( buffer != NULL )
    {
        free( buffer );
    }
}


void CCData::set(const char *data, const uint inLength)
{
    if( inLength > 0 )
    {
        length = inLength+1;
        if( length > bufferSize )
        {
            if( buffer != NULL )
            {
                free( buffer );
            }
            
            // 32 byte aligned
            bufferSize = ( round( length / 32 ) + 1 ) * 32;
            buffer = (char*)malloc( bufferSize );
        }
        memcpy( buffer, data, inLength );
        buffer[inLength] = 0;
    }
    else
    {
        if( buffer != NULL )
        {
            free( buffer );
        }
        zero();
    }
}



// CCText
CCText::CCText(const int inLength)
{
    ensureLength( inLength );
}


CCText::CCText(const char *text)
{
    zero();
    set( text );
}


CCText::CCText(const CCText &other)
{
    zero();
    set( other.buffer );
}


const bool CCText::operator==(const char *other) const
{
    if( length > 0 )
    {
        return ( strcmp( buffer, other ) == 0 );
    }
    return false;
}


const bool CCText::operator!=(const char *other) const
{
    if( length > 0 )
    {
        return ( strcmp( buffer, other ) != 0 );
    }
    return false;
}


CCText& CCText::operator=(const char *text)
{
    set( text );
    return *this;
}


CCText& CCText::operator=(const CCText &other)
{
    if( this != &other )
    {
        if( other.buffer != NULL )
        {
            set( other.buffer );
        }
        else if( buffer != NULL )
        {
            free( buffer );
            buffer = NULL;
            bufferSize = 0;
        }
    }
    return *this;
}


CCText& CCText::operator+=(const char *other)
{
    if( other != NULL )
    {
        if( length == 0 )
        {
            set( other );
        }
        else
        {
            const uint otherLength = strlen( other );
            const uint totalLength = length + otherLength;
            ensureLength( totalLength, true );

            strcat( buffer, other );
            length = totalLength;
        }
    }
    return *this;
}


CCText& CCText::operator+=(const int value)
{
    char other[32];
    sprintf( other, "%i", value );
    *this += other;
    return *this;
}


CCText& CCText::operator+=(const float value)
{
    char other[32];
    sprintf( other, "%f", value );
    *this += other;
    return *this;
}


void CCText::set(const char *text)
{
    if( text != NULL )
    {
        length = strlen( text );
        uint minLength = length < 31 ? 31 : length;
        ensureLength( minLength );

        strcpy( buffer, text );
    }
}


void CCText::clear()
{
    set( "" );
}


void CCText::ensureLength(const uint minLength, const bool keepBuffer)
{
    const uint newSize = sizeof( char ) * ( minLength + 1 );
    if( newSize > bufferSize )
    {
        char *oldBuffer = buffer;
        bufferSize = newSize;
        buffer = (char*)malloc( bufferSize );

        if( oldBuffer != NULL )
        {
            if( keepBuffer )
            {
                strcpy( buffer, oldBuffer );
            }

            free( oldBuffer );
        }
    }
}


CCList<char>* CCText::split(const char *token)
{
    const uint tokenLength = strlen( token );
    CCList<char> *splitList = new CCList<char>();
    if( buffer )
    {
        char *previousFind = NULL;
        char *currentFind = buffer;
        splitList->add( currentFind );
        do
        {
            previousFind = currentFind;
            currentFind = strstr( currentFind, token );
            if( currentFind != NULL )
            {
                *currentFind = 0;
                currentFind += tokenLength;
                if( currentFind != buffer )
                {
                    splitList->add( currentFind );
                }
            }
        } while( currentFind != NULL && currentFind != previousFind );
    }
    return splitList;
}


const bool CCText::Equals(const char *buffer, const char *token)
{
    if( buffer != NULL )
    {
        return strcmp( buffer, token ) == 0;
    }
    return false;
}


const bool CCText::Contains(const char *buffer, const char *token)
{
    if( buffer != NULL )
    {
        return strstr( buffer, token ) != NULL;
    }
    return false;
}


const bool CCText::StartsWith(const char *buffer, const char *token)
{
    if( buffer != NULL )
    {
        const char *find = strstr( buffer, token );
        if( find == buffer )
        {
            return true;
        }
    }
    return false;
}


void CCText::stripExtension()
{
    length -= 4;
    buffer[length] = 0;
}


void CCText::stripDirectory()
{
    CCList<char> *directorySplit = split( "/" );
    if( directorySplit->length > 1 )
    {
        set( directorySplit->list[directorySplit->length-1] );
    }
    DELETE_POINTER( directorySplit );
}


void CCText::toLowercase()
{
	for( uint i=0; i<length; ++i )
	{
		char lowercase = tolower( buffer[i] );
		buffer[i] = lowercase;
	}
}


void CCText::SetLastWord(const char *inBuffer, CCText &outText)
{
    CCText nameData = inBuffer;
    CCList<char> *nameDataSplit = nameData.split( " " );
    ASSERT( nameDataSplit->length > 0 );
    outText = nameDataSplit->list[nameDataSplit->length-1];
    DELETE_POINTER( nameDataSplit );
}


void CCText::replaceChar(const char search, const char replace)
{
    for( uint i=0; i<length; ++i )
    {
        if( buffer[i] == search )
        {
            buffer[i] = replace;
        }
    }
}


void CCText::splitBetween(CCText &source, const char *from, const char *to)
{
    CCList<char> *list1 = source.split( from );
    if( list1->length > 1 )
    {
        CCText text2 = list1->list[1];
        CCList<char> *list2 = text2.split( to );
        set( list2->list[0] );
        DELETE_POINTER( list2 );
    }
    DELETE_POINTER( list1 );
}


void CCText::splitBefore(CCText &source, const char *before)
{
    CCList<char> *list1 = source.split( before );
    set( list1->list[0] );
    DELETE_POINTER( list1 );
}


void CCText::splitAfter(CCText &source, const char *after)
{
    CCList<char> *list1 = source.split( after );
    if( list1->length > 1 )
    {
        set( list1->list[1] );
        for( int i=2; i<list1->length; ++i )
        {
            *this += after;
            *this += list1->list[i];
        }
    }
    DELETE_POINTER( list1 );
}


void CCText::removeBetween(const char *from, const char *to)
{
    CCText start( buffer );
    start.splitAfter( start, to );
    CCText end( buffer );
    end.splitBefore( end, from );

    *this = end.buffer;
    *this += from;
    *this += to;
    *this += start.buffer;
}


void CCText::removeBetweenIncluding(const char *from, const char *to)
{
    CCText start( buffer );
    CCList<char> *list1 = split( from );
    if( list1->length > 1 )
    {
        CCText end( list1->list[1] );
        start.set( list1->list[0] );
        end.splitAfter( end, to );

        set( start.buffer );
        *this += end.buffer;
    }
    DELETE_POINTER( list1 );
}
