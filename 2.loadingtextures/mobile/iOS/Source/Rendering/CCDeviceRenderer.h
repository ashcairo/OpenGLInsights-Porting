/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceRenderer.h
 * Description : iOS specific OpenGL renderer.
 *
 * Created     : 01/05/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCDEVICERENDERER_H__
#define __CCDEVICERENDERER_H__


#include "CCRenderer.h"

#define ENABLE_MULTISAMPLING 1

class CCDeviceRenderer : public CCRenderer
{
public:
    typedef CCRenderer super;
    
    CCDeviceRenderer();
	virtual ~CCDeviceRenderer();
	
	void clear();
	void render();
	
private:
	const bool linkProgram(GLuint prog);
    const int getShaderUniformLocation(const char *name);
    const bool loadShader(CCShader *shader);
    virtual const bool createContext();
	const bool createFrameBuffer();
	void destroyFrameBuffer();
    
    void refreshScreenSize();
	
private:
	EAGLContext *context;
    
    bool useMultisampling;
#if ENABLE_MULTISAMPLING
    // Buffer definitions for the MSAA
    GLuint frameBufferMSAA, renderBufferMSAA;
#endif
};


#endif // __CCDEVICERENDERER_H__