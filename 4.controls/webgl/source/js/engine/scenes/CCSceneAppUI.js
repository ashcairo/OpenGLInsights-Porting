/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCSceneAppUI.js
 * Description : AppUI scene template.
 *
 * Created     : 20/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCSceneAppUI()
{
    this.construct();
}
CopyPrototype( CCSceneAppUI, CCSceneBase, "CCSceneBase" );


CCSceneAppUI.prototype.construct = function()
{
	this.CCSceneBase_construct();
			
    this.tiles = new Array();
	this.camera = false;
	
	this.controlsMoving = false;
    this.cameraScrolling = false;
	
	this.resizeCallbacks = new Array();
	this.resizing = false;
    
    this.scrollBar = false;
}


CCSceneAppUI.prototype.setup = function()
{
	if( this.camera == false )
	{
		var camera = this.camera = new CCCameraAppUI();
		gEngine.addCamera( camera );
		camera.setupViewport( 0.0, 0.0, 1.0, 1.0 );
		camera.setCameraWidth( 100.0 );
	}
	
    this.refreshCameraView();
    this.lockCameraView();
}


CCSceneAppUI.prototype.resize = function()
{   
	this.camera.setCameraWidth( 100.0 );

	var resizeCallbacks = this.resizeCallbacks;
	var length = this.resizeCallbacks.length;
    for( var i=0; i<length; ++i )
    {
        resizeCallbacks[i].run();
    }
    
    this.resizing = true;
}


CCSceneAppUI.prototype.handleControls = function(ui, state, touch, touchOverTable)
{
	if( this.resizing )
	{
		return false;
	}
	
    var usingControls = false;
	touch = this.camera.cameraControls( touch );

    if( touchOverTable && state >= touch_pressed && state <= touch_moving 
        && this.touchAllowed( touch ) )
    {
        if( !this.touching )
        {
            this.touching = true;
        }
        
        var absDeltaX = Math.abs( touch.totalDeltaX );
        var absDeltaY = Math.abs( touch.totalDeltaY );
        if( !this.controlsMoving )
        {
            if( absDeltaX > CC_TOUCH_TO_MOVEMENT_THRESHOLD || absDeltaY > CC_TOUCH_TO_MOVEMENT_THRESHOLD )
            {                           
                this.controlsMoving = true;
                this.controlsMovingVertical = absDeltaY > absDeltaX;
            }
            else
            {   
                usingControls = this.handleTilesTouch( touch, touch_pressed );
            }
        }
        
        if( this.controlsMoving )
        {   
            usingControls = this.touchMoving( touch );
        }
    }
    
    // On touch release
    else if( this.touching )
    {
        this.touching = false;
        
        if( touchOverTable && state == touch_released )
        {
            usingControls = this.touchReleased( touch );
        }
        else
        {
            this.handleTilesTouch( touch, touch_lost );
        }
        
        this.lockCameraView();
        
        // Reset state
        if( this.controlsMoving )
        {
            this.controlsMoving = false;
        }
    }

    return usingControls;
}


CCSceneAppUI.prototype.update = function(delta)
{
	this.CCSceneBase_update( delta );
}


CCSceneAppUI.prototype.updateCamera = function(delta)
{
	var lookAtSpeed = this.controlsMoving && this.cameraScrolling == false ? 20.0 : 1.5;
	if( this.camera.interpolateLookAt( delta, lookAtSpeed ) )
    {
        // Tell the scroll bar where to go
        if( this.scrollBar != false )
        {
            //this.scrollBar->reposition( camera->getLookAt().y, cameraWidth, cameraHeight );
        }
	}
    else
    {
        if( this.cameraScrolling )
        {
            this.cameraScrolling = false;
            this.lockCameraView();
        }
        
        if( this.resizing )
        {
            this.resizing = false;
            this.refreshCameraView();
            this.lockCameraView();
        }
    }
}


CCSceneAppUI.prototype.render = function(renderer, camera, pass, alpha)
{
	if( this.camera == camera )
	{
		this.CCSceneBase_render( renderer, camera, pass, alpha );
	}
}


CCSceneAppUI.prototype.scrollCameraToTop = function()
{
    this.camera.targetLookAt[1] = this.sceneTopY;
    //camera->flagUpdate();
}


CCSceneAppUI.prototype.refreshCameraView = function()
{
    this.sceneTopY = 0.0;
    this.sceneBottomY = 0.0;
}


// Lock the camera on something interesting
CCSceneAppUI.prototype.lockCameraView = function()
{
    if( this.cameraScrolling )
    {
        return;
    }
    
    //camera->flagUpdate();
    gEngine.renderer.pendingRender = true;
    this.camera.targetLookAt[0] = 0.0;
    this.camera.targetLookAt[1] = 0.0;
}


CCSceneAppUI.prototype.touchAllowed = function(touch)
{
    return touch.x > 0.0 && touch.x < 1.0 &&
    touch.y > 0.0 && touch.y < 1.0 &&
    touch.startX > 0.0 && touch.startX < 1.0 &&
    touch.startY > 0.0 && touch.startY < 1.0;
}


CCSceneAppUI.prototype.touchPressed = function()
{
}


CCSceneAppUI.prototype.touchReleased = function(touch)
{
    // Find pressed tile
    var usingControls = this.handleTilesTouch( touch, touch_released );
    if( usingControls == false )
    {
        usingControls = this.touchReleaseSwipe();
    }
    return usingControls;
}


CCSceneAppUI.prototype.touchMoving = function(touch)
{
    // Run through all the tiles
    if( this.handleTilesTouch( touch, this.controlsMovingVertical ? touch_movingVertical : touch_movingHorizontal ) )
    {
        return true;
    }

    return this.touchCameraMoving( touch );
}


CCSceneAppUI.prototype.touchReleaseSwipe = function()
{
    return false;
}


CCSceneAppUI.prototype.touchCameraMoving = function(touch)
{
    var x = touch.deltaX;
    var y = touch.deltaY;
    var camera = this.camera;
    var cameraLookAt = this.camera.targetLookAt;
    
    if( this.controlsMovingVertical )
    {
        var delta = y * camera.cameraHeight;
        if( cameraLookAt[1] > this.sceneTopY || cameraLookAt[1] < this.sceneBottomY )
        {
            delta *= 0.5;
        }
        cameraLookAt[1] += delta;
    }
    else
    {
        var delta = x * camera.cameraWidth;
        if( cameraLookAt[0] != 0.0 )
        {
            delta *= 0.5;
        }
        cameraLookAt[0] -= delta;
    }
    
    //camera->flagUpdate();
    return true;
}


CCSceneAppUI.prototype.handleTilesTouch = function(touch, touchAction)
{
    var camera = this.camera;
	if( camera.project3D( touch.x, touch.y ) )
	{
        var objects = this.objects;
        var length = objects.length;

        // Scan to see if we're blocked by a collision
        var hitPosition = vec3.create();
        var hitObject = this.basicLineCollisionCheck( objects, length,
                                                      camera.projectionNear,
                                                      camera.projectionFar,
                                                      hitPosition,
                                                      true );

        for( var i=0; i<length; ++i )
        {
            var tile = objects[i];
            if( tile.handleProjectedTouch( hitObject, hitPosition, touch, touchAction ) == 2 )
            {
                return true;
            }
        }
    }
    return false;
}


CCSceneAppUI.prototype.addTile = function(inTile)
{
    this.tiles.push( inTile );
}