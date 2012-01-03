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
void CCSceneAppUI::updateScene(const CCTime &gameTime)
{
    super::updateScene( gameTime );
}


void CCSceneAppUI::updateCamera(const CCTime &gameTime)
{
    const float lookAtSpeed = 1.5f;
	if( camera->interpolateCamera( gameTime.delta, lookAtSpeed ) )
    {
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
