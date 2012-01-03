/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCControls.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCControls.h"
#include "CCDeviceControls.h"


const CCPoint CCScreenTouches::averageLastDeltas() const
{
    CCPoint averageDeltas;
    int numberOfDeltas = 0;
    const float lifetime = gEngine->gameTime.lifetime;
    for( int i=0; i<max_last_deltas; ++i )
    {
        const TimedDelta &lastDelta = lastDeltas[i];
        const float difference = lifetime - lastDelta.time;
        if( difference < 0.25f )
        {
            averageDeltas.x += lastDelta.delta.x;
            averageDeltas.y += lastDelta.delta.y;
            numberOfDeltas++;
        }
        else
        {
            break;
        }
    }
    if( numberOfDeltas > 1 )
    {
        averageDeltas.x /= numberOfDeltas;
        averageDeltas.y /= numberOfDeltas;
    }
    return averageDeltas;
}


CCControls::CCControls()
{
    inUse = false;
}


void CCControls::render()
{
}


void CCControls::update(const CCTime &gameTime)
{
	for( uint i=0; i<numberOfTouches; ++i )
	{
        CCScreenTouches &screenTouch = screenTouches[i];
        if( screenTouch.usingTouch != NULL )
		{
            screenTouch.timeHeld += gameTime.real;
		}
		
        if( screenTouch.lastTouch != NULL )
		{
            if( screenTouch.usingTouch != NULL )
            {
                screenTouch.usingTouch = NULL;
            }
            else
            {
                screenTouch.lastTouch = NULL;
            }
		}
		else
		{
            screenTouch.lastTimeReleased += gameTime.real;
		}

		for( uint j=1; j<CCScreenTouches::max_last_deltas; ++j )
		{
            screenTouch.lastDeltas[j] = screenTouch.lastDeltas[j-1];
		}
        screenTouch.lastDeltas[0].time = gameTime.lifetime;
        screenTouch.lastDeltas[0].delta = screenTouch.delta;
		
        screenTouch.delta.x = 0.0f;
        screenTouch.delta.y = 0.0f;
	}
}


void CCControls::unTouch(void *touch)
{
    UITouch *uiTouch = (UITouch*)touch;
	for( uint i=0; i<numberOfTouches; ++i )
	{
        CCScreenTouches &screenTouch = screenTouches[i];
		if( screenTouch.usingTouch == uiTouch )
        {
            CCEngineThreadLock();
            screenTouch.lastTouch = uiTouch;
			
			screenTouch.lastTimeReleased = 0.0f;
			screenTouch.lastTotalDelta = screenTouch.totalDelta;
            CCEngineThreadUnlock();
		}
	}
}


const bool CCControls::detectZoomTouch()
{
	if( screenTouches[0].delta.x != 0.0f || 
	    screenTouches[0].delta.y != 0.0f ||	
	    screenTouches[1].delta.x != 0.0f ||
	    screenTouches[1].delta.y != 0.0f )
	{	
		// If our x's are opposite
		if( screenTouches[0].delta.x > 0.0f )
		{
			if( screenTouches[1].delta.x < 0.0f )
			{
                return true;
			}
		}
		else if( screenTouches[0].delta.x < 0.0f )
		{
			if( screenTouches[1].delta.x > 0.0f )
			{
                return true;
			}
		}
		
		// Find out the position of our touches
		CCScreenTouches *topTouch = &screenTouches[0],
                     *bottomTouch = &screenTouches[1];
		if( screenTouches[0].position.y < screenTouches[1].position.y )
		{
			topTouch = &screenTouches[1];
			bottomTouch = &screenTouches[0];
		}
		
		if( topTouch->delta.y > 0.0f )
		{
			if( bottomTouch->delta.y < 0.0f )
			{
				if( topTouch->delta.x > 0.0f )
				{
					if( bottomTouch->delta.x < 0.0f )
					{
                        return true;
					}
				}
				else if( topTouch->delta.x < 0.0f )
				{
					if( bottomTouch->delta.x > 0.0f )
					{
                        return true;
					}
				}
			}
		}
		else if( topTouch->delta.y < 0.0f )
		{
			if( bottomTouch->delta.y > 0.0f )
			{
				if( topTouch->delta.x > 0.0f )
				{
					if( bottomTouch->delta.x < 0.0f )
					{
                        return true;
					}
				}
				else if( topTouch->delta.x < 0.0f )
				{
					if( bottomTouch->delta.x > 0.0f )
					{
                        return true;
					}
				}
			}
		}
		
		float combinedHorizontalDelta = 0.0f;
		if( topTouch->delta.x > 0.0f || bottomTouch->delta.x > 0.0f )
		{
			combinedHorizontalDelta += topTouch->delta.x;
			combinedHorizontalDelta += bottomTouch->delta.x;
		}
		else
		{
			combinedHorizontalDelta += -topTouch->delta.x;
			combinedHorizontalDelta += -bottomTouch->delta.x;
		}

		float combinedVerticalDelta = 0.0f;
		if( screenTouches[0].delta.y > 0.0f )
		{
			combinedVerticalDelta += screenTouches[0].delta.y;
			combinedVerticalDelta += -screenTouches[1].delta.y;
		}
		else if( screenTouches[0].delta.y < 0.0f )
		{
			combinedVerticalDelta += -screenTouches[0].delta.y;
			combinedVerticalDelta += screenTouches[1].delta.y;
		}
		else if( screenTouches[1].delta.y > 0.0f )
		{
			combinedVerticalDelta += screenTouches[1].delta.y;
			combinedVerticalDelta += -screenTouches[0].delta.y;
		}
		else if( screenTouches[1].delta.y < 0.0f )
		{
			combinedVerticalDelta += -screenTouches[1].delta.y;
			combinedVerticalDelta += screenTouches[0].delta.y;
		}
		
		if( combinedVerticalDelta > combinedHorizontalDelta )
		{
            return true;
		}
	}
	
    return false;
}


const bool CCControls::touchActionMoving(const CCTouchAction touchAction)
{
    if( touchAction >= touch_movingHorizontal && touchAction <= touch_moving )
    {
        return true;
    }
    return false;
}
