/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCRenerable.h
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCRenderable.h"


CCRenderable::CCRenderable()
{
    shouldRender = true;
    dirtyModelMatrix();
	
    updateWorldMatrix = true;
	
	positionPtr = &position;
	scale = NULL;
}


void CCRenderable::destruct()
{
}


void CCRenderable::dirtyModelMatrix()
{
    updateModelMatrix = true;
}


void CCRenderable::dirtyWorldMatrix()
{
	updateWorldMatrix = true;
}


void CCRenderable::refreshModelMatrix()
{
	if( updateModelMatrix )
	{
		CCMatrixLoadIdentity( modelMatrix );	
        CCMatrixTranslate( modelMatrix, position.x, position.y, position.z );
			
		if( scale != NULL )
		{	
			CCMatrixScale( modelMatrix, scale->x, scale->y, scale->z );
		}
		
		if( rotation.x != 0.0f )
		{
            CCMatrixRotate( modelMatrix, rotation.x, 1.0f, 0.0f, 0.0f );
		}
		
		if( rotation.y != 0.0f )
		{
            CCMatrixRotate( modelMatrix, rotation.y, 0.0f, 1.0f, 0.0f );
		}
		
		if( rotation.z != 0.0f )
		{
            CCMatrixRotate( modelMatrix, rotation.z, 0.0f, 0.0f, 1.0f );
		}
			
		updateModelMatrix = false;
		dirtyWorldMatrix();
	}
}


void CCRenderable::setPosition(const CCVector3 &vector)
{
    setPositionXYZ( vector.x, vector.y, vector.z );
}


void CCRenderable::setPositionXYZ(const float x, const float y, const float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
    dirtyModelMatrix();
}


void CCRenderable::setPositionX(const float x)
{
	setPositionXYZ( x, position.y, position.z );
}


void CCRenderable::setPositionXZ(const float x, const float z)
{
	setPositionXYZ( x, position.y, z );
}


void CCRenderable::translate(CCVector3 *vector)
{
	translate( vector->x, vector->y, vector->z );
}


void CCRenderable::translate(const float x, const float y, const float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
    dirtyModelMatrix();
}


void CCRenderable::rotationUpdated()
{
    dirtyModelMatrix();
}


void CCRenderable::setRotation(const CCVector3 &vector)
{
    rotation = vector;
    rotationUpdated();
}


void CCRenderable::setRotationY(const float y)
{
    rotation.y = y;
    rotationUpdated();
}


void CCRenderable::rotateX(const float x)
{
	rotation.x += x;
    CCClampRotation( rotation.x );
    rotationUpdated();
}


void CCRenderable::rotateY(const float y)
{
	rotation.y += y;
    CCClampRotation( rotation.y );
    rotationUpdated();
}


void CCRenderable::rotateZ(const float z)
{
	rotation.z += z;
    CCClampRotation( rotation.z );
    rotationUpdated();
}
