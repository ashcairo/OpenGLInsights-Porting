/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTile3DButton.h
 * Description : A dynamic button widget.
 *
 * Created     : 14/09/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCTILE3DBUTTON_H__
#define __CCTILE3DBUTTON_H__


class CCModelText;

class CCTile3DButton : public CCTile3D
{
public:
    typedef CCTile3D super;
    
    CCTile3DButton(CCSceneBase *scene);
    CCTile3DButton(CCSceneBase *scene, const float width, const float height, const char *text=NULL);
    
    // Create tile with width and textures aspect ratio
    CCTile3DButton(CCSceneBase *scene, const float width, const char *textureName);
    
    // Create tile with the text height
    CCTile3DButton(CCSceneBase *scene, const char *text, const float height, const bool centered);
    
    virtual void construct(CCSceneBase *scene);
    virtual void destruct();
    
	// CCRenderable
	virtual void refreshModelMatrix();

	// CCSceneObject
	virtual void update(const CCTime &gameTime);
    virtual void renderModels(const bool alpha);

    virtual void setupBase(const float width, const float height);
    void setBaseTexture(const char *name, const CCResourceType resourceType);
    void setBaseColour(const CCColour &inColour);
    void setBaseColourAlpha(const float inAlpha);
    const CCColour& getBaseColour() { return *baseModel->getColour(); }
    inline CCPrimitiveSquare* getBaseSquare() { return baseSquare; }
    
    void setRenderDepth(const bool toggle) { renderDepth = toggle; }
    
    // Touchable
    virtual const uint handleProjectedTouch(const CCCameraProjectionResults &cameraProjectionResults,
                                            const CCSceneCollideable *hitObject, 
                                            const CCVector3 &hitPosition,
                                            const CCScreenTouches &touch, 
                                            const CCTouchAction touchAction);

    // Called when the tile is touched
    virtual void touchActionPressed(const float x, const float y, const CCTouchAction touchAction);

    // Called when the tile is released
    virtual void touchActionRelease(const CCTouchAction touchAction);

protected:
    // Callbacks
    void handleTouchRelease();

public:
    inline void allowTouchRotation(const bool allow) { touchRotationAllowed = allow; }
    const CCVector3& getRotationTarget() 
    {
        if( touchRotationInterpolator.interpolators.length > 0 )
        {
            return touchRotationInterpolator.interpolators.list[touchRotationInterpolator.interpolators.length-1]->getTarget();
        }
        return rotation;
    }

    void allowTouchMovement(const bool allow=true)
    {
        touchMovementAllowed = allow;
    }
    
    void setTouchRotationSpeed(const float speed)
    {
        touchRotationSpeed = speed;
    }
    
    void setTouchDepressRange(const float range)
    {
        touchDepressRange = range;
    }

public:
    CCModelText *textModel;

protected:
    bool renderDepth;
    
    CCModelBase *baseModel;
    CCPrimitiveSquare *baseSquare;
    
    CCInterpolatorCurveV3<CCInterpolatorCurve> scaleInterpolator;

    bool touchMovementAllowed;
    
    bool touchRotationAllowed;
    CCInterpolatorLinearsV3 touchRotationInterpolator;
    float touchRotationMagnitude;
    float touchRotationSpeed;
    
    float touchDepressRange;
    float touchDepressDepth;

    // Touch Position interpolation
    CCVector3 touchDepressPosition;
    CCInterpolatorCurvesV3 touchDepressInterpolator;
    
public:
    CCInterpolatorLinearColour colourInterpolator;
};


#endif // __CCTILE3DBUTTON_H__
