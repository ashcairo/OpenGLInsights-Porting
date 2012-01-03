/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCModelText.h
 * Description : Represents a 3d text primitive.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCMODELTEXT_H__
#define __CCMODELTEXT_H__


class CCPrimitiveText : public CCPrimitiveBase
{
public:
    typedef CCPrimitiveBase super;

    CCPrimitiveText(const char *inText, const float inHeight=10.0f);

    // CCBaseType
    virtual void destruct();

    // PrimitiveBase
    virtual void renderVertices(const bool textured);

	void setText(const char *inText);
    
    void setFont(const char *font);
    
    void setHeight(const float inHeight) { height = inHeight; }
    void setCentered(const bool inCentered) { centered = inCentered; }

    const float getWidth();
    const float getHeight() { return height; }
    const CCText& getText() { return text; }
    
	struct CCText text;
    
protected:
    float height;
    bool centered;
    CCTextureFontPage *fontPage;
};

// Here's a model that contains the text primitive to make things easier
class CCModelText : public CCModelBase
{
public:
    typedef CCModelBase super;
    
    CCModelText(CCSceneCollideable *inParent=NULL);
    virtual void destruct();

    void setParent(CCSceneCollideable *inParent);
    
    const CCText& getText() { return primitive->text; }
    const float getWidth() { return primitive->getWidth(); }
    
    void setText(const char *text, const float height=-1.0f, const char *font=NULL);
    void setHeight(const float height);
    void setCentered(const bool centered);
	void setColour(const CCColour &inColour);
    
    void setFont(const char *font);
    
protected:
    CCPrimitiveText *primitive;
    CCSceneCollideable *parent;
    
public:
    CCInterpolatorLinearColour colourInterpolator;
};


#endif // __CCMODELTEXT_H__
