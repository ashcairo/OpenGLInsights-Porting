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
    virtual const bool handleControls(const CCTime &gameTime);
    
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

public:
    void addTile(CCTile3D *inTile)
    {
        tiles.add( inTile );
    }
    
    CCTile3D* getTile(const int i)
    {
        return tiles.list[i];
    }
    
protected:
    virtual void cameraUpdated() {}

    virtual const bool touchAllowed(const CCScreenTouches &touch);
    virtual const bool handleTwoTouches(const CCScreenTouches &touch1, const CCScreenTouches &touch2);
    virtual const bool handleThreeTouches(const CCScreenTouches &touch1, 
                                          const CCScreenTouches &touch2, 
                                          const CCScreenTouches &touch3);
    
    virtual void touchPressed(const CCScreenTouches &touch);
    virtual const bool touchMovementAllowed(const CCScreenTouches &touch, CCPoint &touchDelta);
    virtual const bool touchMoving(const CCScreenTouches &touch, const CCPoint &touchDelta);
    virtual const bool touchReleased(const CCScreenTouches &touch);

    virtual const bool handleTilesTouch(const CCScreenTouches &touch, const CCTouchAction touchAction);

    virtual const bool touchCameraMoving(const CCScreenTouches &touch, const float x, const float y);
    virtual const bool touchCameraZooming(const float amount);
    virtual const bool touchCameraRotating(const float x, const float y);
    virtual const bool touchReleaseSwipe(const CCScreenTouches &touch);
    
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
    
	void *controlsUITouch;
    bool controlsMoving;
    bool controlsMovingVertical;

    CCTile3DScrollBar *scrollBar;
    
    // Used in the handleTilesTouch function to detect touches
    CCList<CCTile3D> tiles;
};
