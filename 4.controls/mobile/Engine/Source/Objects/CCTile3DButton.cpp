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
    
    allowTouchRotation( false );
    touchRotationMagnitude = 0.0f;
    touchRotationSpeed = 1.0f;
    
    if( scale == NULL )
    {
        CCVector3FillPtr( &scale, 0.0f, 0.0f, 1.0f );
    }
    scaleInterpolator.setup( scale, 1.0f );

    allowTouchMovement( false );

    touchDepressInterpolator.setDuration( 0.125f );
    setTouchDepressRange( 1.0f );
    touchDepressDepth = 3.0f;

    CCUpdateCollisions( this );
}


void CCTile3DButton::destruct()
{
    onPress.deleteObjectsAndList();
    onRelease.deleteObjectsAndList();
    onLoss.deleteObjectsAndList();

    super::destruct();
}

// CCRenderable
void CCTile3DButton::refreshModelMatrix()
{
	if( updateModelMatrix )
	{
        CCMatrixLoadIdentity( modelMatrix );	
        CCMatrixTranslate( modelMatrix, position.x, position.y, position.z );
        CCMatrixTranslate( modelMatrix, touchDepressPosition.x, touchDepressPosition.y, touchDepressPosition.z );
        
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
        
		if( touchRotationMagnitude != 0.0f )
        {
            CCMatrixRotate( modelMatrix, touchRotationMagnitude * 20.0f, 1.0f, 0.0f, 0.0f );
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

    // Touch depress: On update
    if( touchDepressRange > 0.0f )
    {
        if( touchDepressInterpolator.update( gameTime.delta ) )
        {
            dirtyModelMatrix();
        }
    }

    if( touchRotationAllowed )
    {
        const float speed = touching || touchReleased ? 3.0f : touchRotationSpeed;
        if( touchRotationInterpolator.update( gameTime.delta * speed ) )
        {
            const float magnitudeSquared = rotation.x * rotation.x + rotation.y * rotation.y;
            touchRotationMagnitude = sqrtf( magnitudeSquared );
            touchRotationMagnitude = MIN( touchRotationMagnitude, 1.0f );

            dirtyModelMatrix();
        }
    }

    if( touching )
    {
        touchingTime += gameTime.real;
    }
    else if( touchReleased )
    {
        if( touchDepressInterpolator.finished() && touchRotationInterpolator.finished() )
        {
            handleTouchRelease();
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


const uint CCTile3DButton::handleProjectedTouch(const CCCameraProjectionResults &cameraProjectionResults,
                                                const CCSceneCollideable *hitObject, 
                                                const CCVector3 &hitPosition,
                                                const CCScreenTouches &touch, 
                                                const CCTouchAction touchAction)
{
    if( enabled == false )
    {
        return false;
    }

    if( hitObject == this && 
        ( touchAction == touch_pressed || ( touchMovementAllowed && CCControls::touchActionMoving( touchAction ) ) ) )
    {
        if( touching == false )
        {
            touching = true;
            touchingTime = 0.0f;
            onTouchPress();
        }
    }
    
    if( touching )
    {
        const float maxTimeHeld = 0.125f;
        
        if( touchAction == touch_pressed )
        {
            if( touch.timeHeld >= maxTimeHeld )
            {
                CCVector3 relativeHitPosition;
                relativeHitPosition.x = hitPosition.x - position.x;
                relativeHitPosition.y = hitPosition.y - position.y;
                float x = relativeHitPosition.x / collisionBounds.x;
                float y = relativeHitPosition.y / collisionBounds.y;
                touchActionPressed( x, y, touchAction );
            }
        }
        else if( touchMovementAllowed && CCControls::touchActionMoving( touchAction ) )
        {
            if( hitObject != this )
            {
                return handleProjectedTouch( cameraProjectionResults, hitObject, hitPosition, touch, touch_lost );
            }
            else
            {
                CCVector3 relativeHitPosition;
                relativeHitPosition.x = hitPosition.x - position.x;
                relativeHitPosition.y = hitPosition.y - position.y;
                float x = relativeHitPosition.x / collisionBounds.x;
                float y = relativeHitPosition.y / collisionBounds.y;
                touchActionPressed( x, y, touchAction );
            }
        }
        else
        {   
            // Ensure we have a good touch release
            if( touchAction == touch_released )
            {
                if( touchMovementAllowed == false )
                {
                    const float absDeltaX = fabsf( touch.totalDelta.x );
                    const float absDeltaY = fabsf( touch.totalDelta.y );
                    if( absDeltaX > CC_TOUCH_TO_MOVEMENT_THRESHOLD || absDeltaY > CC_TOUCH_TO_MOVEMENT_THRESHOLD )
                    {
                        return handleProjectedTouch( cameraProjectionResults, hitObject, hitPosition, touch, touch_lost );
                    }
                }
                
                // If we have a good first touch, register it.
                CCVector3 relativeHitPosition;
                relativeHitPosition.x = hitPosition.x - position.x;
                relativeHitPosition.y = hitPosition.y - position.y;
                float x = relativeHitPosition.x / collisionBounds.x;
                float y = relativeHitPosition.y / collisionBounds.y;
                touchActionPressed( x, y, touch_pressed );
            }

            touching = false;
            touchActionRelease( touchAction );
            
            if( touchAction == touch_released )
            {
                return 2;
            }
        }
    }
    
    return 0;
}


void CCTile3DButton::touchActionPressed(const float x, const float y, const CCTouchAction touchAction)
{
    if( touchRotationAllowed )
    {
        float clampedX = x;
        float clampedY = y;
        CCClampFloat( clampedX, -1.0f, 1.0f );
        CCClampFloat( clampedY, -1.0f, 1.0f );
        touchRotationInterpolator.pushV3( &rotation, CCVector3( -clampedY, clampedX, 0.0f ), true );
    }
    
    // Touch depress: On pressed
    if( touchDepressRange > 0.0f )
    {
        // Touch depress range specifies the area which the button is pressed down
        // If it's out of range the button is pushed back up
        if( fabsf( x ) < touchDepressRange && fabsf( y ) < touchDepressRange )
        {
            touchDepressInterpolator.pushV3( &touchDepressPosition, CCVector3( 0.0f, 0.0f, -touchDepressDepth ), true );
        }
        else
        {
            touchDepressInterpolator.pushV3( &touchDepressPosition, CCVector3(), true );
        }
    }

    if( touchAction > touch_pressed && touchAction < touch_released )
    {
        onTouchMove();
    }
}


void CCTile3DButton::touchActionRelease(const CCTouchAction touchAction)
{
    if( touchAction == touch_released )
    {
        touchReleased = true;
    }
    else 
    {
        handleTouchRelease();
        if( touchAction == touch_lost )
        {
            onTouchLoss();
        }
    }
}


void CCTile3DButton::handleTouchRelease()
{
    // If the touch has been released successfully we fire our callback
    if( touchReleased )
    {
        touchReleased = false;
        onTouchRelease();
    }

    if( touchRotationAllowed )
    {
        touchRotationInterpolator.pushV3( &rotation, CCVector3() );
    }

    // Touch depress: On releasse
    if( touchDepressRange > 0.0f )
    {
        touchDepressInterpolator.pushV3( &touchDepressPosition, CCVector3(), true );
    }
}
