/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCTextureManager.js
 * Description : Manages the loading and setting of textures.
 *
 * Created     : 20/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCTextureManager(renderer)
{
    this.renderer = renderer;
	
	this.fontPages = new Array();
	
    this.currentTextureIndex = -1;
    this.textureHandles = new Array();

    this.assignTextureIndex( 'resources/textures/white.png' )
}


CCTextureManager.prototype.loadFont = function(font)
{
	var path = "resources/fonts/" + font;
	var fontPage = new CCTextureFontPageFile( font );
	fontPage.load( path );
	this.fontPages.push( fontPage );
}


CCTextureManager.prototype.assignTextureIndex = function(src)
{
    var textureHandles = this.textureHandles;
    var length = textureHandles.length;

    for( var i=0; i<length; ++i )
    {
        var texture = textureHandles[i];
        if( src == texture.src )
        {
            return i;
        }
    }

    var texture = this.renderer.gl.createTexture();
	texture.src = src;
    texture.image = new Image();
    texture.image.texture = texture;
    texture.downloaded = false;
    texture.loaded = false;
    texture.image.onload = function() 
    {
//        unlockTextureData( this.texture );
		var texture = this.texture;
		if( texture.onLoad )
		{
			texture.onLoad.callback( this );
			texture.onLoad = false;
		}
        texture.downloaded = true;
    }
    texture.image.src = src;
    textureHandles.push( texture );
    return length;
}


function unlockTextureData(texture)
{
    var imageData = getBase64Image( texture.image );
    texture.image = new Image();
    texture.image.texture = texture;
    texture.image.onload = function() 
    {
		var texture = this.texture;
		if( texture.onLoad )
		{
			texture.onLoad.callback( this );
			texture.onLoad = false;
		}
        texture.downloaded = true;
    }
    texture.image.src = "data:image/gif;base64,"+imageData;
}


function getBase64Image(img) 
{
    // Create an empty canvas element
    var canvas = document.createElement("canvas");
    canvas.width = img.width;
    canvas.height = img.height;
    
    // Copy the image contents to the canvas
    var ctx = canvas.getContext("2d");
    ctx.drawImage(img, 0, 0);
    
    // Get the data-URL formatted image
    // Firefox supports PNG and JPEG. You could check img.src to guess the
    // original format, but be aware the using "image/jpg" will re-encode the image.
    var dataURL = canvas.toDataURL("image/png");
    
    return dataURL.replace(/^data:image\/(png|jpg);base64,/, "");
}


CCTextureManager.prototype.handleLoadedTexture = function(texture)
{
    var gl = this.renderer.gl;
    gl.activeTexture( gl.TEXTURE0 );
    gl.bindTexture( gl.TEXTURE_2D, texture );
    gl.pixelStorei( gl.UNPACK_FLIP_Y_WEBGL, true );
    gl.texImage2D( gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, texture.image );
	
    gl.texParameteri( gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR );
    gl.texParameteri( gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR );
	//gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_NEAREST);
	//gl.generateMipmap(gl.TEXTURE_2D);
	
	// Required if using non power of 2 textures
	gl.texParameteri( gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE );
	gl.texParameteri( gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE );
	
    gl.bindTexture( gl.TEXTURE_2D, null );
}


CCTextureManager.prototype.setTextureIndex = function(index)
{
    if( index != this.currentTextureIndex )
    {
        var texture = this.textureHandles[index];
        if( texture.loaded == false )
        {
            if( texture.downloaded )
            {
                this.handleLoadedTexture( texture );
                texture.loaded = true;
            }
            else
            {
				if( index != 0 )
				{
					this.setTextureIndex( 0 );
				}
				return;
            }
        }
		
		this.currentTextureIndex = index;

		var renderer = this.renderer;
		var gl = renderer.gl;

		gl.bindTexture( gl.TEXTURE_2D, texture );
		gl.uniform1i( renderer.shaderProgram.samplerUniform, 0 );
    }
}


CCTextureManager.prototype.getTexture = function(src, callback)
{
	var textureHandleIndex = this.assignTextureIndex( src );
	var handle = this.textureHandles[textureHandleIndex];
	if( handle.downloaded )
	{
		callback.callback( handle.image );
	}
	else
	{
		handle.onLoad = callback;
	}
}