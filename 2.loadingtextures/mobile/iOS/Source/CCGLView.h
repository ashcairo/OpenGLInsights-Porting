/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCGLView.h
 * Description : OpenGL view.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCGLVIEW_H__
#define __CCGLVIEW_H__


#import "CCDefines.h"

@class CCGLView;
extern CCGLView *gView;

@interface CCGLView : UIView 
{	
@public
	bool runningGame, paused, engineThreadRunning;
	pthread_mutex_t engineThreadMutex;
    pthread_mutexattr_t engineThreadMutexType;
	
	NSTimer *updateTimer;
}

-(void)shutdown;

-(void)setup;

-(void)updateNativeThread;

@end


#endif // __CCGLVIEW_H__
