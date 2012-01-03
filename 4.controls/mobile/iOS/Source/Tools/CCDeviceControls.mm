/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceControls.mm
 *-----------------------------------------------------------
 */

#import "CCDefines.h"
#import "CCDeviceControls.h"


CCDeviceControls::CCDeviceControls()
{
    //motionManager = [[CMMotionManager alloc] init];
    //[motionManager startGyroUpdates];
    //[motionManager startAccelerometerUpdates];
}


CCDeviceControls::~CCDeviceControls()
{
    //[motionManager release];
}


void CCDeviceControls::touchBegin(NSSet *touches, CCGLView *view)
{	
	touchHandle( touches, view );

	inUse = false;
	for( uint touchIndex=0; touchIndex<numberOfTouches; ++touchIndex )
	{
		inUse |= screenTouches[touchIndex].usingTouch != NULL;
	}
}


void CCDeviceControls::touchMove(NSSet *touches, CCGLView *view)
{	
	if( inUse )
	{
		touchHandle( touches, view);
	}
}


void CCDeviceControls::touchEnd(NSSet *touches, UIEvent *event, CCGLView *view)
{	
    // For all the touches that have been released, call unTouch on them
	NSArray *touchesArray = [touches allObjects];
	for( uint i=0; i<[touchesArray count]; ++i )
	{
		UITouch *touch = [touchesArray objectAtIndex:i];
		unTouch( touch );
	}
	
    // Now compare the number of touches  being released with the touches present on the screen
    // If they're different, something weird has occured, hence touchesCancelled
	NSArray *touchesForViewArray = [[event touchesForView:view] allObjects];
    if( [touchesArray count] == [touchesForViewArray count] )
	{
		inUse = false;
		//DEBUGLOG( @"touchesEnded, all fingers up" );
    }
	else
	{
		DEBUGLOG( "touchesCancelled" );
	}
}



void CCDeviceControls::touchHandle(NSSet* touches, CCGLView *view)
{
	const CCSize &screenSizeMultiple = gEngine->renderer->screenSizeMultiple;
	
 	NSArray *touchesArray = [touches allObjects];
	for( uint i=0; i<[touchesArray count]; ++i )
	{
		UITouch *touch = [touchesArray objectAtIndex:i];
		
		CGPoint cgPosition = [touch locationInView:view];
		CCPoint position( cgPosition.x, cgPosition.y );
		position.x *= screenSizeMultiple.width;
		position.y *= screenSizeMultiple.height;
		
		// Fill in our screen touches
		for( uint touchIndex=0; touchIndex<numberOfTouches; ++touchIndex )
		{
			CCScreenTouches &screenTouch = screenTouches[touchIndex];
			if( screenTouch.usingTouch == NULL || screenTouch.usingTouch == touch )
			{	
				CCPoint screenPosition = position;
				if( gEngine->renderer->orientation.target == 270.0f )
				{
					CCSwapFloat( screenPosition.x, screenPosition.y );
					screenPosition.y = 1.0f - screenPosition.y;
				}
				else if( gEngine->renderer->orientation.target == 90.0f )
				{
					CCSwapFloat( screenPosition.x, screenPosition.y );
					screenPosition.x = 1.0f - screenPosition.x;
				}
				else if( gEngine->renderer->orientation.target == 180.0f )
				{
					screenPosition.x = 1.0f - screenPosition.x;
					screenPosition.y = 1.0f - screenPosition.y;
				}
                
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
}
