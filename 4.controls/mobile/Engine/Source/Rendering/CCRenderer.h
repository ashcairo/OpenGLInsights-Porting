/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCRenderer.h
 * Description : Common OpenGL renderer.
 *
 * Created     : 01/05/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCRENDERER_H__
#define __CCRENDERER_H__


enum CCRenderFlags
{
    render_all				= 0x000000001,
    render_collisionBoxes	= 0x000000002,
    render_collisionTrees	= 0x000000004,
    render_aiNodes			= 0x000000008,
    render_noPyramids		= 0x000000010,
    render_fontPage         = 0x000000012,
};

enum CCRenderPass
{
	render_background,
	render_main,
	render_finished
};

// uniform index
enum 
{
	UNIFORM_MODELVIEWPROJECTIONMATRIX,
    UNIFORM_MODELVIEWMATRIX,
    UNIFORM_MODELCOLOUR,
    
    UNIFORM_MODELNORMALMATRIX,
    UNIFORM_LIGHTPOSITION,
    UNIFORM_LIGHTDIFFUSE,
    
    UNIFORM_CAMERAPOSITION,
    
    TEXTURE_DIFFUSE,
    TEXTURE_ENV,
    
    NUM_UNIFORMS
};

// attribute index
enum 
{
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
	ATTRIB_TEXCOORD,
    ATTRIB_NORMAL,
    NUM_ATTRIBUTES
};

struct CCShader
{
    CCShader(const char *name);
    
    void use();
    
    const char *name;
    GLint uniforms[NUM_UNIFORMS];
    
#ifdef QT
    class QGLShaderProgram *program;
#else
    GLuint program;
#endif
};


class CCRenderer
{
public:
    virtual ~CCRenderer();

    void setup(const bool lighting, const bool clear);

    // Calculates the screen size parameters
    void setupScreenSizeParams();

    void update(const float delta);
    virtual void clear();
    virtual void render() {};

protected:
    virtual const bool createContext() { return true; }
    virtual const bool createFrameBuffer() { return true; }
	virtual void destroyFrameBuffer() {}
    
    virtual const int getShaderUniformLocation(const char *name) = 0;
    CCShader* loadShader(const char *name);
    virtual const bool loadShader(CCShader *shader) = 0;
    const bool loadShaders();

    // Gets the current screen size
    virtual void refreshScreenSize() = 0;

public:
    void setupOpenGL();
    
    inline const bool isPortrait() { return orientation.target == 0.0f || orientation.target == 180.0f; }
    void setOrientation(const float deviceOrientation, const bool immediate=false);
    void correctOrientation(float &x, float &y);
    
    inline const CCShader* getShader() { return currentShader; }
    const bool setShader(const char *name);
    
    inline const int getBackBufferWidth() { return backBufferWidth; }
    inline const int getBackBufferHeight() { return backBufferHeight; }

protected:
    int backBufferWidth, backBufferHeight;
    GLuint frameBuffer, renderBuffer, depthBuffer;

public:
    CCSize screenSize;
    CCSize screenSizeMultiple;
    float aspectRatio;
    float heightDivWidth;

    bool lightingEnabled;
    bool clearScreenRequired;

    CCFloatTarget orientation;
    uint updatingOrientation;
    
protected:
    CCShader *currentShader;
    CCList<CCShader> shaders;
};


// Attempt to simulate OpenGL 1.1 interface to shaders
#include "CCMatrix.h"

#define MAX_PUSHES 15
extern CCMatrix pushedMatrix[MAX_PUSHES];
extern int currentPush;

extern void GLSetPushMatrix(CCMatrix &matrix);
extern void GLPushMatrix();
extern void GLPopMatrix();
extern void GLLoadIdentity();
extern void GLMultMatrixf(CCMatrix &matrix);
extern void GLScalef(GLfloat sx, GLfloat sy, GLfloat sz);
extern void GLRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

extern void GLVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void GLTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void GLColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

extern void CCSetVertexAttribute(const uint attribute, 
                               GLint size, GLenum type, GLsizei stride,
                               const GLvoid *pointer, const bool normalized);
extern void CCSetUniformVector3(const uint uniform, 
                                const float x, const float y, const float z);
extern void CCSetUniformVector4(const uint uniform, 
                                const float x, const float y, const float z, const float w);

// Implemented in DeviceRenderer, as they require device specific hooks
extern void GLVertexAttribPointer(uint index, int size, GLenum type, bool normalized, int stride, const void *pointer);
extern void GLUniform3fv(int location, int count, const GLfloat *value); 
extern void GLUniform4fv(int location, int count, const GLfloat *value); 
extern void GLUniformMatrix4fv(int location, int count, bool transpose, const GLfloat value[4][4]);

#endif // __CCRENDERER_H__
