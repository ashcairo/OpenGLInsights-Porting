/*----------------------------------------------------------
* Copyright © 2011 - 2011 France Telecom
* This software is distributed under the Apache 2.0 license,
* see the "license.txt" file for more details.
*-----------------------------------------------------------
* File Name   : main.cpp
* Description : JNI entry and interface point.
*
* Created     : 15/05/11
* Author(s)   : Ashraf Samy Hegab, Chris Bowers
*-----------------------------------------------------------
*/

#include "CCDefines.h"
#include "CCAppEngine.h"
#include "CCGLView.h"
#include "CCDeviceControls.h"
#include "CCDeviceURLManager.h"

#include "CCViewManager.h"
static CCViewManager *viewManager = NULL;

#include <jni.h>


static void printGLString(const char *name, GLenum s)
{
    const char *v = (const char *)glGetString( s );
    DEBUGLOG( "GL %s = %s\n", name, v );
}


extern "C"
{
    JNIEXPORT void JNICALL Java_com_android2c_CCJNI_onSurfaceChanged(JNIEnv *jEnv, jobject jObj,  jint jWidth, jint jHeight);
    JNIEXPORT void JNICALL Java_com_android2c_CCJNI_onDrawFrame(JNIEnv *jEnv, jobject jObj);
};

JNIEXPORT void JNICALL Java_com_android2c_CCJNI_onSurfaceChanged(JNIEnv *jEnv, jobject jObj, jint jWidth, jint jHeight)
{	
#ifdef DEBUGON
	// Now we can do what we intended
	printGLString( "Version", GL_VERSION );
	printGLString( "Vendor", GL_VENDOR );
	printGLString( "Renderer", GL_RENDERER );
	printGLString( "Extensions", GL_EXTENSIONS );
#endif

	if( viewManager != NULL )
	{
		viewManager->shutdown();
		delete viewManager;
	}
	viewManager = new CCViewManager();

	// Get the window controller to create the glView
	viewManager->startup();

	// Then feed it in it's appropriate settings here
	gView->jniEnv = jEnv;
	gView->jniObj = jObj;
	gView->resizeView( jWidth, jHeight );

	if( gEngine != NULL )
	{
		delete gEngine;
	}

	gEngine = new CCAppEngine();
	gEngine->setupNativeThread();
	gEngine->setupEngineThread();

	gView->runningGame = true;
	gView->engineThreadRunning = true;

#if defined PROFILEON
    CCProfiler::open();
#endif
}


JNIEXPORT void JNICALL Java_com_android2c_CCJNI_onDrawFrame(JNIEnv *jEnv, jobject jObj)
{
	if( gView->runningGame )
	{
		gView->jniEnv = jEnv;
		gView->jniObj = jObj;

		gEngine->updateNativeThread();
		gEngine->updateEngineThread();
	}

#if defined PROFILEON
    CCProfiler::save();
#endif
}
