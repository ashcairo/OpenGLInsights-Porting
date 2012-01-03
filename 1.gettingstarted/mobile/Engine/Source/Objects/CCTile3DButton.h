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


class CCTile3DButton : public CCTile3D
{
public:
    typedef CCTile3D super;
    
    CCTile3DButton(CCSceneBase *scene);
    CCTile3DButton(CCSceneBase *scene, const float width, const float height);
    
    virtual void construct(CCSceneBase *scene);
    virtual void destruct();

	// CCSceneObject
	virtual void update(const CCTime &gameTime);
    virtual void renderModels(const bool alpha);

    virtual void setupBase(const float width, const float height);
    void setBaseColour(const CCColour &inColour);
    void setBaseColourAlpha(const float inAlpha);
    const CCColour& getBaseColour() { return *baseModel->getColour(); }
    inline CCPrimitiveSquare* getBaseSquare() { return baseSquare; }
    
    void setRenderDepth(const bool toggle) { renderDepth = toggle; }
    
protected:
    bool renderDepth;
    
    CCModelBase *baseModel;
    CCPrimitiveSquare *baseSquare;

public:
    CCInterpolatorLinearColour colourInterpolator;
};


#endif // __CCTILE3DBUTTON_H__
