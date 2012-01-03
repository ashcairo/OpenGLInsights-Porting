/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCBaseTypes.h
 * Description : Contains base structures.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCBASETYPES_H__
#define __CCBASETYPES_H__


#include "CCBaseTools.h"


enum CCResourceType
{
    Resource_Cached,
    Resource_Packaged
};


class CCBaseType
{
public:
    virtual ~CCBaseType();
    virtual void destruct() = 0;
};


class CCUpdater : public CCBaseType
{
public:
    typedef CCBaseType super;

    CCUpdater()
    {
#ifdef DEBUGON
        destructCalled = false;
#endif
    }

    virtual ~CCUpdater()
    {
#ifdef DEBUGON
        ASSERT( destructCalled );
#endif
    }

    virtual void destruct()
    {
        #ifdef DEBUGON
            destructCalled = true;
        #endif
    }

    virtual const bool update(const float delta) = 0;

#ifdef DEBUGON
protected:
    bool destructCalled;
#endif
};


// A list containing pointers which aren't deleted on release
template <typename T> class CCList
{
public:
    CCList(const uint size=0)
    {
        list = NULL;
        length = 0;
        allocated = 0;

        if( size > 0 )
        {
            allocate( size );
        }
    }

    ~CCList()
    {
        freeList();
    }

    void allocate(const int size)
    {
        allocated = size;

        if( list != NULL )
        {
            ::free( list );
        }

        const uint sizeOfPointer = sizeof( void* );
        list = (T**)malloc( sizeOfPointer * allocated );
        ASSERT( list != NULL );
    }

    void add(T *object)
    {
        //ASSERT( find( object ) == -1 );
        if( length == allocated )
        {
            allocated += 16;
            const uint sizeOfPointer = sizeof( void* );
            T **newArray = (T**)malloc( sizeOfPointer * allocated );
            ASSERT( newArray != NULL );

            if( list != NULL )
            {
                for( int i=0; i<length; ++i )
                {
                    newArray[i] = list[i];
                }

                ::free( list );
            }

            newArray[length++] = object;
            list = newArray;
        }
        else
        {
            list[length++] = object;
        }
    }

    const bool remove(const T *object)
    {
        int index = find( object );
        if( index != -1 )
        {
            ASSERT( length > 0 );
            length--;
            for( int i=index; i<length; ++i )
            {
                list[i] = list[i+1];
            }

            return true;
        }

        return false;
    }

    void freeList()
    {
        if( list != NULL )
        {
            free( list );
            list = NULL;
        }
        length = 0;
        allocated = 0;
    }

    void deleteObjects()
    {
        for( int i=0; i<length; ++i )
        {
            delete list[i];
        }
        length = 0;
    }

    void deleteObjectsAndList()
    {
        deleteObjects();
        freeList();
    }
    
    void resize(const int size)
    {
        // If it's too small
        while( length < size )
        {
            add( new T() );
        }
    
        // If it's too big
        for( int i=size; i<length; ++i )
        {
            delete list[i];
        }
        length = size;
    }
    
    void copy(CCList<T> &other)
    {
        for( int i=0; i<other.length; ++i )
        {
            add( other.list[i] );
        }
    }
    
    void push(T *object)
    {
        add( object );
        reinsert( object, 0 );
    }
    
    T* pop()
    {
        ASSERT( length > 0 );
        T *object = list[0];
        remove( object );
        return object;
    }
    
    // Re-insert from the back of the queue
    void reinsert(T *object, const int index)
    {
        ASSERT( list[length-1] == object );
        for( int i=length-1; i>index; --i )
        {
            list[i] = list[i-1];
        }
        list[index] = object;
    }
    
    const int find(const T *object)
    {
        for( int i=0; i<length; ++i )
        {
            if( list[i] == object )
            {
                return i;
            }
        }
        return -1;
    }
    
    T* last()
    {
        ASSERT( length > 0 );
        return list[length-1];
    }

    T **list;
    int length;

protected:
    int allocated;
};


// A list containing objects that deleted using the destruct call on release
template <typename T> class CCDestructList : public CCList<T>
{
public:
    void deleteObjects(const bool reversed=false)
    {
        if( reversed )
        {
            for( int i=this->length-1; i>=0; --i )
            {
                T *object = this->list[i];
                DELETE_OBJECT( object );
            }
        }
        else
        {
            for( int i=0; i<this->length; ++i )
            {
                T *object = this->list[i];
                DELETE_OBJECT( object );
            }
        }
    }

	void deleteObjectsAndList(const bool reversed=false)
	{
		if( this->list != NULL )
		{
            deleteObjects( reversed );
            this->freeList();
		}
	}
};


// Always feed on destruct
template <typename T1, typename T2> class CCPairList
{
public:
    void add(T1 *name, T1 *value)
    {
        names.add( name );
        values.add( value );
    }

    const int length() { return names.length; }

    ~CCPairList()
    {
        names.deleteObjects();
        values.deleteObjects();
    }

    CCList< T1 > names;
    CCList< T2 > values;
};


struct CCData
{
    CCData();
    ~CCData();
    
    void setSize(const uint inLength);
    void set(const char *data, const uint inLength);
    
protected:
    void zero();
	
public:
	uint length;
	char *buffer;
    uint bufferSize;
};


struct CCText : CCData
{
    CCText() {}
    explicit CCText(const int inLength);
    CCText(const char *text);
    CCText(const CCText &other);

    const bool operator==(const char *other) const;
    const bool operator!=(const char *other) const;
    CCText& operator=(const char *text);
    CCText& operator=(const CCText &other);
    CCText& operator=(CCText &other);
    CCText& operator+=(const char *other);
    CCText& operator+=(const int value);
    CCText& operator+=(const float value);

    void set(const char *text);
    void clear();
    void ensureLength(const uint minLength, const bool keepBuffer=false);

    CCList<char>* split(const char *token);
    static const bool Equals(const char *buffer, const char *token);
    static const bool Contains(const char *buffer, const char *token);
    static const bool StartsWith(const char *buffer, const char *token);
    void stripExtension();
    void stripDirectory();
    void toLowercase();
    static void SetLastWord(const char *inBuffer, CCText &outText);

    void replaceChar(const char search, const char replace);

    // Set the text to be the value between the split tokens
    void splitBetween(CCText &source, const char *from, const char *to);
    void splitBefore(CCText &source, const char *before);
    void splitAfter(CCText &source, const char *after);
    void removeBetween(const char *from, const char *to);
    void removeBetweenIncluding(const char *from, const char *to);
};


#endif // __CCBASETYPES_H__
