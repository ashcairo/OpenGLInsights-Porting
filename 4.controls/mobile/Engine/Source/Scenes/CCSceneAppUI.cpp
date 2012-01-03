/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCSceneAppUI.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"


CCSceneAppUI::CCSceneAppUI()
{
    camera = NULL;
    cameraScrolling = false;
    // s = ( ( u+v ) / 2 ) * t;
    // u+v = ( s / t ) * 2
    //float distance = ( velocity * time ) + ( ( -velocity * pow( time, 2 ) ) / 2 );
    //    if( cameraVelocity != 0.0f )
    //    {
    //        cameraLookAt.y += cameraVelocity * gameTime.delta;
    //        ToTarget( &cameraVelocity, 0.0f, cameraReleaseVelocity * gameTime.delta * 2.0f );
    //        camera->flagUpdate();
    //        
    //        if( cameraScrolling && fabsf( cameraVelocity ) < 10.0f )
    //        {
    //            cameraScrolling = false;
    //            lockCameraView();
    //        }
    //    }

    controlsUITouch = NULL;
    controlsMoving = false;

    orientationCallback = NULL;
    updatingOrientation = false;

    scrollBar = NULL;
}


void CCSceneAppUI::setup()
{
    if( camera == NULL )
    {
        camera = new CCCameraAppUI();
        gEngine->addCamera( camera );
        camera->setupViewport( 0.0f, 0.0f, 1.0f, 1.0f );
    }
    camera->setCameraWidth( 100.0f );

    refreshCameraView();
    lockCameraView();
}


void CCSceneAppUI::destruct()
{
    orientationCallback.deleteObjectsAndList();

    if( camera != NULL )
    {
        gEngine->removeCamera( camera );
        delete camera;
    }
    
    super::destruct();
}


// CCSceneBase
const bool CCSceneAppUI::handleControls(const CCTime &gameTime)
{	
    if( updatingOrientation )
    {
        return false;
    }

    bool usingControls = false;
    CCScreenTouches *touches = (CCScreenTouches*)&camera->cameraTouches;

    // Handles two touches pressed
	if( touches[0].usingTouch != NULL && touches[1].usingTouch != NULL )
	{
        if( touchAllowed( touches[0] ) )
        {
        if( controlsUITouch == touches[0].usingTouch )
        {
            handleTilesTouch( touches[0], touch_lost );
        }
        
        if( controlsUITouch != touches[1].usingTouch )
        {
            controlsUITouch = touches[1].usingTouch;
        }
        
            usingControls = handleTwoTouches( touches[0], touches[1] );
        }
	}
	else
	{
		const CCScreenTouches &touch = touches[0];
		
		// Handles one touch pressed
		if( touches[1].lastTimeReleased > 0.0f &&
            touchAllowed( touch ) )
		{
			if( controlsUITouch != touch.usingTouch )
			{
                controlsUITouch = touch.usingTouch;
                touchPressed( touch );
			}
			
            CCPoint touchDelta = touch.delta;
            if( controlsMoving == false )
            {
                if( touchMovementAllowed( touch, touchDelta ) )
                {
                }
                else
                {   
                    usingControls = handleTilesTouch( touch, touch_pressed );
                }
            }
            
            if( controlsMoving )
            {   
                usingControls = touchMoving( touch, touchDelta );
            }
		}
		
		// On touch release
		else 
        {
            if( controlsUITouch != NULL )
            {
                if( controlsUITouch == touch.lastTouch )
                {	
                    usingControls = touchReleased( touch );
                }
                else
                {
                    handleTilesTouch( touch, touch_lost );
                }
                
                lockCameraView();
                
                // Reset state
                if( controlsMoving )
                {
                    controlsMoving = false;
                }
				controlsUITouch = NULL;
                
                return usingControls;
			}
		}
	}

    if( usingControls == false )
    {
        return super::handleControls( gameTime );
    }
    
    return usingControls;
}


void CCSceneAppUI::updateScene(const CCTime &gameTime)
{
    super::updateScene( gameTime );
}


void CCSceneAppUI::updateCamera(const CCTime &gameTime)
{
    const float lookAtSpeed = controlsUITouch && cameraScrolling == false ? 20.0f : 1.5f;
	if( camera->interpolateCamera( gameTime.delta, lookAtSpeed ) )
    {
        // Tell the scroll bar where to go
        if( scrollBar != NULL )
        {
            scrollBar->reposition( camera->getLookAt().y, camera->cameraWidth, camera->cameraHeight );
        }
	}
    else
    {
        if( cameraScrolling )
        {
            cameraScrolling = false;
            lockCameraView();
        }
        
        if( updatingOrientation )
        {
            updatingOrientation = false;
            refreshCameraView();
            lockCameraView();
        }
    }
}


const bool CCSceneAppUI::render(const CCCameraBase *inCamera, const int pass, const bool alpha)
{
    if( camera == inCamera )
    {
        renderObjects( pass, alpha );
        return true;
    }
    return false;
}


void CCSceneAppUI::renderOctreeObject(CCSceneObject *object, const CCCameraBase *inCamera, const int pass, const bool alpha)
{
    if( camera == inCamera )
	{
        object->render( alpha );
	}
}


void CCSceneAppUI::render2DBackground(const uint inCameraIndex)
{
    if( inCameraIndex == camera->getIndex() )
    {
        for( int i=0; i<childScenes.length; ++i )
        {
            childScenes.list[i]->render2DBackground( inCameraIndex );
        }
    }
}


void CCSceneAppUI::render2DForeground(const uint inCameraIndex)
{
    if( inCameraIndex == camera->getIndex() )
    {
        for( int i=0; i<childScenes.length; ++i )
        {
            childScenes.list[i]->render2DForeground( inCameraIndex );
        }
    }
}


void CCSceneAppUI::beginOrientationUpdate()
{
    camera->setCameraWidth( camera->cameraWidth );

    for( int i=0; i<orientationCallback.length; ++i )
    {
        orientationCallback.list[i]->run();
    }
    
    updatingOrientation = true;
}


void CCSceneAppUI::finishOrientationUpdate()
{
    updatingOrientation = true;
}


const bool CCSceneAppUI::touchAllowed(const CCScreenTouches &touch)
    {
    return touch.usingTouch != NULL && 
    touch.position.x > 0.0f && touch.position.x < 1.0f &&
    touch.position.y > 0.0f && touch.position.y < 1.0f &&
    touch.startPosition.x > 0.0f && touch.startPosition.x < 1.0f &&
    touch.startPosition.y > 0.0f && touch.startPosition.y < 1.0f;
}


const bool CCSceneAppUI::handleTwoTouches(const CCScreenTouches &touch1, const CCScreenTouches &touch2)
{
//    CCPoint movement;
//    // If x's are going in different directions?
//#if !defined( TARGET_OS_IPHONE )
//    movement.x = -touch1.delta.x;
//    movement.y = touch1.delta.y;
//    const CCPoint movementAbs = CCPoint( fabsf( movement.x ), fabsf( movement.y ) );
//    if( movementAbs.y > movementAbs.x )
//#else
//        movement.x = -touch1.delta.x + -touch2.delta.x;
//    movement.y = touch1.delta.y + touch2.delta.y;
//    if( gEngine->controls->detectZoomTouch() )
//#endif
//    {
//        // Find out the position of our touches
//        const CCScreenTouches *topTouch = &touch1,
//        *bottomTouch = &touch2;
//        if( touch1.position.y < touch2.position.y )
//        {
//            topTouch = &touch2;
//            bottomTouch = &touch1;
//        }
//        const CCScreenTouches *rightTouch = &touch1,
//        *leftTouch = &touch2;
//        if( touch1.position.x < touch2.position.x )
//        {
//            rightTouch = &touch2;
//            leftTouch = &touch1;
//        }
//        const float combinedDelta = topTouch->delta.y + rightTouch->delta.x + -bottomTouch->delta.y + -leftTouch->delta.x;
//        return touchCameraZooming( combinedDelta );
//    }
//    else
    {
        return touchCameraRotating( touch1.delta.x, touch1.delta.y );
    }
}


const bool CCSceneAppUI::handleThreeTouches(const CCScreenTouches &touch1, 
                                            const CCScreenTouches &touch2, 
                                            const CCScreenTouches &touch3)
{
    return false;
}


void CCSceneAppUI::touchPressed(const CCScreenTouches &touch)
{

}


const bool CCSceneAppUI::touchMovementAllowed(const CCScreenTouches &touch, CCPoint &touchDelta)
{
    const float absDeltaX = fabsf( touch.totalDelta.x );
    const float absDeltaY = fabsf( touch.totalDelta.y );
    if( absDeltaX > CC_TOUCH_TO_MOVEMENT_THRESHOLD || absDeltaY > CC_TOUCH_TO_MOVEMENT_THRESHOLD )
    {
        controlsMoving = true;
        touchDelta.x += touch.totalDelta.x;
        touchDelta.y += touch.totalDelta.y;
        
        controlsMovingVertical = absDeltaY > absDeltaX;
        return true;
    }
    return false;
}


const bool CCSceneAppUI::touchMoving(const CCScreenTouches &touch, const CCPoint &touchDelta)
{
    // Run through all the tiles
    if( handleTilesTouch( touch, controlsMovingVertical ? touch_movingVertical : touch_movingHorizontal ) )
    {
        return true;
    }

    return touchCameraMoving( touch, touchDelta.x, touchDelta.y );
}


const bool CCSceneAppUI::touchReleased(const CCScreenTouches &touch)
{
    // Find pressed tile
    bool usingControls = handleTilesTouch( touch, touch_released );
    if( usingControls == false )
    {
        usingControls = touchReleaseSwipe( touch );
    }
    return usingControls;
}


const bool CCSceneAppUI::handleTilesTouch(const CCScreenTouches &touch, const CCTouchAction touchAction)
{
    if( camera->project3D( touch.position.x, touch.position.y ) )
    {
        // Scan to see if we're blocked by a collision
        static CCVector3 hitPosition;
        CCSceneCollideable *hitObject = CCBasicLineCollisionCheck( (CCSceneCollideable**)tiles.list,
                                                                   tiles.length,
                                                                   NULL,
                                                                   camera->projection.vNear, camera->projection.vFar,
                                                                   &hitPosition,
                                                                   true,
                                                                   collision_ui,
                                                                   false );

        for( int i=0; i<tiles.length; ++i )
        {
            CCTile3D *tile = tiles.list[i];
            if( tile->handleProjectedTouch( camera->projection, hitObject, hitPosition, touch, touchAction ) == 2 )
            {
                return true;
            }
        }
    }
    return false;
}


const bool CCSceneAppUI::touchCameraMoving(const CCScreenTouches &touch, const float x, const float y)
{
    if( controlsMovingVertical )
    {
        float delta = y * camera->cameraHeight;
        if( camera->targetLookAt.y > sceneTop || camera->targetLookAt.y < sceneBottom )
        {
            delta *= 0.5f;
        }
        camera->targetLookAt.y += delta;
    }
    else
    {
        float delta = x * camera->cameraWidth;
        if( camera->targetLookAt.x < sceneLeft || camera->targetLookAt.x > sceneRight )
        {
            delta *= 0.5f;
        }
        camera->targetLookAt.x -= delta;
    }

    camera->flagUpdate();
    return true;
}


const bool CCSceneAppUI::touchCameraZooming(const float amount)
{
    camera->targetOffset.z -= amount * 2.0f * camera->getOffset().z;
    CCClampFloat( camera->targetOffset.z, 1.0f, camera->targetOffset.z );
    camera->flagUpdate();
    return true;
}


const bool CCSceneAppUI::touchCameraRotating(const float x, const float y)
{
    CCVector3 rotation = camera->getRotationVector();
    
    rotation.y += -x * 180.0f;
    CCClampFloat( rotation.y, -45.0f, 45.0f );
    camera->setRotationY( rotation.y );
    
    rotation.x += -y * 180.0f;
    CCClampFloat( rotation.x, -45.0f, 45.0f );
    camera->setRotationX( rotation.x );
    
    camera->flagUpdate();
    return true;
}


const bool CCSceneAppUI::touchReleaseSwipe(const CCScreenTouches &touch)
{
    const float maxTimeHeld = 0.5f;
    if( touch.timeHeld < maxTimeHeld )
    {
        if( controlsMovingVertical )
        {
            cameraScrolling = true;
            const CCPoint averageLastDelta = touch.averageLastDeltas();
            const float displacement = averageLastDelta.y * camera->cameraHeight * 5.0f;
            float cameraLookAtTarget = camera->targetLookAt.y + displacement;
            if( cameraLookAtTarget > sceneTop )
            {
                const float distance = cameraLookAtTarget - sceneTop;
                cameraLookAtTarget -= distance * 0.25f;
                cameraLookAtTarget = MIN( cameraLookAtTarget, sceneTop + camera->cameraHHeight * 0.5f );
            }
            else if( cameraLookAtTarget < sceneBottom )
            {
                const float distance = cameraLookAtTarget - sceneBottom;
                cameraLookAtTarget -= distance * 0.25f;
                cameraLookAtTarget = MAX( cameraLookAtTarget, sceneBottom - camera->cameraHHeight * 0.5f );
            }
            camera->targetLookAt.y = cameraLookAtTarget;
            camera->flagUpdate();
        }
        else
        {
            const float minMovementThreashold = 0.1f;
            if( touch.totalDelta.x < -minMovementThreashold )
            {
                camera->targetLookAt.x += camera->cameraHWidth;
                camera->flagUpdate();
            }
            else if( touch.totalDelta.x > minMovementThreashold )
            {
                camera->targetLookAt.x -= camera->cameraHWidth;
                camera->flagUpdate();
            }
        }
    }
    return true;
}


void CCSceneAppUI::scrollCameraToTop()
{
    camera->targetLookAt.y = sceneTop;
    camera->flagUpdate();
}


void CCSceneAppUI::refreshCameraView()
{
    sceneLeft = 0.0f;
    sceneRight = 0.0f;
    sceneTop = 0.0f;
    sceneBottom = 0.0f;
}


// Lock the camera on something interesting
void CCSceneAppUI::lockCameraView()
{
    if( cameraScrolling )
    {
        return;
    }
    
    camera->flagUpdate();
    camera->targetLookAt.x = 0.0f;
    camera->targetLookAt.y = 0.0f;
}
