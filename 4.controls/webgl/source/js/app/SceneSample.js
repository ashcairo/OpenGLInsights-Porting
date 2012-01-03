/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : SceneSample.js
 * Description : Sample demonstrating a scene with two tiles.
 *
 * Created     : 30/11/11
 *-----------------------------------------------------------
 */
 
function SceneSample()
{
    this.construct();
    
    // Create a new camera
    var camera = this.camera = new CCCameraAppUI();
    gEngine.addCamera( camera );
    
    // Set it up to take up the entire screen
    camera.setupViewport( 0.0, 0.0, 1.0, 1.0 );
}

// Inherit from CCSceneAppUI class
CopyPrototype( SceneSample, CCSceneAppUI, "CCSceneAppUI" );


SceneSample.prototype.setup = function()
{
    // Set our virtual camera width to be 320
    // This means that the width of the view will be 320
    var camera = this.camera;
	camera.setCameraWidth( 320.0 );
    
	{
        var tile = new CCTile3DButton( this ).setupTexture( 100.0, "resources/f7u12_derp.png" );
		
		function MyCallback(tile)
		{
			this.tile = tile;
		}
		MyCallback.prototype.run = function()
		{
			this.tile.setBaseTexture( "resources/f7u12_rage.png" );
		}
        tile.onRelease.push( new MyCallback( tile ) );
		
        this.addTile( tile );
    }
	
	{
        var tile = new CCTile3DButton( this ).setupTexture( 100.0, "resources/f7u12_troll.png" );
		tile.position[1] += 100.0;
		
		function MyCallback(tile)
		{
			this.tile = tile;
		}
		MyCallback.prototype.run = function()
		{
			this.tile.setBaseTexture( "resources/f7u12_rage.png" );
		}
        tile.onRelease.push( new MyCallback( tile ) );
		
        this.addTile( tile );
    }
}


SceneSample.prototype.updateScene = function(delta)
{
    // Called once a frame and internally updates all objects managed by this scene
	this.CCSceneAppUI_updateScene( delta );
}


SceneSample.prototype.render = function(renderer, camera, pass, alpha)
{
    // Handles drawing objects only in view
    this.CCSceneAppUI_render( renderer, camera, pass, alpha );
}


SceneSample.prototype.touchMoving = function(touch)
{
    // Callback for when a touch is moving
    return this.CCSceneAppUI_touchMoving( touch );
}


SceneSample.prototype.touchReleased = function(touch)
{
    // Callback for when a touch is moving
    return this.CCSceneAppUI_touchReleased( touch );
}