/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceControls.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCDeviceControls.h"

#include <jni.h>


// Java constants pulled from touch event at Java end
enum ActionTypes
{
	ACTION_DOWN  = 0x0,
	ACTION_UP    = 0x1,
	ACTION_MOVE  = 0x2,
	POINTER_DOWN = 0x5,
	POINTER_UP   = 0x6
};


extern "C"
{
JNIEXPORT void JNICALL Java_com_android2c_CCJNI_controlsHandleTouch(JNIEnv *jEnv, jobject jObj, jfloat jX, jfloat jY, jint jActionType, jint jFinger);
JNIEXPORT bool JNICALL Java_com_android2c_CCJNI_controlsHandleBackButton(JNIEnv *jEnv, jobject jObj);
};

JNIEXPORT void JNICALL Java_com_android2c_CCJNI_controlsHandleTouch(JNIEnv *jEnv, jobject jObj, jfloat jX, jfloat jY, jint jActionType, jint jFinger)
{
	CCPoint position( jX, jY );

	// Force only two fingers
	if( gEngine && ( jFinger == 0 || jFinger == 1 ) )
	{
		CCDeviceControls *controls = (CCDeviceControls*)gEngine->controls;
		controls->touch( position, jActionType, jFinger );
	}
}


JNIEXPORT bool JNICALL Java_com_android2c_CCJNI_controlsHandleBackButton(JNIEnv *jEnv, jobject jObj)
{
	if( gEngine != NULL )
	{
		for( int i=0; i<gEngine->scenes.length; ++i )
		{
			CCSceneBase *scene = gEngine->scenes.list[i];
			if( scene->handleBackButton() )
			{
				return true;
			}
		}
	}
	return false;
}


CCDeviceControls::CCDeviceControls()
{
}

void CCDeviceControls::touch(CCPoint position, int action, int finger)
{
	UITouch *touch;
	if( action == ACTION_DOWN || action == POINTER_DOWN )
	{
		touch = &deviceTouches[finger];
		touch->pos = position;
		touchBegin( touch );
	}
	else if( action == ACTION_MOVE )
	{
		touch = (UITouch*)screenTouches[finger].usingTouch;
		if( touch != NULL )
		{
			touch->pos = position;
			touchMove( touch );
		}
	}
	else if( action == ACTION_UP || action == POINTER_UP )
	{
		touch = (UITouch*)screenTouches[finger].usingTouch;
		touchEnd( touch );

	}
}

void CCDeviceControls::touchBegin(UITouch *touch)
{
    touchHandle( touch );

    inUse = false;
    for( uint touchIndex=0; touchIndex<numberOfTouches; ++touchIndex )
    {
        inUse |= screenTouches[touchIndex].usingTouch != NULL;
    }
}


void CCDeviceControls::touchMove(UITouch *touch)
{
    if( inUse )
    {
        touchHandle( touch );
    }
}


void CCDeviceControls::touchEnd(UITouch *touch)
{
    unTouch( touch );
    bool touchesFinished = true;

    for( uint i = 0; i<numberOfTouches; ++i )
    {
    	if( screenTouches[i].usingTouch != NULL )
    	{
    		touchesFinished = false;
    	}
    }
    
    if( touchesFinished )
    {
        inUse = false;
    }
}


void CCDeviceControls::touchHandle(UITouch *touch)
{
    const CCSize &screenSize = gEngine->renderer->screenSizeMultiple;

    CCPoint position = touch->pos;
    position.x *= screenSize.width;
    position.y *= screenSize.height;

    for( uint touchIndex = 0; touchIndex<numberOfTouches; ++touchIndex )
    {
		CCScreenTouches& screenTouch = screenTouches[touchIndex];

		if( screenTouch.usingTouch == NULL || screenTouch.usingTouch == touch )
		{
			CCPoint screenPosition = position;
			CCEngineThreadLock();
			if( screenTouch.usingTouch != NULL )
			{
				screenTouch.delta.x += screenPosition.x - screenTouch.position.x;
				screenTouch.delta.y += screenPosition.y - screenTouch.position.y;
				screenTouch.totalDelta.x += screenTouch.delta.x;
				screenTouch.totalDelta.y += screenTouch.delta.y;
			}
			else
			{
				// Restart our counters
				screenTouch.totalDelta.x = 0.0f;
				screenTouch.totalDelta.y = 0.0f;
				screenTouch.timeHeld = 0.0f;
				screenTouch.startPosition = screenPosition;
		
				for( uint i=0; i<CCScreenTouches::max_last_deltas; ++i )
				{
					screenTouch.lastDeltas[i].clear();
				}
			}
			screenTouch.position = screenPosition;
			screenTouch.usingTouch = touch;
			CCEngineThreadUnlock();
			break;
		}
    }
}
