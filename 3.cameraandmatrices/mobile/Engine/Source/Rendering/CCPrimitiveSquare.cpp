/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCPrimitiveCube.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCPrimitives.h"
#include "CCTextureBase.h"


CCPrimitiveSquare::CCPrimitiveSquare()
{
	vertices = (float*)malloc( sizeof( float ) * 4 * 3 );
	
	customUVs = NULL;
}


void CCPrimitiveSquare::destruct()
{
	if( customUVs != NULL )
	{
		delete customUVs;
	}
	
	super::destruct();
}


// CCPrimitiveBase
void CCPrimitiveSquare::adjustTextureUVs()
{
	// We scale the textures to be square on Android
#ifndef ANDROID
    const int textureHandleIndex = textureInfo->primaryIndex;
    CCTextureHandle *textureHandle = gEngine->textureManager->getTextureHandle( textureHandleIndex );
    const CCTextureBase *texture = textureHandle->texture;
    //ASSERT( texture != NULL );
    if( texture != NULL )
    {
        const float width = texture->getImageWidth();
        const float height = texture->getImageHeight();
        const float allocatedWidth = texture->getAllocatedWidth();
        const float allocatedHeight = texture->getAllocatedHeight();
        
        if( width == allocatedWidth && height == allocatedHeight )
        {
            if( customUVs != NULL )
            {
                DELETE_POINTER( customUVs );
            }
        }
        else
        {
        	CCPrimitiveSquareUVs::Setup( &customUVs, 0.0f, 0.0f, width / allocatedWidth, height / allocatedHeight );
        }
    }
#endif
}


void CCPrimitiveSquare::renderVertices(const bool textured)
{	
#if defined PROFILEON
    CCProfiler profile( "CCPrimitiveSquare::renderVertices()" );
#endif

	if( customUVs != NULL )
	{
		CCSetTexCoords( customUVs->uvs );
	}
	else
	{
		CCDefaultTexCoords();
	}
	
    CCSetViewMatrix();
    GLVertexPointer( 3, GL_FLOAT, 0, vertices );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}


void CCPrimitiveSquare::renderOutline()
{
#if defined PROFILEON
	CCProfiler profile( "CCPrimitiveSquare::renderOutline()" );
#endif
    
    CCSetViewMatrix();
	GLVertexPointer( 3, GL_FLOAT, 0, vertices );
	
	static const GLubyte faces[] = 
	{
		0, 1, 3, 2, 0,
	};
	static const uint numberOfFaces = sizeof( faces ) / sizeof( GLubyte );

	glDrawElements( GL_LINE_STRIP, numberOfFaces, GL_UNSIGNED_BYTE, faces );
}


void CCPrimitiveSquare::setupYFacing(const float x, const float y, const float z, const bool useNormals, const float direction)
{
    setupYFacing( CCPoint( -x, y ), CCPoint( x, y ), z, useNormals, direction );
}


void CCPrimitiveSquare::setupYFacing(const CCPoint start, const CCPoint end, const float z, const bool useNormals, const float direction)
{
	vertices[0] = start.x;
	vertices[1] = start.y;
	vertices[2] = z;
	
	vertices[3] = end.x;
	vertices[4] = end.y;
	vertices[5] = z;
	
	vertices[6] = start.x;
	vertices[7] = start.y;
	vertices[8] = -z;
	
	vertices[9] = end.x;
	vertices[10] = end.y;
	vertices[11] = -z;
	
	
	if( useNormals )
	{
		normals = (float*)malloc( sizeof( float ) * 4 * 3 );
		
		normals[0] = 0.0f;
		normals[1] = 1.0f * direction;
		normals[2] = 0.0f;
		normals[3] = 0.0f;
		normals[4] = 1.0f * direction;
		normals[5] = 0.0f;
		normals[6] = 0.0f;
		normals[7] = 1.0f * direction;
		normals[8] = 0.0f;
		normals[9] = 0.0f;
		normals[10] = 1.0f * direction;
		normals[11] = 0.0f;
	}
}


void CCPrimitiveSquare::setupXFacing(const float x, const float height, const float depth)
{
	vertices[0] = x;
	vertices[1] = -height;
	vertices[2] = -depth;
	
	vertices[3] = x;
	vertices[4] = -height;
	vertices[5] = depth;
	
	vertices[6] = x;
	vertices[7] = height;
	vertices[8] = -depth;
	
	vertices[9] = x;
	vertices[10] = height;
	vertices[11] = depth;
}


void CCPrimitiveSquare::setupZFacing(const float hWidth, const float hHeight, const float z)
{
	setup( z, z, -hHeight, hHeight, -hWidth, hWidth, -hWidth, hWidth );
}


void CCPrimitiveSquare::setup(const float bZ, const float tZ, const float bY, const float tY, const float bL, const float bR, const float tL, const float tR)
{
	vertices[0] = bL;
	vertices[1] = bY;
	vertices[2] = bZ;
	
	vertices[3] = bR;
	vertices[4] = bY;
	vertices[5] = bZ;
	
	vertices[6] = tL;
	vertices[7] = tY;
	vertices[8] = tZ;
	
	vertices[9] = tR;
	vertices[10] = tY;
	vertices[11] = tZ;
}
