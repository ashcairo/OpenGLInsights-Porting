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
}


// Called after our constructor is called
void SceneSample::setup()
{
    {
        CCTile3DButton *tile = new CCTile3DButton( this,                            // Pass in this scene
                                                   1.0f,                            // Specify the width of the tile
                                                   1.0f );                          // Specify the height of the tile
        tile->setBaseColour( CCColour( 1.0f, 0.0f, 0.0f, 1.0f ) );
    }
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