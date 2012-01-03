/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCVectors.js
 * Description : Contains point respresentative structures.
 *
 * Created     : 20/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCSize(width, height)
{
    this.width = this.height = 0.0;
	
    if( width )
    {
        this.width = width;
    }
    
    if( height )
    {
        this.height = height;
    }
};



function CCPoint(x, y)
{
    this.x = this.y = 0.0;
	
    if( x )
    {
        this.x = x;
    }
    
    if( y )
    {
        this.y = y;
    }
};



// A colour vector
function CCColour()
{
    this.white();
}


CCColour.prototype.white = function()
{
    this.setRGBA( 1.0, 1.0, 1.0, 1.0 );
	return this;
}


CCColour.prototype.set = function(colour)
{
    this.r = colour.r;
    this.g = colour.g;
    this.b = colour.b;
    this.a = colour.a;
    return this;
}


CCColour.prototype.setGrey = function(grey, a)
{
    this.setRGBA( grey, grey, grey, a );
    return this;
}


CCColour.prototype.setGreyInt = function(grey, a)
{
    this.setInt( grey, grey, grey, a );
    return this;
}


CCColour.prototype.setRGB = function(r, g, b)
{
    this.r = r;
    this.g = g;
    this.b = b;
    this.a = 1.0;
    return this;
}


CCColour.prototype.setRGBA = function(r, g, b, a)
{
    this.r = r;
    this.g = g;
    this.b = b;
    this.a = a;
    return this;
}


CCColour.prototype.setInt = function(r, g, b, a)
{
	var multiple = 1.0 / 255.0;
	this.setRGBA( r * multiple, g * multiple, b * multiple, a );
	return this;
}
var gColour = new CCColour();



function CCVector3Target()
{
	this.current = vec3.create();
	this.target = vec3.create();
}


CCVector3Target.prototype.equals = function()
{
	return vec3.equals( this.current, this.target );
}