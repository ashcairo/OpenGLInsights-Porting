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
}

// Inherit from CCSceneAppUI class
CopyPrototype( SceneSample, CCSceneAppUI, "CCSceneAppUI" );


SceneSample.prototype.setup = function()
{    
	{
        var tile = new CCTile3DButton( this ).setupTexture( 1.0, "resources/f7u12_derp.png" );
    }
}


SceneSample.prototype.updateScene = function(delta)
{
    // Called once a frame and internally updates all objects managed by this scene
	this.CCSceneAppUI_updateScene( delta );
}


SceneSample.prototype.render = function(renderer, pass, alpha)
{
    // Handles drawing objects only in view
    this.CCSceneAppUI_render( renderer, pass, alpha );
}