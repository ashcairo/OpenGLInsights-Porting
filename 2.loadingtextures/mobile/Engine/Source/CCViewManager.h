/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCViewManager.h
 * Description : Controls the different app views.
 *
 * Created     : 30/08/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCVIEWMANAGER_H__
#define __CCVIEWMANAGER_H__


#include "CCGLView.h"

#ifdef IOS
#import "CCViewController.h"
#import "CCARView.h"
#endif

#ifndef ANDROID
#include "CCVideoView.h"
#endif

class CCViewManager
{
public:
	CCViewManager();
    ~CCViewManager();
    
    void startup();
    void shutdown();
    void pause();
    void resume();
    
    void toggleAdverts(const bool toggle);
    const float getAdvertHeight();

    void startVideoView(const char *file);
    void toggleVideoView(const bool toggle);
    void stopVideoView();
    
    void startARView();
    void stopARView();

protected:
    void toggleAdvertsNativeThread(const bool toggle);

    void startVideoViewNativeThread(const char *file);
    void toggleVideoViewNativeThread(const bool toggle);
    void stopVideoViewNativeThread();
    
    void startARViewNativeThread();
    void stopARViewNativeThread();
    
    void toggleBackgroundRender(const bool toggle);
    
protected:
    CCGLView *glView;

#ifndef ANDROID
    CCVideoView *videoView;
#endif

#ifdef IOS
    CCARView *arView;
	CCViewController *viewController;
    UIWindow *window;
#endif
    
    bool opaqueOpenGLRendering;
    
public:
    static CCViewManager *instance;
};


#endif // __CCVIEWMANAGER_H__

