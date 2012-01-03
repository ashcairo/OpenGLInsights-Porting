/*-----------------------------------------------------------
 * FaceStalker
 * http://softpoetry.com/projects/facestalker/
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 SoftPoetry
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : SceneSocialLogin.js
 * Description : Logs into Facebook.
 *
 * Created     : 18/10/11
 *-----------------------------------------------------------
 */
 
function SceneSocialLogin()
{
    this.construct();
}
CopyPrototype( SceneSocialLogin, CCSceneAppUI, "CCSceneAppUI" );


SceneSocialLogin.prototype.setup = function()
{
	this.CCSceneAppUI_setup();
	var camera = this.camera;
    camera.interpolateLookAt( CC_MAXFLOAT, CC_MAXFLOAT );
    
    // Create the user tile
    var profileTile = tile;
    {
        var tile = new TileSocialProfile( this ).setupProfile( camera.cameraWidth * 0.25, "me" );
		function TileCallback(tile)
		{
			this.tile = tile;
		}
		TileCallback.prototype.run = function()
		{
			this.tile.bufferProfilePhoto( 2 );
		}
        tile.fbInfoDownloadedCallback = new TileCallback( tile );
        tile.bufferFBInfo( 2 );
        
        this.addTile( tile );
        tile.translate( 0.0, tile.collisionBounds[1], 0.0 );
        this.profileTile = tile;
    }
    
    // Login
    //if( false )
    {
        var tile = new TileSocial( this ).setup( 80, 20, "Login" );
        tile.textModel.setColour( gColour.white() );
        tile.setBaseColour( gColour.setInt( 209, 225, 243, 0.75 ) );
        this.addTile( tile );

		function FBCallback(scene)
		{
			this.scene = scene;
		}
		FBCallback.prototype.run = function()
		{
			gEngine.removeScene( this.scene );
			gEngine.addScene( new SceneSocialController() );
		}
		
		function TileCallback2(scene)
		{
			this.scene = scene;
		}
		TileCallback2.prototype.run = function()
		{
			gEngine.fbApi.LaunchBrowser( new FBCallback( this.scene ) );
		}
		
		tile.onRelease.push( new TileCallback2( this ) );
        
        tile.positionTileBelow( this.profileTile );
        tile.translate( 0.0, -2.0, 0.0 );
    }
}

SceneSocialLogin.prototype.update = function(delta)
{
	this.CCSceneAppUI_update( delta );
}

SceneSocialLogin.prototype.render = function(renderer, camera, pass, alpha)
{
    this.CCSceneAppUI_render( renderer, camera, pass, alpha );
}


SceneSocialLogin.prototype.touchReleased = function(touch)
{
    // Find pressed tile
    var usingControls = this.handleTilesTouch( touch, touch_released );
    if( usingControls == false )
    {
        usingControls = this.touchReleaseSwipe();
    }
    return usingControls;
}


SceneSocialLogin.prototype.touchMoving = function(touch)
{
    // Run through all the tiles
    if( this.handleTilesTouch( touch, this.controlsMovingVertical ? touch_movingVertical : touch_movingHorizontal ) )
    {
        return true;
    }

    return this.touchCameraMoving( touch );
}