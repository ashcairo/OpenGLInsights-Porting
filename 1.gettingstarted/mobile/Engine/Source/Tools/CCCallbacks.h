/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCCallbacks.h
 * Description : Closure implementation using functors.
 *
 * Created     : 25/06/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCCALLBACKS_H__
#define __CCCALLBACKS_H__


// Automatically keeps track of allocations to let you know if it's ok to callback this class
class CCLazyCallback
{
public:
    CCLazyCallback();
    virtual ~CCLazyCallback();
    static const bool isCallbackActive(void *pendingCallback, const long pendingID);
    
    void *lazyPointer;
    long lazyID;
protected:
    static CCList<void> activeCallbacks;
    static long activeID;
};



class CCLambdaCallback
{
public:
    virtual void run() = 0;
};


// Unsafe means no lazy callback checking to see if the pointer is still active or not
#define LAMBDA_UNSAFE(NAME, FUNCTION_CALL)                          \
class NAME : public CCLambdaCallback                                \
{                                                                   \
public:                                                             \
void run()                                                          \
{                                                                   \
FUNCTION_CALL                                                       \
}                                                                   \
};


#define LAMBDA_1_UNSAFE(NAME, TYPE1, NAME1, FUNCTION_CALL)          \
class NAME : public CCLambdaCallback                                \
{                                                                   \
public:                                                             \
    NAME(TYPE1 *NAME1)                                              \
    {                                                               \
        this->NAME1 = NAME1;                                        \
    }                                                               \
    void run()                                                      \
    {                                                               \
        FUNCTION_CALL                                               \
    }                                                               \
private:                                                            \
    TYPE1 *NAME1;                                                   \
};


#define LAMBDA_1(NAME, TYPE1, NAME1, FUNCTION_CALL)                     \
class NAME : public CCLambdaCallback                                    \
{                                                                       \
public:                                                                 \
    NAME(TYPE1 *NAME1)                                                  \
    {                                                                   \
        this->NAME1 = NAME1;                                            \
        this->lazyPointer = NAME1->lazyPointer;                         \
        this->lazyID = NAME1->lazyID;                                   \
    }                                                                   \
    void run()                                                          \
    {                                                                   \
        if( CCLazyCallback::isCallbackActive( lazyPointer, lazyID ) )   \
        {                                                               \
            FUNCTION_CALL                                               \
        }                                                               \
    }                                                                   \
private:                                                                \
    TYPE1 *NAME1;                                                       \
    void *lazyPointer;                                                  \
    long lazyID;                                                        \
};


#define LAMBDA_2(NAME, TYPE1, NAME1, TYPE2, NAME2, FUNCTION_CALL)       \
class NAME : public CCLambdaCallback                                    \
{                                                                       \
public:                                                                 \
    NAME(TYPE1 *NAME1, TYPE2 *NAME2)                                    \
    {                                                                   \
        this->NAME1 = NAME1;                                            \
        this->lazyPointer = NAME1->lazyPointer;                         \
        this->lazyID = NAME1->lazyID;                                   \
        this->NAME2 = NAME2;                                            \
    }                                                                   \
    void run()                                                          \
    {                                                                   \
        if( CCLazyCallback::isCallbackActive( lazyPointer, lazyID ) )   \
        {                                                               \
            FUNCTION_CALL                                               \
        }                                                               \
    }                                                                   \
private:                                                                \
    TYPE1 *NAME1;                                                       \
    void *lazyPointer;                                                  \
    long lazyID;                                                        \
    TYPE2 *NAME2;                                                       \
};


// Event stiching

// Defines a list of callbacks
#define LAMBDA_SIGNAL CCList<CCLambdaCallback>

// Runs through the list running all the callbacks
#define LAMBDA_EMIT(LIST1)                                          \
if( LIST1.length > 0 )                                              \
{                                                                   \
    for( int i=0; i<LIST1.length; ++i )                             \
    {                                                               \
        LIST1.list[i]->run();                                       \
    }                                                               \
}

// Runs through the list running all the callbacks then clears them
#define LAMBDA_EMIT_ONCE(LIST1)                                     \
if( LIST1.length > 0 )                                              \
{                                                                   \
    for( int i=0; i<LIST1.length; ++i )                             \
    {                                                               \
        LIST1.list[i]->run();                                       \
    }                                                               \
    LIST1.deleteObjectsAndList();                                   \
}

// Connects an event list with a function call
#define LAMBDA_CONNECT(LIST1, TYPE1, POINTER1, FUNCTION_CALL)       \
{                                                                   \
    LAMBDA_1( EventCallback, TYPE1, NAME1,                          \
              NAME1->FUNCTION_CALL;                                 \
            )                                                       \
    LIST1.add( new EventCallback( POINTER1 ) );                     \
}

#define LAMBDA_CONNECT_UNSAFE(LIST1, FUNCTION_CALL)                 \
{                                                                   \
    LAMBDA_UNSAFE( EventCallback,                                   \
              FUNCTION_CALL;                                        \
            )                                                       \
    LIST1.add( new EventCallback() );                               \
}


#endif // __CCCALLBACKS_H__
