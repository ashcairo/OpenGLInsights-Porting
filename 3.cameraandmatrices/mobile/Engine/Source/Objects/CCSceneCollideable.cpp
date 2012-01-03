/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCSceneCollideable.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCObjects.h"
#include "CCSceneBase.h"
#include "CCOctree.h"
#include "CCTextureManager.h"


CCSceneCollideable::CCSceneCollideable()
{
    octreeRender = true;

	collideableType = 0;
	setHCollisionBounds( 0.5f, 0.5f, 0.5f );

    updateCollisions = true;
	octrees.allocate( 32 );
	
	drawOrder = 100;
    visible = false;
	
	owner = NULL;
}


void CCSceneCollideable::destruct()
{	
	super::destruct();
}


// CCRenderable
void CCSceneCollideable::setPositionXYZ(const float x, const float y, const float z)
{
	super::setPositionXYZ( x, y, z );
    updateCollisions = true;
    CCOctreeRefreshObject( this );
}


void CCSceneCollideable::translate(const float x, const float y, const float z)
{
	super::translate( x, y, z );
    updateCollisions = true;
}


// CCSceneObject
void CCSceneCollideable::setScene(CCSceneBase *scene)
{
	super::setScene( scene );
	AddFlag( collideableType, collision_box );
    scene->addCollideable( this );
}


void CCSceneCollideable::removeFromScene()
{	
    inScene->removeCollideable( this );
	super::removeFromScene();
}


void CCSceneCollideable::deactivate()
{
	super::deactivate();
	RemoveFlag( collideableType, collision_box );
	CCOctreeRemoveObject( this );
    
    if( owner != NULL )
	{
		owner->unOwnObject( this );
		owner = NULL;
	}
	
	for( int i=0; i<owns.length; ++i )
	{
		owns.list[i]->removeOwner( this );
	}
	owns.freeList();
}


const bool CCSceneCollideable::shouldCollide(CCSceneCollideable *collideWith, const bool initialCall)
{
	// Ask myself if we should collide
    if( super::shouldCollide( collideWith, initialCall ) == false )
    {
        return false;
	}
	
	// Ask my owner if I should collide with this object
	if( owner != NULL && owner != parent )
    {
        if( owner->shouldCollide( collideWith, initialCall ) == false )
        {
            return false;
		}
	}
	
	// Ask the other object if we should collide
	if( initialCall )
    {
        return collideWith->shouldCollide( this, false );
	}
	
    // Yeah let's collide baby
    return true;
}


void CCSceneCollideable::renderModels(const bool alpha)
{
	super::renderModels( alpha );
	
	if( alpha == transparent &&
        gEngine->renderFlags & render_collisionBoxes &&
        HasFlag( collideableType, collision_box ) )
	{
		if( transparent == false )
		{
			glEnable( GL_BLEND );
		}
		
		renderCollisionBox();
		
		if( transparent == false )
		{
			glDisable( GL_BLEND );
		}
	}
}


void CCSceneCollideable::renderCollisionBox()
{
	GLPushMatrix();
	{
		GLScalef( collisionBounds.x, collisionBounds.y, collisionBounds.z );
		//glColor4f( 1.0f, 1.0f, 0.0f, 0.5f );
        //RenderCube( true );
		
		gEngine->textureManager->setTextureIndex( 0 );
		GLRotatef( -rotation.y, 0.0f, 1.0f, 0.0f );
        static CCColour colour( 1.0f, 0.0f, 0.0f, 0.5f );
		CCSetColour( colour );
        CCRenderCube( true );
	}
	GLPopMatrix();
}


void CCSceneCollideable::setHSquareCollisionBounds(const float hSize)
{
	setHCollisionBounds( hSize, hSize, hSize );
}


void CCSceneCollideable::setHSquareCollisionBounds(const float hWidth, const float hHeight)
{
	setHCollisionBounds( hWidth, hHeight, hWidth );
}


void CCSceneCollideable::setHCollisionBounds(const float hWidth, const float hHeight, const float hDepth)
{
	collisionBounds.x = hWidth;
	collisionBounds.y = hHeight;
	collisionBounds.z = hDepth;
    
	collisionBoundsLength.x = hWidth > hDepth ? hWidth : hDepth;
    collisionBoundsLength.x *= 2.0f;
    collisionBoundsLength.y = hHeight * 2.0f;
	inverseBoundsLength.x = 1.0f / collisionBoundsLength.x;
	inverseBoundsLength.y = 1.0f / collisionBoundsLength.y;
    
    updateCollisions = true;
}


CCSceneCollideable* CCSceneCollideable::requestCollisionReport(CCSceneCollideable *collidedWith)
{
	return collidedWith->reportCollision( this, 0.0f, 0.0f, 0.0f );
}


CCSceneCollideable* CCSceneCollideable::reportCollision(CCSceneCollideable *collidedWith, const float x, const float y, const float z)
{	
	return this;
}


const bool CCSceneCollideable::reportAttack(CCSceneObject *attackedBy, const float force, const float x, const float y, const float z)
{
    return false;
}


void CCSceneCollideable::ownObject(CCSceneCollideable *object)
{
	owns.add( object );
	object->setOwner( this );
}


void CCSceneCollideable::unOwnObject(CCSceneCollideable *object)
{
	if( owns.remove( object ) )
	{
		if( owns.length == 0 )
		{
			owns.freeList();
		}
	}
}


void CCSceneCollideable::setOwner(CCSceneCollideable *newOwner)
{
	owner = newOwner;
}


void CCSceneCollideable::removeOwner(CCSceneCollideable *currentOwner)
{
	if( currentOwner == owner )
	{
		owner = NULL;
	}
}
