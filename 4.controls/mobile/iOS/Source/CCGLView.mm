/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCGLView.mm
 *-----------------------------------------------------------
 */

#import "CCDefines.h"
#import "CCDeviceControls.h"

#import <QuartzCore/QuartzCore.h>
#import <pthread.h>


@implementation CCGLView

// You must implement this method
+(Class)layerClass 
{
    return [CAEAGLLayer class];
}


-(id)initWithFrame:(CGRect)frame
{	
	self = [super initWithFrame:frame];
    if( self != NULL )
	{
		gView = self;
		
		self.userInteractionEnabled = false;
		self.hidden = false;
		
		runningGame = paused = false;
        
        pthread_mutexattr_init( &engineThreadMutexType );
        pthread_mutexattr_settype( &engineThreadMutexType, PTHREAD_MUTEX_RECURSIVE );
		pthread_mutex_init( &engineThreadMutex, &engineThreadMutexType );
		
		// Get the layer
		CAEAGLLayer *eaglLayer = (CAEAGLLayer*)self.layer;
        
        // Set contentScale Factor to 2, to use high resolution
        if( false && [[UIScreen mainScreen] respondsToSelector:@selector( scale )] && [[UIScreen mainScreen] scale] == 2.0 ) 
        {
            self.contentScaleFactor = 2.0f;
            eaglLayer.contentsScale = 2.0f;
        }
        
		eaglLayer.opaque = true;
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:false], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, NULL];
        
        gEngine = new CCAppEngine();
		updateTimer = [NSTimer scheduledTimerWithTimeInterval:0.0f target:self selector:@selector( setup ) userInfo:NULL repeats:false];
	}
	
    return self;
}


-(void)dealloc 
{	
    ASSERT( runningGame == false );
	pthread_mutex_destroy( &engineThreadMutex );
    pthread_mutexattr_destroy( &engineThreadMutexType );
	
    [super dealloc];
}


-(void)shutdown
{	
	self.userInteractionEnabled = false;
	delete gEngine;
	gEngine = NULL;
    gView = NULL;
    
	[self release];
}


static inline void refreshReleasePool(NSAutoreleasePool **pool, uint *count, const uint target)
{
	if( (*count)++ > target )
	{
		[*pool release];
		*pool = [[NSAutoreleasePool alloc] init];
		*count = 0;
	}
}


void* PosixGameThread(void* data)
{	
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	gEngine->setupEngineThread();
	[pool release];
	pool = [[NSAutoreleasePool alloc] init];
	
	CCGLView *view = (CCGLView*)data;
	view->engineThreadRunning = true;
	view->runningGame = true;
	usleep( 0 );

	uint poolRefreshCounter = 0;
	do
	{
		if( view->paused )
		{
			// Sleep at 20 fps
			usleep( 50000 );
		}
		else
		{
			gEngine->updateEngineThread();
		}
		
		refreshReleasePool( &pool, &poolRefreshCounter, 1000 );
	} while( view->runningGame );
	[pool release];
	
	view->engineThreadRunning = false;
	return NULL;
}


-(void)emptyThread
{
}


void createThread(void *(*start_routine)(void*), void *__restrict arg, int prioritySet, int priorityAdj)
{
	// Create the game thread using POSIX routines.
	pthread_attr_t attr;
	pthread_t posixThreadID;
	int returnVal;
	
	returnVal = pthread_attr_init( &attr );
	ASSERT( !returnVal );
	
	returnVal = pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
	ASSERT( !returnVal );
	
	returnVal = pthread_create( &posixThreadID, &attr, start_routine, arg );
	ASSERT( !returnVal );
	
	struct sched_param param;
	int policy;
	pthread_getschedparam( posixThreadID, &policy, &param );
	ASSERT( !returnVal );
	
	if( prioritySet != 0 )
	{
		param.sched_priority = prioritySet;
	}
	else if( priorityAdj != 0 )
	{
		param.sched_priority += priorityAdj;
	}
	ASSERT( param.sched_priority > 0 && param.sched_priority < 100 );
	
	returnVal = pthread_setschedparam( posixThreadID, policy, &param );
	ASSERT( !returnVal );
	
	returnVal = pthread_attr_destroy( &attr );
	ASSERT( !returnVal );
}


-(void)setup
{
	gEngine->setupNativeThread();
	
	// iPhone SDK recommends launching an empty NSThread when using POSIX threads with Cocoa applications
	[NSThread detachNewThreadSelector:@selector( emptyThread ) toTarget:self withObject:NULL]; 
	
	// Create the game thread using POSIX routines.
	createThread( &PosixGameThread, self, 0, +4 );
	
	// Start the updating of native OS thread
	[updateTimer invalidate];
	updateTimer = [NSTimer scheduledTimerWithTimeInterval:0.25f target:self selector:@selector( updateNativeThread ) userInfo:NULL repeats:true];
}


-(void)updateNativeThread
{
	if( runningGame )
	{	
        static bool firstRun = true;
        if( firstRun )
        {
            firstRun = false;
            self.alpha = 0.0f;
            self.alpha = 1.0f;
            
            // Enable Multi Touch of the view
            self.multipleTouchEnabled = true;
            self.userInteractionEnabled = true;
        }
        
        if( gEngine->updateNativeThread() == false )
        {
        }
	}
}


// These are four methods touchesBegan, touchesMoved, touchesEnded, touchesCancelled and use to notify about touches and gestures
-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event 
{	
    CCDeviceControls *controls = (CCDeviceControls*)gEngine->controls;
	controls->touchBegin( touches, self );
}


-(void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event 
{
    CCDeviceControls *controls = (CCDeviceControls*)gEngine->controls;
	controls->touchMove( touches, self);
}


-(void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event 
{
    CCDeviceControls *controls = (CCDeviceControls*)gEngine->controls;
	controls->touchEnd( touches, event, self );
}


-(void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event 
{
    CCDeviceControls *controls = (CCDeviceControls*)gEngine->controls;
	controls->touchEnd( touches, event, self );
}

@end
