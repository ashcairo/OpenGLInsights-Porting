/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceRenderer.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCDeviceRenderer.h"
#include "CCGLView.h"
#include "CCFileManager.h"


CCDeviceRenderer::CCDeviceRenderer()
{
}


CCDeviceRenderer::~CCDeviceRenderer()
{
}


const int CCDeviceRenderer::getShaderUniformLocation(const char *name)
{
    return currentShader->program->uniformLocation( name );
}


const bool CCDeviceRenderer::loadShader(CCShader *shader)
{
    // Create shader program
    shader->program = new QGLShaderProgram();

    // Create and compile vertex shader
    CCText shaderFilename( "Resources/Shaders/" );
    shaderFilename += shader->name;
    shaderFilename += ".fx";

    const char *qtDefines = "#define QT\r\n";

    QGLShader *vertexShader = new QGLShader( QGLShader::Vertex );
    QGLShader *fragmentShader = new QGLShader( QGLShader::Fragment );
    {
        char *fileData = NULL;
        CCFileManager::getPackagedFile( shaderFilename.buffer, &fileData );
        {
            QString shaderData = QString( qtDefines );
            shaderData.append( "#define VERTEX_SHADER\r\n" );
            shaderData.append( fileData );
            if( vertexShader->compileSourceCode( shaderData ) == false )
            {
                QString error( "CCDeviceRenderer::Failed to compile vertex shader.\n" );
                error.append( vertexShader->log() );
                ASSERT_MESSAGE( false, error.toUtf8().constData() );

                delete vertexShader;
                vertexShader = NULL;
            }
        }
        {
            QString shaderData = QString( qtDefines );
            shaderData.append( "#define PIXEL_SHADER\r\n" );
            shaderData.append( fileData );
            if( fragmentShader->compileSourceCode( shaderData ) == false )
            {
                QString error( "CCDeviceRenderer::Failed to compile pixel shader.\n" );
                error.append( fragmentShader->log() );
                ASSERT_MESSAGE( false, error.toUtf8().constData() );

                delete fragmentShader;
                fragmentShader = NULL;
            }
        }
        FREE_POINTER( fileData );
    }
	
    // Attach vertex shader to program
    shader->program->addShader( vertexShader );
	
    // Attach fragment shader to program
    shader->program->addShader( fragmentShader );
	
    // Bind attribute locations
    // this needs to be done prior to linking
    shader->program->bindAttributeLocation( "vs_position", ATTRIB_VERTEX );
    shader->program->bindAttributeLocation( "vs_color",	ATTRIB_COLOR );
    shader->program->bindAttributeLocation( "vs_texCoord", ATTRIB_TEXCOORD );
	
    // Link program
    if( !shader->program->link() )
    {
        // Should log: "Failed to link program";
        return false;
    }

    return true;
}


void CCDeviceRenderer::refreshScreenSize()
{
    screenSize = CCSize( gView->rect().width(), gView->rect().height() );

    backBufferWidth = screenSize.width;
    backBufferHeight = screenSize.height;
}


void GLVertexAttribPointer(uint index, int size, GLenum type, bool normalized, int stride, const void *pointer)
{
    ASSERT( type == GL_FLOAT );
    gEngine->renderer->getShader()->program->setAttributeArray( index, (const GLfloat*)pointer, size, stride );
}


void GLUniform3fv(int location, int count, const GLfloat *value)
{
    gEngine->renderer->getShader()->program->setUniformValue( location, value[0], value[1], value[2] );
}


void GLUniform4fv(int location, int count, const GLfloat *value)
{
    gEngine->renderer->getShader()->program->setUniformValue( location, value[0], value[1], value[2], value[3] );
}


void GLUniformMatrix4fv(int location, int count, bool transpose, const GLfloat value[4][4])
{
    gEngine->renderer->getShader()->program->setUniformValue( location, value );
}
