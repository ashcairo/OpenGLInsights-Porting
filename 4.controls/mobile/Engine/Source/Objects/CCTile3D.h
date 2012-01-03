/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTile3D.h
 * Description : Base class for our Tile widgets.
 *
 * Created     : 14/09/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCTILE3D_H__
#define __CCTILE3D_H__


class CCTouchable
{
public:
    CCTouchable()
    {
        touching = false;
        touchReleased = false;
    }
    
    inline const float getTouchingTime() { return touchingTime; }
    
    virtual const uint handleProjectedTouch(const CCCameraProjectionResults &cameraProjectionResults,
                                            const CCSceneCollideable *hitObject, 
                                            const CCVector3 &hitPosition,
                                            const CCScreenTouches &touch, 
                                            const CCTouchAction touchAction) = 0;
    
    // Called when the tile is touched
    virtual void touchActionPressed(const float x, const float y, const CCTouchAction touchAction) = 0;
    
    // Called when the tile is released
    virtual void touchActionRelease(const CCTouchAction touchAction) = 0;
    
protected:
    // Callbacks
    virtual void handleTouchRelease() = 0;
    virtual void onTouchPress()
    {
        LAMBDA_EMIT( onPress );
    }
    
    virtual void onTouchMove()
    {
        LAMBDA_EMIT( onMove );
    }

    virtual void onTouchRelease()
    {
        LAMBDA_EMIT( onRelease );
    }

    virtual void onTouchLoss()
    {
        LAMBDA_EMIT( onLoss );
    }
    
protected:
    bool touching;
    float touchingTime;
    bool touchReleased;
    
public:
    LAMBDA_SIGNAL onPress;
    LAMBDA_SIGNAL onMove;
    LAMBDA_SIGNAL onRelease;
    LAMBDA_SIGNAL onLoss;
};


class CCControllerMovement;

class CCTile3D : public CCSceneCollideable, public CCTouchable, public virtual CCLazyCallback
{
public:
    typedef CCSceneCollideable super;

    CCTile3D();

    // CCRenderable
    virtual void dirtyModelMatrix();
    virtual void setPositionXYZ(const float x, const float y, const float z);
    virtual void translate(const float x, const float y, const float z);
    
    // Positioning Tiles
    void positionTileY(float &y);
    virtual void positionTileBelow(CCTile3D *fromTile);
    void positionTileAbove(CCTile3D *fromTile);
    void positionTileRight(CCTile3D *fromTile);

    // Objects which move along with this tile, but contain handle their own collisions
    CCList<CCSceneObject> attachments;
    CCControllerMovement *movementController;
};


#endif // __CCTILE3D_H__
