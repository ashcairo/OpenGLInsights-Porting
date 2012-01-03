/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceRenderer.h
 * Description : Qt specific OpenGL renderer.
 *
 * Created     : 01/05/10
 * Author(s)   : Jerome Portier, Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCDEVICERENDERER_H__
#define __CCDEVICERENDERER_H__


#include "CCRenderer.h"

class CCDeviceRenderer : public CCRenderer
{
public:
    typedef CCRenderer super;

    CCDeviceRenderer();
    virtual ~CCDeviceRenderer();

private:
    const int getShaderUniformLocation(const char *name);
    const bool loadShader(CCShader *shader);

    virtual void refreshScreenSize();
};


#endif // __CCDEVICERENDERER_H__
