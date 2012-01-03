/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTexturePNG.h
 * Description : Android specific texture loader for JPG/GIF/PNG.
 *
 * Created     : 15/05/11
 * Author(s)   : Chris Bowers, Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCTEXTUREPNG_H__
#define __CCTEXTUREPNG_H__

#include "CCTextureBase.h"

class CCTexturePNG : public CCTextureBase
{
public:
	typedef CCTextureBase super;

	CCTexturePNG();
    virtual ~CCTexturePNG();

    // TextureBase
    virtual const bool load(const char *name, const CCResourceType resourceType, const bool generateMipMap);
    virtual const bool createGLTexture();
    
private:
    
};

#endif // __TEXTUREPNG_H__

