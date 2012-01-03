/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCPlatform.mm
 *-----------------------------------------------------------
 */

#import "CCDefines.h"
#import "CCGLView.h"

#import <pthread.h>

void CCEngineThreadLock()
{
//    pthread_t threadID = pthread_self();
//    DEBUGLOG( "Locked thread %i \n", threadID );
    pthread_mutex_lock( &gView->engineThreadMutex );
}


void CCEngineThreadUnlock()
{
//    pthread_t threadID = pthread_self();
//    DEBUGLOG( "Unlocked thread %i \n", threadID );
    pthread_mutex_unlock( &gView->engineThreadMutex );
}