/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCModelBase.js
 * Description : Represents the attributes of a renderable object.
 *
 * Created     : 21/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCModelBase()
{
	this.construct();
}
CopyPrototype( CCModelBase, CCRenderable, "CCRenderable" );


CCModelBase.prototype.construct = function()
{
	this.CCRenderable_construct();
	
    this.primitives = new Array();
    this.models = new Array();
    
    this.colour = false;
	this.colourOutline = false;
}


CCModelBase.prototype.render = function(renderer, alpha)
{
	if( this.shouldRender )
	{
		GLPushMatrix();
		{
			this.refreshModelMatrix();
			GLMultMatrix( this.modelMatrix );
			
			var primitives = this.primitives;
			var primitivesLength = primitives.length;
			
			if( this.colourOutline != false )
			{
				renderer.CCSetColour( this.colourOutline );
				if( alpha == false && this.colourOutline.a > 0.0 )
				{
					gl.enable( gl.BLEND );
				}
				
				renderer.CCDefaultTexCoords();
				gEngine.textureManager.setTextureIndex( 0 );
				for( var i=0; i<primitivesLength; ++i )
				{
					primitives[i].renderOutline( renderer );
				}
				
				if( alpha == false && this.colourOutline.a > 0.0 )
				{
					gl.disable( gl.BLEND );
				}
			}

			if( this.colour != false )
			{
				renderer.CCSetColour( this.colour );
			}
			
			if( renderer.colour.a > 0 )
			{
				for( var i=0; i<primitivesLength; ++i )
				{
					primitives[i].render( renderer );
				}
			}

			var models = this.models;
			var length = models.length;
			for( var i=0; i<length; ++i )
			{
				models[i].render( renderer, alpha );
				if( this.colour != false )
				{
					renderer.CCSetColour( this.colour );
				}
			}
		}
		GLPopMatrix();
	}
}


CCModelBase.prototype.addModel = function(model)
{
	this.models.push( model );
}


CCModelBase.prototype.addPrimitive = function(primitive)
{
	this.primitives.push( primitive );
}


CCModelBase.prototype.setColour = function(inColour)
{
	if( this.colour == false )
	{
		this.colour = new CCColour();
	}
	this.colour.set( inColour );
}


CCModelBase.prototype.setOutline = function(inColour)
{
	if( this.colourOutline == false )
	{
		this.colourOutline = new CCColour();
	}
	this.colourOutline.set( inColour );
}