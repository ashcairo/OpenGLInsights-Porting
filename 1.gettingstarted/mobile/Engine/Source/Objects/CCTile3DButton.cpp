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


CCTile3DButton::CCTile3DButton(CCSceneBase *scene)
{
    construct( scene );
}


CCTile3DButton::CCTile3DButton(CCSceneBase *scene, const float width, const float height)
{
    construct( scene );
    
    setupBase( width, height );
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
}


void CCTile3DButton::destruct()
{
    super::destruct();
}


// CCSceneObject
void CCTile3DButton::update(const CCTime &gameTime)
{
    super::update( gameTime );

    colourInterpolator.update( gameTime.delta );
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