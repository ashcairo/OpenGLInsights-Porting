/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCSceneAppUI.h
 * Description : AppUI scene template.
 *
 * Created     : 06/06/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#include "CCCameraAppUI.h"

class CCTile3D;
class CCTile3DScrollBar;

class CCSceneAppUI : public CCSceneBase, public virtual CCLazyCallback
{
public:
    typedef CCSceneBase super;

	CCSceneAppUI();
	virtual void destruct();
    
    // CCSceneBase
	virtual void setup();
    
protected:
	virtual void updateScene(const CCTime &gameTime);
    virtual void updateCamera(const CCTime &gameTime);
    
public:
    virtual const bool render(const CCCameraBase *inCamera, const int pass, const bool alpha);
	virtual void renderOctreeObject(CCSceneObject *object, const CCCameraBase *inCamera, const int pass, const bool alpha);

    virtual void render2DBackground(const uint inCameraIndex);
    virtual void render2DForeground(const uint inCameraIndex);
    
    virtual void beginOrientationUpdate();
    virtual void finishOrientationUpdate();
    
protected:
    virtual void cameraUpdated() {}
    
public:
    CCCameraBase* getCamera() { return camera; }
    virtual void scrollCameraToTop();
    virtual void refreshCameraView();
    virtual void lockCameraView();
    
protected:
    CCCameraAppUI *camera;
    bool cameraScrolling;
    float sceneLeft, sceneRight;
    float sceneTop, sceneBottom;    // The top and bottom in relation to the center of the camera

    LAMBDA_SIGNAL orientationCallback;
    bool updatingOrientation;

    CCTile3DScrollBar *scrollBar;
};
