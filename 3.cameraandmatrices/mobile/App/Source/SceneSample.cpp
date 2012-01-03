/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : SceneSample.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "Samples.h"


SceneSample::SceneSample()
{
    // Create a new camera
    camera = new CCCameraAppUI();
    gEngine->addCamera( camera );
    
    // Set it up to take up the entire screen
    camera->setupViewport( 0.0f, 0.0f, 1.0f, 1.0f );
}


// Called after our constructor is called
void SceneSample::setup()
{
    // Set our virtual camera width to be 320
    // This means that the width of the view will be 320
    camera->setCameraWidth( 320.0f );

    {
        CCTile3DButton *tile = new CCTile3DButton( this,                            // Pass in this scene
                                                   100.0f,                          // Specify the width of the tile
                                                   "Resources/f7u12_derp.png" );    // Texture to load
    }
    
    {
        CCTile3DButton *tile = new CCTile3DButton( this,                            // Pass in this scene
                                                  100.0f,                          // Specify the width of the tile
                                                  "Resources/f7u12_troll.png" );    // Texture to load
        tile->translate( 0.0f, tile->collisionBoundsLength.y, 0.0f );
    }

    // refresh the scene range
    refreshCameraView();
}


void SceneSample::destruct()
{
    super::destruct();
}


// CCSceneBase
void SceneSample::updateScene(const CCTime &gameTime)
{
    // Called once a frame and internally updates all objects managed by this scene
    super::updateScene( gameTime );
}


void SceneSample::updateCamera(const CCTime &gameTime)
{
    // Called once a frame and internally updates the camera view
    super::updateCamera( gameTime );
}


void SceneSample::renderOctreeObject(CCSceneObject *object, const CCCameraBase *inCamera, const int pass, const bool alpha)
{
    // Called on render by our octree which handles drawing objects only in view
    if( camera == inCamera )
	{
        object->render( alpha );
	}
}