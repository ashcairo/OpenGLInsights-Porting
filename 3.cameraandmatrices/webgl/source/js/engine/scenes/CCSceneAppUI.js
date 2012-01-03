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

	this.camera = false;
	
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


CCSceneAppUI.prototype.update = function(delta)
{
	this.CCSceneBase_update( delta );
}


CCSceneAppUI.prototype.updateCamera = function(delta)
{
	var lookAtSpeed = 1.5;
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