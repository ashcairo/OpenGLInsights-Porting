/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCTile3DButton.js
 * Description : A dynamic button widget.
 *
 * Created     : 09/08/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCTile3DButton(scene)
{
    this.construct( scene );
}
CopyPrototype( CCTile3DButton, CCTile3D, "CCTile3D" );


CCTile3DButton.prototype.construct = function(scene)
{
	this.CCTile3D_construct();
	
	if( scene )
	{
		this.setScene( scene );
	}
	
	this.renderDepth = false;
	this.setTransparent( true );

    var model = this.model = new CCModelBase();
    
    var baseModel = this.baseModel = new CCModelBase();
    model.addModel( baseModel );
	this.colourInterpolator = new CCInterpolatorColour();
	this.colourInterpolator.setDuration( 0.5 );
	this.setBaseColour( gColour.white() );
	this.baseSquare = false;
	
	this.textModel = new CCModelText( this );
}


CCTile3DButton.prototype.setup = function(width, height, text)
{
	this.setupBase( width, height );
    
    if( text )
    {
        this.textModel.setText( text, height );
    }
	
	return this;
}

// Create tile with width and textures aspect ratio
CCTile3DButton.prototype.setupTexture = function(width, textureSrc)
{
	var TextureLoadCallback = function(tile, src) 
	{
		this.tile = tile;
		this.src = src;
	}
    TextureLoadCallback.prototype.callback = function(texture)
    {
        var aspectRatio = texture.width / texture.height;
		var height = width / aspectRatio;
		this.tile.setupBase( width, height );
		
		this.tile.setBaseTexture( this.src );
    }
	
	gEngine.textureManager.getTexture( textureSrc, new TextureLoadCallback( this, textureSrc ) );
	return this;
}

// Create tile with the text height
CCTile3DButton.prototype.setupText = function(text, height, centered)
{
	var textModel = this.textModel;
	textModel.setText( text, height );
    
    var width = textModel.getWidth();
    this.setHCollisionBounds( width * 0.5, height * 0.5, CC_SMALLFLOAT );
    
    textModel.setCentered( centered );
    
    if( centered == false )
    {
        this.translate( collisionBounds.x, 0.0, 0.0 );
    }
	
	return this;
}


CCTile3DButton.prototype.destruct = function(width, height)
{
}


CCTile3DButton.prototype.update = function(delta)
{
	this.colourInterpolator.update( delta );
	
	if( this.textModel != false )
    {
        this.textModel.colourInterpolator.update( delta );
    }
}


CCTile3DButton.prototype.setupBase = function(width, height)
{
    if( this.baseSquare == false )
    {
        var baseSquare = this.baseSquare = new CCPrimitiveSquare();
        this.baseModel.addPrimitive( baseSquare );
    }

    var hWidth = width * 0.5;
    var hHeight = height * 0.5;
    this.setHCollisionBounds( hWidth, hHeight, CC_SMALLFLOAT );
	this.baseSquare.setScale( width, height, 1.0 );
    
    this.refreshCollisions();
}


CCTile3DButton.prototype.setBaseTexture = function(src)
{
	this.baseSquare.setTexture( src );
}


CCTile3DButton.prototype.setBaseColour = function(inColour)
{
	this.baseModel.setColour( inColour );
	this.colourInterpolator.setup( this.baseModel.colour );
}