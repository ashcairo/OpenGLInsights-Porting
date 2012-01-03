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
}


CCTile3DButton.prototype.setup = function(width, height)
{
	this.setupBase( width, height );	
	return this;
}


CCTile3DButton.prototype.destruct = function(width, height)
{
}


CCTile3DButton.prototype.update = function(delta)
{
	this.colourInterpolator.update( delta );
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


CCTile3DButton.prototype.setBaseColour = function(inColour)
{
	this.baseModel.setColour( inColour );
	this.colourInterpolator.setup( this.baseModel.colour );
}