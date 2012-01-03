/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTile3DScrollBar.h
 * Description : Scroll bar implementation.
 *
 * Created     : 25/06/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCTILE3DSCROLLBAR_H__
#define __CCTILE3DSCROLLBAR_H__


class CCTile3DScrollBar : public CCTile3DButton
{
public:
    typedef CCTile3DButton super;

    CCTile3DScrollBar(CCSceneBase *scene);
    virtual void destruct();

    // CCSceneObject
    void update(const CCTime &gameTime);

    // Tile3D
    virtual const uint handleProjectedTouch(const CCCameraProjectionResults &cameraProjectionResults,
                                            const CCSceneCollideable *hitObject, 
                                            const CCVector3 &hitPosition,
                                            const CCScreenTouches &touch, 
                                            const CCTouchAction touchAction);

    void resize(const float cameraY, const float cameraWidth, const float cameraHeight, 
                const float sceneTopY, const float sceneBottomY);
    void reposition(const float cameraY, const float cameraWidth, const float cameraHeight);

protected:
    void moved();
    
public:
    void unHighlight();

protected:
    float topY;
    float bottomY;
};


#endif // __CCTILE3DSCROLLBAR_H__