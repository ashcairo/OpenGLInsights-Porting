/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright ﾩ 2011 ￐ 2011 France Telecom
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


static const bool compileShader(GLuint *shader, GLenum type, const char *path)
{
	GLint status;
	
	char *data;
	const int fileSize = CCFileManager::getPackagedFile( path, &data );

	const int offset = 41;
	char code[fileSize + offset];
	
	if( type == GL_VERTEX_SHADER )
	{
		sprintf( code, "%s%s", "#define VERTEX_SHADER\r\n", data );
	}
	else if( type == GL_FRAGMENT_SHADER )
	{
		sprintf( code, "%s%s", "#define PIXEL_SHADER\r\n", data );
	}
	
	// For safety
	code[fileSize + offset - 1] = '0';
	const char* appendedCode = (const char*)&code[0];
	
	*shader = glCreateShader( type );
    DEBUG_OPENGL();
    
	glShaderSource( *shader, 1, &appendedCode, NULL );
	DEBUG_OPENGL();
    
	glCompileShader( *shader );
	DEBUG_OPENGL();

#ifdef DEBUGON
	GLint logLength = 0;
	glGetShaderiv( *shader, GL_INFO_LOG_LENGTH, &logLength );
	if( logLength > 0 )
	{
		char *log = (char*)malloc( logLength );
		glGetShaderInfoLog( *shader, logLength, &logLength, log );
		DEBUGLOG( "Shader compile log:\n%s", log );
		free( log );
	}
#endif
	
	glGetShaderiv( *shader, GL_COMPILE_STATUS, &status );
	if( status == 0 )
	{
		glDeleteShader( *shader );
		return false;
	}
	
	return true;
}


const bool CCDeviceRenderer::linkProgram(GLuint &program)
{
    glLinkProgram( program );
    GLint linkStatus = GL_FALSE;
    glGetProgramiv( program, GL_LINK_STATUS, &linkStatus );
    if( linkStatus != GL_TRUE )
    {
        GLint bufLength = 0;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &bufLength );
        if( bufLength )
        {
            char *buf = (char*)malloc( bufLength );
            if( buf )
            {
                glGetProgramInfoLog( program, bufLength, NULL, buf );
                DEBUGLOG( "Could not link program:\n%s\n", buf );
                free( buf );
            }
        }
        glDeleteProgram( program );
        program = 0;
        return false;
    }
    return true;
}



const int CCDeviceRenderer::getShaderUniformLocation(const char *name)
{
    return glGetUniformLocation( currentShader->program, name );
}


const bool CCDeviceRenderer::loadShader(CCShader *shader)
{
	GLuint vertShader, fragShader;
	
	char filepath[100];
	sprintf( filepath, "%s%s", shader->name, ".fx" );
	const char* shaderPath = (const char*)&filepath[0];
	
    // Create shader program
	shader->program = glCreateProgram();
	
	if( !compileShader( &vertShader, GL_VERTEX_SHADER, shaderPath ) )
	{
		DEBUGLOG( "Failed to compile vertex shader." );
		glDeleteProgram( shader->program );
		shader->program = 0;
		return false;
	}
	
	if( !compileShader( &fragShader, GL_FRAGMENT_SHADER, shaderPath ) )
	{
		DEBUGLOG( "Failed to compile fragment shader." );
		glDeleteProgram( shader->program );
		shader->program = 0;
		return false;
	}
	
	DEBUG_OPENGL();
	
	// Attach shaders to program
	glAttachShader( shader->program, vertShader );
    DEBUG_OPENGL();
    
	glAttachShader( shader->program, fragShader );
    DEBUG_OPENGL();
	
    // Bind attribute locations - this needs to be done prior to linking
	glBindAttribLocation( shader->program, ATTRIB_VERTEX, "vs_position" );
    DEBUG_OPENGL();
    
	glBindAttribLocation( shader->program, ATTRIB_COLOR, "vs_color" );
    DEBUG_OPENGL();
    
	glBindAttribLocation( shader->program, ATTRIB_TEXCOORD, "vs_texCoord" );
    DEBUG_OPENGL();
	
	if( !linkProgram( shader->program ) )
	{
		DEBUGLOG( "Failed to link program." );
		if( vertShader )
		{
			glDeleteShader( vertShader );
			vertShader = 0;
		}
		
		if( fragShader )
		{
			glDeleteShader( fragShader );
			fragShader = 0;
		}
		
		if( shader->program )
		{
			glDeleteProgram( shader->program );
			shader->program = 0;
		}
		
		return false;
	}
    
    // Release vertex and fragment shaders
    if( vertShader )
	{
		glDeleteShader( vertShader );
	}
	
	if( fragShader )
	{
		glDeleteShader( fragShader );
	}
	
    return true;
}


void CCDeviceRenderer::refreshScreenSize()
{
	screenSize.width = gView->getWidth();
	screenSize.height = gView->getHeight();

	// On Android we always use the same back buffer as our view
    backBufferWidth = screenSize.width;
    backBufferHeight = screenSize.height;
}


void GLVertexAttribPointer(uint index, int size, GLenum type, bool normalized, int stride, const void *pointer)
{
	glVertexAttribPointer( index, size, type, normalized, stride, pointer );
}


void GLUniform3fv(int location, int count, const GLfloat *value)
{
    glUniform3fv( location, count, value );
}


void GLUniform4fv(int location, int count, const GLfloat *value)
{
    glUniform4fv( location, count, value );
}


void GLUniformMatrix4fv(int location, int count, bool transpose, const GLfloat value[4][4])
{
	glUniformMatrix4fv( location, count, transpose, &value[0][0] );
}
