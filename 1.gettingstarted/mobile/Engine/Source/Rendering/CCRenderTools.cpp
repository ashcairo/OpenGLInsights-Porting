/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCRenderTools.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"


// Matrix functions
//-----------------
void CCMatrixMulVec(const float matrix[16], const float in[4], float out[4])
{
    int i;
    for( i=0; i<4; ++i )
    {
        out[i] =
        in[0] * matrix[0*4+i] +
        in[1] * matrix[1*4+i] +
        in[2] * matrix[2*4+i] +
        in[3] * matrix[3*4+i];
    }
}


void CCMatrixMulMat(const float a[16], const float b[16], float r[16])
{
    int i, j;
    for( i=0; i<4; ++i )
    {
        for( j=0; j<4; ++j )
        {
             r[i*4+j] = a[i*4+0]*b[0*4+j] +	a[i*4+1]*b[1*4+j] +	a[i*4+2]*b[2*4+j] +	a[i*4+3]*b[3*4+j];
        }
    }
}


bool CCMatrixInvert(const float m[16], float invOut[16])
{
	float inv[16];
	inv[0]	=   m[5]*m[10]*m[15]	- m[5]*m[11]*m[14]	- m[9]*m[6]*m[15]
			  + m[9]*m[7]*m[14]		+ m[13]*m[6]*m[11]	- m[13]*m[7]*m[10];
	inv[4]	= - m[4]*m[10]*m[15]	+ m[4]*m[11]*m[14]	+ m[8]*m[6]*m[15]
			  - m[8]*m[7]*m[14]		- m[12]*m[6]*m[11]	+ m[12]*m[7]*m[10];
	inv[8]	=   m[4]*m[9]*m[15]		- m[4]*m[11]*m[13]	- m[8]*m[5]*m[15]
			  + m[8]*m[7]*m[13]		+ m[12]*m[5]*m[11]	- m[12]*m[7]*m[9];
	inv[12] = - m[4]*m[9]*m[14]		+ m[4]*m[10]*m[13]	+ m[8]*m[5]*m[14]
			  - m[8]*m[6]*m[13]		- m[12]*m[5]*m[10]	+ m[12]*m[6]*m[9];
	inv[1]	= - m[1]*m[10]*m[15]	+ m[1]*m[11]*m[14]	+ m[9]*m[2]*m[15]
			  - m[9]*m[3]*m[14]		- m[13]*m[2]*m[11]	+ m[13]*m[3]*m[10];
	inv[5]	=   m[0]*m[10]*m[15]	- m[0]*m[11]*m[14]	- m[8]*m[2]*m[15]
			  + m[8]*m[3]*m[14]		+ m[12]*m[2]*m[11]	- m[12]*m[3]*m[10];
	inv[9]	= - m[0]*m[9]*m[15]		+ m[0]*m[11]*m[13]	+ m[8]*m[1]*m[15]
			  - m[8]*m[3]*m[13]		- m[12]*m[1]*m[11]	+ m[12]*m[3]*m[9];
	inv[13] =   m[0]*m[9]*m[14]		- m[0]*m[10]*m[13]	- m[8]*m[1]*m[14]
			  + m[8]*m[2]*m[13]		+ m[12]*m[1]*m[10]	- m[12]*m[2]*m[9];
	inv[2]	=   m[1]*m[6]*m[15]		- m[1]*m[7]*m[14]	- m[5]*m[2]*m[15]
			  + m[5]*m[3]*m[14]		+ m[13]*m[2]*m[7]	- m[13]*m[3]*m[6];
	inv[6]	= - m[0]*m[6]*m[15]		+ m[0]*m[7]*m[14]	+ m[4]*m[2]*m[15]
			  - m[4]*m[3]*m[14]		- m[12]*m[2]*m[7]	+ m[12]*m[3]*m[6];
	inv[10] =   m[0]*m[5]*m[15]		- m[0]*m[7]*m[13]	- m[4]*m[1]*m[15]
			  + m[4]*m[3]*m[13]		+ m[12]*m[1]*m[7]	- m[12]*m[3]*m[5];
	inv[14] = - m[0]*m[5]*m[14]		+ m[0]*m[6]*m[13]	+ m[4]*m[1]*m[14]
			  - m[4]*m[2]*m[13]		- m[12]*m[1]*m[6]	+ m[12]*m[2]*m[5];
	inv[3]	= - m[1]*m[6]*m[11]		+ m[1]*m[7]*m[10]	+ m[5]*m[2]*m[11]
			  - m[5]*m[3]*m[10]		- m[9]*m[2]*m[7]	+ m[9]*m[3]*m[6];
	inv[7]	=   m[0]*m[6]*m[11]		- m[0]*m[7]*m[10]	- m[4]*m[2]*m[11]
			  + m[4]*m[3]*m[10]		+ m[8]*m[2]*m[7]	- m[8]*m[3]*m[6];
	inv[11] = - m[0]*m[5]*m[11]		+ m[0]*m[7]*m[9]	+ m[4]*m[1]*m[11]
			  - m[4]*m[3]*m[9]		- m[8]*m[1]*m[7]	+ m[8]*m[3]*m[5];
	inv[15] =   m[0]*m[5]*m[10]		- m[0]*m[6]*m[9]	- m[4]*m[1]*m[10]
			  + m[4]*m[2]*m[9]		+ m[8]*m[1]*m[6]	- m[8]*m[2]*m[5];

	float det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
	if( det == 0.0f )
	{
		return false;
	}
	
	det = 1.0f / det;

	for( int i=0; i<16; ++i )
	{
		invOut[i] = inv[i] * det;
	}
	
	return true;
}


// Render functions
//-----------------
void CCRenderSquare2D(const CCPoint &start, const CCPoint &end, const bool outline)
{
#if defined PROFILEON
    CCProfiler profile( "CCRenderSquare()" );
#endif

    CCSetViewMatrix();
	if( outline )
	{
		const float vertices[] = 
		{
			start.x, start.y,	// Top left
			end.x, start.y,		// Top right
			end.x, end.y,		// Bottom right
			start.x, end.y,		// Bottom left
			start.x, start.y,	// Top left
		};
		
		// draw the square
		GLVertexPointer( 2, GL_FLOAT, 0, vertices );
		glDrawArrays( GL_LINE_STRIP, 0, 5 );
	}
	else
	{
		const float vertices[] = 
		{
			start.x, start.y,	// Top left
			end.x, start.y,		// Top right
			start.x, end.y,		// Bottom left
			end.x, end.y,		// Bottom right
		};
		
		// draw the square
		GLVertexPointer( 2, GL_FLOAT, 0, vertices );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}
}


void CCRenderSquare(const CCVector3 &start, const CCVector3 &end)
{
    const float vertices[] = 
	{
		start.x,    start.y,    start.z,	// Top left
		end.x,      start.y,    start.z,	// Top right
		start.x,    end.y,      end.z,		// Bottom left
		end.x,      end.y,      end.z,		// Bottom right
    };
	
	// draw the square
    CCSetViewMatrix();
	GLVertexPointer( 3, GL_FLOAT, 0, vertices );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}


void CCRenderSquareYAxisAligned(const CCVector3 &start, const CCVector3 &end)
{
    const float vertices[] = 
	{
		start.x, start.y, start.z,	// Top left
		end.x, start.y, end.z,		// Top right
		start.x, end.y, start.z,	// Bottom left
		end.x, end.y, end.z,		// Bottom right
    };
	
	// draw the square
    CCSetViewMatrix();
	GLVertexPointer( 3, GL_FLOAT, 0, vertices );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}


void CCRenderSquarePoint(const CCPoint &position, const float &size)
{	
	CCRenderRectanglePoint( position, size, size );
}


void CCRenderRectanglePoint(const CCPoint &position, const float &sizeX, const float &sizeY, const bool outline)
{
    static CCPoint start, end;
	start.x = position.x - sizeX;
	start.y = position.y - sizeY;
	end.x = position.x + sizeX;
	end.y = position.y + sizeY;
	
	CCRenderSquare2D( start, end, outline );
}


void CCRenderLine(const CCVector3 &start, const CCVector3 &end)
{
	const float vertices[] =
	{
		start.x, start.y, start.z,
		end.x, end.y, end.z,
	};
	
    CCSetViewMatrix();
	GLVertexPointer( 3, GL_FLOAT, 0, vertices );
	glDrawArrays( GL_LINE_STRIP, 0, 2 );
}


void CCRenderLineTarget()
{
#define size 0.5f
	static const float vertices[] =
	{
		-1.0f, size, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f + size, 1.0f, 0.0f,
		
		1.0f, size, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f - size, 1.0f, 0.0f,
		
		-1.0f, -size, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f + size, -1.0f, 0.0f,
		
		1.0f, -size, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f - size, -1.0f, 0.0f,
		
	};
#undef size
	static const uint numberOfVerts = sizeof( vertices ) / sizeof( float ) / 3;
	
    CCSetViewMatrix();
	GLVertexPointer( 3, GL_FLOAT, 0, vertices );
	glDrawArrays( GL_LINES, 0, numberOfVerts );
}


void CCRenderCube(const bool outline)
{
	// Define the square vertices
	static const float vertices[] = 
	{
		// Front
		-1.0f, -1.0f, -1.0f,	// Bottom left	0
		1.0f, -1.0f, -1.0f,		// Bottom right	1
		-1.0f, 1.0f, -1.0f,		// Top Left		2
		1.0f, 1.0f, -1.0f,		// Top Right	3
		
		// Back
		-1.0f, -1.0f, 1.0f,		// Bottom left	4
		1.0f, -1.0f, 1.0f,		// Bottom right	5
		-1.0f, 1.0f, 1.0f,		// Top left		6
		1.0f, 1.0f, 1.0f,		// Top right	7
	};
    
    CCSetViewMatrix();
	GLVertexPointer( 3, GL_FLOAT, 0, vertices );
	
	if( outline )
	{
		static const GLubyte faces[] = 
		{
			0, 1, 3, 2, 0, 4, 5, 1, 5, 7, 3, 7, 6, 2, 6, 4, 
		};
		static const uint numberOfFaces = sizeof( faces ) / sizeof( GLubyte );
		glDrawElements( GL_LINE_STRIP, numberOfFaces, GL_UNSIGNED_BYTE, faces );
	}
	else
	{
		static const GLubyte faces[] = 
		{
			0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1,
		};
		static const uint numberOfFaces = sizeof( faces ) / sizeof( GLubyte );
		glDrawElements( GL_TRIANGLE_STRIP, numberOfFaces, GL_UNSIGNED_BYTE, faces );
	}
}


void CCRenderCubeMinMax(const CCVector3 min, const CCVector3 max, const bool outline)
{
	// Define the square vertices
	const float vertices[] = 
	{
		// Front
		min.x, min.y, min.z,		// Bottom left	0
		max.x, min.y, min.z,		// Bottom right	1
		min.x, max.y, min.z,		// Top Left		2
		max.x, max.y, min.z,		// Top Right	3
		
		// Back
		min.x, min.y, max.z,		// Bottom left	4
		max.x, min.y, max.z,		// Bottom right	5
		min.x, max.y, max.z,		// Top left		6
		max.x, max.y, max.z,		// Top right	7
	};
    
    CCSetViewMatrix();
	GLVertexPointer( 3, GL_FLOAT, 0, vertices );
	
	if( outline )
	{
		static const GLubyte faces[] = 
		{
			0, 1, 3, 2, 0, 4, 5, 1, 5, 7, 3, 7, 6, 2, 6, 4, 
		};
		static const uint numberOfFaces = sizeof( faces ) / sizeof( GLubyte );
		glDrawElements( GL_LINE_STRIP, numberOfFaces, GL_UNSIGNED_BYTE, faces );
	}
	else
	{
		static const GLubyte faces[] = 
		{
			0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1,
		};
		static const uint numberOfFaces = sizeof( faces ) / sizeof( GLubyte );
		glDrawElements( GL_TRIANGLE_STRIP, numberOfFaces, GL_UNSIGNED_BYTE, faces );
	}
}


// Shader functions
//-----------------
static CCColour currentColour = CCColour();
void CCSetColour(const CCColour &colour)
{
    if( currentColour.equals( colour ) == false )
    {
        currentColour = colour;
        GLColor4f( currentColour.red, currentColour.green, currentColour.blue, currentColour.alpha );
	}
}


const bool CCColourHasAlpha()
{
	return currentColour.alpha < 1.0f;
}


static const float *currentUVs = NULL;
void CCSetTexCoords(const float *inUVs)
{
	if( currentUVs != inUVs )
	{
		currentUVs = inUVs;
		GLTexCoordPointer( 2, GL_FLOAT, 0, currentUVs );
	}
}

void CCDefaultTexCoords()
{
	static const float texCoords[] = 
	{
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};
	CCSetTexCoords( texCoords );
}


void CCSetViewMatrix()
{
    static CCMatrix modelViewProjectionMatrix;
    CCMatrixLoadIdentity( modelViewProjectionMatrix );
    
    const GLint *uniforms = gEngine->renderer->getShader()->uniforms;
    GLUniformMatrix4fv( uniforms[UNIFORM_MODELVIEWPROJECTIONMATRIX], 1, GL_FALSE, modelViewProjectionMatrix.m );
}


void CCRefreshRenderAttributes()
{
    GLColor4f( currentColour.red, currentColour.green, currentColour.blue, currentColour.alpha );
    GLTexCoordPointer( 2, GL_FLOAT, 0, currentUVs );
    
    CCSetUniformVector3( UNIFORM_LIGHTPOSITION, 0.0f, 0.0f, 1.0f );
    CCSetUniformVector4( UNIFORM_LIGHTDIFFUSE, 1.0f, 1.0f, 1.0f, 1.0f );
}
