/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTile3DButton.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCObjects.h"
#include "CCTextureFontPage.h"


CCTile3DButton::CCTile3DButton(CCSceneBase *scene)
{
    construct( scene );
}


CCTile3DButton::CCTile3DButton(CCSceneBase *scene, const float width, const float height, const char *text)
{
    construct( scene );
    
    setupBase( width, height );
    
    if( text )
    {
        textModel->setText( text, height );
    }
}


CCTile3DButton::CCTile3DButton(CCSceneBase *scene, const float width, const char *textureName)
{
    construct( scene );
    
    const CCTextureBase *texture = gEngine->textureManager->getTexture( textureName, Resource_Packaged );
    const float aspectRatio = texture->getImageWidth() / texture->getImageHeight();
    const float height = width / aspectRatio;
    setupBase( width, height );

    setBaseTexture( textureName, Resource_Packaged );
}


CCTile3DButton::CCTile3DButton(CCSceneBase *scene, const char *text, const float height, const bool centered)
{
    construct( scene );
    
    textModel->setText( text, height );
    
    const float width = textModel->getWidth();
    
    setHCollisionBounds( width * 0.5f, height * 0.5f, CC_SMALLFLOAT );
    
    textModel->setCentered( centered );
    
    if( centered == false )
    {
        translate( collisionBounds.x, 0.0f, 0.0f );
    }
}


void CCTile3DButton::construct(CCSceneBase *scene)
{
    AddFlag( collideableType, collision_ui );

    if( scene != NULL )
    {
        setScene( scene );
    }
    
    renderDepth = false;
    setTransparent();
    
    model = new CCModelBase();
    
    baseModel = new CCModelBase();
    model->addModel( baseModel );
    setBaseColour( CCColour() );
    colourInterpolator.setDuration( 0.5f );
    baseSquare = NULL;
    
    textModel = new CCModelText( this );
    
    if( scale == NULL )
    {
        CCVector3FillPtr( &scale, 0.0f, 0.0f, 1.0f );
    }
    scaleInterpolator.setup( scale, 1.0f );

    CCUpdateCollisions( this );
}


void CCTile3DButton::destruct()
{
    super::destruct();
}

// CCRenderable
void CCTile3DButton::refreshModelMatrix()
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
        
		updateModelMatrix = false;
		dirtyWorldMatrix();
	}
}


// CCSceneObject
void CCTile3DButton::update(const CCTime &gameTime)
{
    super::update( gameTime );

    if( scale != NULL )
    {
        const float speed = gameTime.delta * 1.0f;
        if( scaleInterpolator.update( speed ) )
        {
            dirtyModelMatrix();
        }
        else
        {
            DELETE_POINTER( scale );
        }
    }

    colourInterpolator.update( gameTime.delta );

    if( textModel != NULL )
    {
        textModel->colourInterpolator.update( gameTime.delta );
    }
}


void CCTile3DButton::renderModels(const bool alpha)
{
    if( alpha )
    {
        if( renderDepth )
        {
            glEnable( GL_DEPTH_TEST );
            super::renderModels( alpha );
            glDisable( GL_DEPTH_TEST );
        }
        else
        {
            super::renderModels( alpha );
        }
    }
    else
    {
        if( renderDepth )
        {
            super::renderModels( alpha );
        }
        else
        {
            glDisable( GL_DEPTH_TEST );
            super::renderModels( alpha );
            glEnable( GL_DEPTH_TEST );
            
        }
    }
}


void CCTile3DButton::setupBase(const float width, const float height)
{
    if( baseSquare == NULL )
    {
        baseSquare = new CCPrimitiveSquare();
        baseModel->addPrimitive( baseSquare );
    }

    const float hWidth = width * 0.5f;
    const float hHeight = height * 0.5f;
    setHCollisionBounds( hWidth, hHeight, CC_SMALLFLOAT );
    baseSquare->setupZFacing( collisionBounds.x, collisionBounds.y );
    CCUpdateCollisions( this );
}


void CCTile3DButton::setBaseTexture(const char *name, const CCResourceType resourceType)
{
    baseSquare->setTexture( name, resourceType );
}


void CCTile3DButton::setBaseColour(const CCColour &inColour)
{
    baseModel->setColour( inColour );
    colourInterpolator.setup( baseModel->getColour(), *baseModel->getColour() );
}


void CCTile3DButton::setBaseColourAlpha(const float inAlpha)
{
    baseModel->setColourAlpha( inAlpha );
    colourInterpolator.setup( baseModel->getColour(), *baseModel->getColour() );
}