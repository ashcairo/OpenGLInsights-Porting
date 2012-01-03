/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCGLView.h
 * Description : OpenGL view handler.
 *
 * Created     : 09/06/11
 * Author(s)   : Ashraf Samy Hegab, Chris Bowers
 *-----------------------------------------------------------
 */

#ifndef __CCGLVIEW_H__
#define __CCGLVIEW_H__


#include <jni.h>

extern class CCGLView *gView;

class CCGLView
{
public:
    CCGLView()
    {
        gView = this;

    	jniEnv = NULL;
    	jniObj = NULL;

    	pthread_mutexattr_init( &engineThreadMutexType );
		pthread_mutexattr_settype( &engineThreadMutexType, PTHREAD_MUTEX_RECURSIVE );
		pthread_mutex_init( &engineThreadMutex, &engineThreadMutexType );

    	width = height = 0.0f;
    	runningGame = paused = engineThreadRunning = false;
    }

    ~CCGLView()
    {
        ASSERT( runningGame == false );
    	pthread_mutex_destroy( &engineThreadMutex );
        pthread_mutexattr_destroy( &engineThreadMutexType );
    }

    void resizeView(const float inWidth, const float inHeight)
	{
    	width = inWidth;
    	height = inHeight;
	}

    const float getWidth() { return width; }
    const float getHeight() { return height; }

public:
    bool runningGame, paused, engineThreadRunning;
    JNIEnv *jniEnv;
    jobject jniObj;

    pthread_mutex_t engineThreadMutex;
	pthread_mutexattr_t engineThreadMutexType;

protected:
    float width, height;
};


#endif // __CCGLVIEW_H__
