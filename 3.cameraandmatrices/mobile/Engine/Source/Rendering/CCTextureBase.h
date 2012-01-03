/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTextureBase.h
 * Description : Represents a texture.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCTEXTUREBASE_H__
#define __CCTEXTUREBASE_H__


#include "CCDefines.h"

class CCSceneBase;

class CCTextureBase
{
public:
    CCTextureBase();
    virtual ~CCTextureBase();

    const bool loadAndCreate(const char *path, const CCResourceType resourceType, const bool generateMipMap);
    virtual const bool load(const char *path, const CCResourceType resourceType, const bool generateMipMap) = 0;

    virtual const bool createGLTexture();
    
    const float getImageWidth() const { return (float)imageWidth; }
    const float getImageHeight() const { return (float)imageHeight; }
    
    const float getAllocatedWidth() const { return (float)allocatedWidth; }
    const float getAllocatedHeight() const { return (float)allocatedHeight; }
    const int getBytes() { return allocatedBytes; }

protected:
    static const bool extensionSupported(const char* extension);

public:
    GLuint glName;

protected:
    uint32_t imageWidth, imageHeight;
    uint32_t allocatedWidth, allocatedHeight;
    uint32_t allocatedBytes;
};


#endif // __CCTEXTUREBASE_H__