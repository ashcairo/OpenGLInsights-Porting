/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCEngine.js
 * Description : Handles the engine loop and feature managers.
 *
 * Created     : 24/07/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */
 
 function CCEngine()
{
	this.scenes = new Array();
	this.setup();
	
	this.timer = 0;
}

function Resize()
{
    gEngine.queueResize();
}

function Scroll()
{
    gEngine.scroll();
}

CCEngine.prototype.setup = function()
{
	gEngine = this;
	
    // HTTP Requests
    this.urlManager = gURLManager = new CCURLManager();
	
	gTools = new CCTools();
	gTools.setGradient( document.body.style, '#000000', '#000000' );

	if( !gBrowserTypes['mobile'] )
	{
		window.onresize = Resize;
		window.onscroll = Scroll;
	}
	else
	{
		document.addEventListener( 'orientationchange', Resize, true );
		document.addEventListener( 'scroll', Scroll, true );
	}
	
	
	this.table = new TableComponent( document.body, false );
    this.table.table.width = '100%';
	this.table.td.align = "center";
	
	this.renderer = new CCRenderer( this.table.td );
	this.cameras = new Array();
	
	this.resize();
	
    this.textureManager = new CCTextureManager( this.renderer );
    this.textureManager.loadFont( "HelveticaNeueLight" );
	
	this.start();
	
	this.lifetime = 0;
	this.lastUpdate = 0;
	setTimeout( Update, 10 );
}

CCEngine.prototype.updateTime = function()
{
    var d = new Date();
    var currentTime = d.getMilliseconds();
    if( currentTime < this.lastUpdate )
    {
        this.lastUpdate -= 1000;
    }
    
    var delta = ( currentTime - this.lastUpdate ) * 0.001;
	delta = delta > 100 ? 100 : delta;
    this.lastUpdate = currentTime;
    
    return delta;
}

// shim layer with setTimeout fallback
window.requestAnimFrame = (function(){
    return  window.requestAnimationFrame       || 
            window.webkitRequestAnimationFrame || 
            window.mozRequestAnimationFrame    || 
            window.oRequestAnimationFrame      || 
            window.msRequestAnimationFrame     || 
            function(/* function */ callback, /* DOMElement */ element) {
				window.setTimeout(callback, 1000 / 60);
			};
})();

function Update()
{
    gEngine.updateOnce();
    requestAnimFrame( Update );
}

CCEngine.prototype.updateOnce = function()
{
	var delta = this.updateTime();
	var clampedDelta = Math.min( delta, 1 );
    
	this.lifetime += delta;
	
    if( this.resizeTimer > 0.0 )
    {
        this.resizeTimer -= delta;
        if( this.resizeTimer <= 0.0 )
        {
            this.resize();
        }
    }
	else
    {	
        var scenes = this.scenes;
        var length = scenes.length;
        for( var i=0; i<length; ++i )
        {
            scenes[i].update( delta );
        }

        var renderer = this.renderer;
		if( renderer.beginRender() )
		{
            var gl = renderer.gl;
            var render_background = renderer.render_background;
            var render_finished = renderer.render_finished;
			
			var cameras = this.cameras;
			var camerasLength = cameras.length;
			for( var cameraIndex=0; cameraIndex<camerasLength; ++cameraIndex )
			{
				var currentCamera = cameras[cameraIndex];
				if( currentCamera.enabled == false )
				{
					continue;
				}
				
				currentCamera.setViewport();
				currentCamera.update();
				
				//gl.disable( gl.DEPTH_TEST );
				for( var pass=render_background; pass<render_finished; ++pass )
				{            
					gl.disable( gl.BLEND );
					for( var i=0; i<length; ++i )
					{
						var scene = scenes[i];
						scene.render( renderer, currentCamera, pass, false );
					}
					
					gl.enable( gl.BLEND );
					for( var i=0; i<length; ++i )
					{
						var scene = scenes[i];
						scene.render( renderer, currentCamera, pass, true );
					}
				}
			}
		}
    }
	
	this.timer += delta;
}

CCEngine.prototype.queueResize = function()
{
    this.zero();
    
    // Queue a resize in half a second
    // so we don't overkill the browser if we're dragging the window size
    this.resizeTimer = 0.25;
}

CCEngine.prototype.zero = function()
{
    this.table.td.height = 1;
	this.renderer.zero();
}

CCEngine.prototype.resize = function()
{
	if( gBrowserTypes['iphone'] )
    {
        // Scroll the page down away from url bar automatically
        window.scrollTo( 0, 0 );
    }
    
    this.zero();
    
	var tools = gTools;
	tools.resize();
    
    this.table.td.height = tools.screenHeight + 'px';
	this.table.dimensions.refresh();
    
    var x = ( tools.screenWidth - this.table.dimensions.totalWidth ) * 0.5;
    var y = ( tools.screenHeight - this.table.dimensions.totalHeight ) * 0.5;
	tools.setPosition( this.table, x, y );
	
	this.renderer.resize();
	{
		var cameras = this.cameras;
		var length = cameras.length;
		for( var i=0; i<length; ++i )
		{
			cameras[i].resize();
		}
	}
	
	{
		var scenes = this.scenes;
		var length = scenes.length;
		for( var i=0; i<length; ++i )
		{
			scenes[i].resize();
		}
	}
}

CCEngine.prototype.scroll = function()
{
    gTools.resize();
	this.renderer.resize();
}


CCEngine.prototype.addCamera = function(camera)
{
	var cameras = this.cameras;
	camera.index = cameras.length;
	cameras.push( camera );
}


CCEngine.prototype.reinsertCamera = function(camera, index)
{
	this.addCamera( camera );
	if( index != -1 )
	{
		//cameras.reinsert( camera, index );
	}
}


CCEngine.prototype.removeCamera = function(camera)
{
	var newCameras = new Array();
	var cameras = this.cameras;
	var length = cameras.length;
	for( var i=0; i<length; ++i )
	{
		if( scene != cameras[i] )
		{
			newScenes.push( cameras[i] );
		}
	}
	this.cameras = newScenes;
}


CCEngine.prototype.addScene = function(scene)
{
	this.scenes.push( scene );
	scene.setup();
}


CCEngine.prototype.removeScene = function(scene)
{
	var newScenes = new Array();
	var scenes = this.scenes;
	var length = scenes.length;
	for( var i=0; i<length; ++i )
	{
		if( scene != scenes[i] )
		{
			newScenes.push( scenes[i] );
		}
	}
	this.scenes = newScenes;
	
	scene.destruct();
}


function StartUp()
{
	new CCEngine();
}

window.onload = StartUp;