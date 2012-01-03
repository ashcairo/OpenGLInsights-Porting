/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTexturePNG.h
 * Description : Qt specific texture loader for PNG/JPG.
 *
 * Created     : 27/04/10
 * Author(s)   : Jerome Portier, Ashraf Samy Hegab
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

    // CCTextureBase
    virtual const bool load(const char *name, const CCResourceType resourceType, const bool generateMipMap);
    virtual const bool createGLTexture();
};

#endif // __CCTEXTUREPNG_H__

