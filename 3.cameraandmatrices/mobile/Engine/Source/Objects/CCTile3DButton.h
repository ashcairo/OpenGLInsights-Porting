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

public:
    CCModelText *textModel;

protected:
    bool renderDepth;
    
    CCModelBase *baseModel;
    CCPrimitiveSquare *baseSquare;
    
    CCInterpolatorCurveV3<CCInterpolatorCurve> scaleInterpolator;

public:
    CCInterpolatorLinearColour colourInterpolator;
};


#endif // __CCTILE3DBUTTON_H__
