/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCModelText.js
 * Description : Represents a 3d text primitive.
 *
 * Created     : 23/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCModelText(inParent)
{
	this.construct();
	
    this.primitive = false;
	this.setParent( inParent );
	this.shader = "alphacolour";
	
	this.colourInterpolator = new CCInterpolatorColour();
	this.colourInterpolator.setDuration( 0.5 );
}
CopyPrototype( CCModelText, CCModelBase, "CCModelBase" );


CCModelText.prototype.setParent = function(inParent)
{
	this.parent = inParent;
    if( inParent )
	{
		if( inParent.model == false )
		{
			inParent.model = this;
		}
		else
		{
			inParent.model.addModel( this );
		}
	}
}


CCModelText.prototype.getText = function()
{
	return this.primitive.text;
}


CCModelText.prototype.getWidth = function()
{
	return this.primitive.getWidth();
}


CCModelText.prototype.setText = function(text, height, font)
{
	if( this.primitive == false )
	{
		this.translate( 0.0, 0.0, CC_SMALLFLOAT );
		this.setColour( gColour.setGrey( 0.0, 1.0 ) );
		this.primitive = new CCPrimitiveText( text );
		this.addPrimitive( this.primitive );
	}
	
	if( height )
	{
		this.setHeight( height );
	}
	
	if( font )
	{
		this.setFont( font );
	}
	else
	{
		this.setFont( "HelveticaNeueLight" );
	}
}


CCModelText.prototype.setHeight = function(height)
{
	this.primitive.setHeight( height );
}


CCModelText.prototype.setCentered = function(centered)
{
	if( centered )
	{
		this.setPositionX( 0.0 );
	}
	else if( this.parent )
	{
		this.setPositionX( -this.parent.collisionBounds[0] );
	}
	this.primitive.setCentered( centered );
}


CCModelText.prototype.setColour = function(inColour)
{
	this.CCModelBase_setColour( inColour );
	this.colourInterpolator.setup( this.colour );
}


CCModelText.prototype.setFont = function(font)
{
	this.primitive.setFont( font );
}



// CCPrimitiveText
function CCPrimitiveText(inText, inHeight)
{
	this.construct();
	
	this.setText( inText );
	this.setHeight( inHeight );
	this.setCentered( true );
}
CopyPrototype( CCPrimitiveText, CCPrimitiveBase, "CCPrimitiveBase" );


CCPrimitiveText.prototype.renderVertices = function(renderer)
{
	var text = this.text;
	this.fontPage.renderText3D( text, text.length, 0.0, 0.0, 0.0, this.height, this.centered );
}


CCPrimitiveText.prototype.setText = function(inText)
{
	this.text = inText;
}


CCPrimitiveText.prototype.setFont = function(inFont)
{
	var fontPages = gEngine.textureManager.fontPages;
	var length = fontPages.length;
	for( var i=0; i<length; ++i )
	{
		var page = fontPages[i];
		if( inFont == page.name )
		{
			this.fontPage = page;
			return;
		}
	}
}


CCPrimitiveText.prototype.setHeight = function(inHeight)
{
	this.height = inHeight;
}


CCPrimitiveText.prototype.setCentered = function(inCentered)
{
	this.centered = inCentered;
}


CCPrimitiveText.prototype.getWidth = function()
{
	var text = this.text;
	return this.fontPage.getWidth( text, text.length, this.height );
}