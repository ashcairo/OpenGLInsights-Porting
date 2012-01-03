/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 Ð 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceRenderer.h
 * Description : Android specific OpenGL renderer.
 *
 * Created     : 15/05/11
 * Author(s)   : Chris Bowers, Ashraf Samy Hegab
 *-----------------------------------------------------------
 */


#ifndef __CCDEVICERENDERER_H__
#define __CCDEVICERENDERER_H__

#include "CCRenderer.h"

class CCDeviceRenderer : public CCRenderer
{
public:
	~CCDeviceRenderer() {};

private:
	const bool linkProgram(GLuint &prog);
    const int getShaderUniformLocation(const char *name);
    const bool loadShader(CCShader *shader);
    const bool createContext() { return true; };
    void refreshScreenSize();
};


#endif // __CCDEVICERENDERER_H__
