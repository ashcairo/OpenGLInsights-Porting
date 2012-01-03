/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCInterpolators.js
 * Description : Interpolators for various curves.
 *
 * Created     : 30/04/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCVarPointer(inValue)
{
    this.value = inValue;
}



function CCInterpolator()
{
    this.construct();
}

CCInterpolator.prototype.construct = function()
{
    this.speed = 1.0;
}

CCInterpolator.prototype.setDuration = function(duration)
{
    this.speed = 1.0/duration;
}



function CCInterpolatorCurve()
{
    this.construct();
}
CopyPrototype( CCInterpolatorCurve, CCInterpolator, "CCInterpolator" );

CCInterpolatorCurve.prototype.construct = function()
{
	this.CCInterpolator_construct();
	
    this.toTarget = gTools.toTarget;
    this.sin = Math.sin;
    this.interpolationCurveMultiple = 1 / this.sin( 2 );

    this.current = false;
    this.target;
    this.start;
    this.length;
    this.amount = 1.0;
}

CCInterpolatorCurve.prototype.setup = function(inCurrent, inTarget)
{
    // Ignore if we're already doing this
    if( this.equals( inCurrent, inTarget ) )
    {
        return;
    }

    this.current = inCurrent;
    this.target = inTarget;

    this.ready();
}

CCInterpolatorCurve.prototype.equals = function(inCurrent, inTarget)
{
    // Ignore if we're already doing this
    if( this.current == inCurrent && this.target == inTarget )
    {
        return true;
    }
    return false;
}

CCInterpolatorCurve.prototype.ready = function()
{
    this.start = this.current.value;
    this.length = this.target - this.start;
    this.amount = 0.0;
}

CCInterpolatorCurve.prototype.update = function(delta)
{
    this.amount = this.toTarget( this.amount, 1.0, delta * this.speed )
    if( this.amount != 1.0 )
    {
        var percentage = this.sin( this.amount * 2.0 ) * this.interpolationCurveMultiple;
        var movement = this.length * percentage;
        this.current.value = movement + this.start;
        return true;
    }
    else if( this.current != false && this.current.value != this.target )
    {
        this.current.value = this.target;
        return true;
    }
    return false;
}



function CCInterpolatorCurveX2()
{
    this.construct();
}
CopyPrototype( CCInterpolatorCurveX2, CCInterpolatorCurve );

CCInterpolatorCurveX2.prototype.update = function(delta)
{
    this.amount = this.toTarget( this.amount, 1.0, delta * this.speed )
    if( this.amount != 1.0 )
    {
        var percentage = this.amount * this.amount;
        var movement = this.length * percentage;
        this.current.value = movement + this.start;
        return true;
    }
    else if( this.current != false && this.current.value != this.target )
    {
        this.current.value = this.target;
        return true;
    }
    return false;
}



// An interpolator which handles interpolating a vec3
function CCInterpolatorCurveV3(type)
{
	this.type = type;
	this.construct();
}
CopyPrototype( CCInterpolatorCurveV3, CCInterpolator, "CCInterpolator" );

CCInterpolatorCurveV3.prototype.construct = function()
{
	this.CCInterpolator_construct();
	
    this.xPointer = new CCVarPointer();
    this.yPointer = new CCVarPointer();
    this.zPointer = new CCVarPointer();
    this.x = new this.type();
    this.y = new this.type();
    this.z = new this.type();
	
	this.onInterpolated = new Array();
}

CCInterpolatorCurveV3.prototype.equals = function(inCurrent, inTarget)
{
	if( this.x.equals( inCurrent[0], inTarget[0] ) &&
		this.y.equals( inCurrent[1], inTarget[1] ) &&
		this.z.equals( inCurrent[2], inTarget[2] ) )
	{
		return true;
	}
	return false;
}
    
CCInterpolatorCurveV3.prototype.setup = function(inCurrent, inTarget, inCallback)
{
    this.current = inCurrent;
    this.xPointer.value = inCurrent[0];
    this.yPointer.value = inCurrent[1];
    this.zPointer.value = inCurrent[2];

	inTarget = inTarget ? inTarget : inCurrent;
	this.setTarget( inTarget, inCallback );
	
	return this;
}

CCInterpolatorCurveV3.prototype.setTarget = function(inTarget, inCallback)
{
    this.x.setup( this.xPointer, inTarget[0] );
    this.y.setup( this.yPointer, inTarget[1] );
    this.z.setup( this.zPointer, inTarget[2] );
	
	this.onInterpolated.length = 0;
	if( inCallback )
	{
		this.onInterpolated.push( inCallback );
	}
}
    
CCInterpolatorCurveV3.prototype.ready = function()
{
    this.xPointer.value = this.current[0];
    this.yPointer.value = this.current[1];
    this.zPointer.value = this.current[2];

    this.x.ready();
    this.y.ready();
    this.z.ready();
}
    
CCInterpolatorCurveV3.prototype.update = function(delta)
{
    var deltaSpeed = delta * this.speed;
    var updating = this.x.update( deltaSpeed );
    updating |= this.y.update( deltaSpeed );
    updating |= this.z.update( deltaSpeed );

    if( updating )
    {
        this.current[0] = this.xPointer.value;
        this.current[1] = this.yPointer.value;
        this.current[2] = this.zPointer.value;
    }
	else
	{
		var onInterpolated = this.onInterpolated;
        var length = onInterpolated.length;
        for( var i=0; i<length; ++i )
        {
            onInterpolated[i].run();
        }
		onInterpolated.length = 0;
	}

    return updating;
}



// A list of interpolators
function CCInterpolatorsV3(type)
{
	this.type = type ? type : CCInterpolatorCurve;
	this.construct();
    this.interpolators = new Array();
}
CopyPrototype( CCInterpolatorsV3, CCInterpolator, "CCInterpolator" );

CCInterpolatorsV3.prototype.pushV3 = function(inCurrent, inTarget, replace, inCallback)
{
    var interpolators = this.interpolators;
    var length = interpolators.length;
    if( length > 0 )
    {
        if( replace )
        {
            var found = false;
            for( var i=0; i<length; ++i )
            {
                var interpolator = interpolators[i];
                if( interpolator.equals( inCurrent, inTarget ) )
                {
                    found = true;
                    
                    if( i != 0 )
                    {
                        interpolator.ready();
                    }
                }
                else
                {
                    interpolators.splice( i, 1 );
                }
            }
            
            if( found )
            {
                return false;
            }
        }
        else
        {
            for( var i=0; i<length; ++i )
            {
                var interpolator = interpolators[i];
                if( interpolator.equals( inCurrent, inTarget ) )
                {
                    return false;
                }
            }
        }
    }
    
    if( inCurrent != inTarget )
    {
        var interpolator = new CCInterpolatorCurveV3( this.type ).setup( inCurrent, inTarget, inCallback );
        interpolators.push( interpolator );
    }
    return true;
}    

CCInterpolatorsV3.prototype.update = function(delta)
{
    var interpolators = this.interpolators;
    if( interpolators.length > 0 )
    {
        var interpolator = interpolators[0];
        if( interpolator.update( delta * this.speed ) == false )
        {
            interpolators.splice( 0, 1 );
            
            // If there's another interpolation planned, tell it to ready itself to interpolate
            if( interpolators.length > 0 )
            {
                interpolators[0].ready();
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
    
CCInterpolatorsV3.prototype.finished = function()
{
    return this.interpolators.length == 0; 
}



// An interpolator for interpolating colours
function CCInterpolatorColour(type)
{
	this.type = type ? type : CCInterpolatorCurveX2;
	this.construct();
}
CopyPrototype( CCInterpolatorColour, CCInterpolator, "CCInterpolator" );

CCInterpolatorColour.prototype.construct = function()
{
	this.CCInterpolator_construct();
	
    this.rPointer = new CCVarPointer();
    this.gPointer = new CCVarPointer();
    this.bPointer = new CCVarPointer();
    this.aPointer = new CCVarPointer();
    this.r = new this.type();
    this.g = new this.type();
    this.b = new this.type();
    this.a = new this.type();
	
	this.onInterpolated = new Array();
}

CCInterpolatorColour.prototype.equals = function(inCurrent, inTarget)
{
	if( this.x.equals( inCurrent.r, inTarget.r ) &&
		this.y.equals( inCurrent.g, inTarget.g ) &&
		this.z.equals( inCurrent.b, inTarget.b ) &&
		this.a.equals( inCurrent.a, inTarget.a ) )
	{
		return true;
	}
	return false;
}
    
CCInterpolatorColour.prototype.setup = function(inCurrent, inTarget, inCallback)
{
    this.current = inCurrent;
    this.rPointer.value = inCurrent.r;
    this.gPointer.value = inCurrent.g;
    this.bPointer.value = inCurrent.b;
    this.aPointer.value = inCurrent.a;

	inTarget = inTarget ? inTarget : inCurrent;
	this.setTarget( inTarget, inCallback );
	
	return this;
}

CCInterpolatorColour.prototype.setTarget = function(inTarget, inCallback)
{
    this.r.setup( this.rPointer, inTarget.r );
    this.g.setup( this.gPointer, inTarget.g );
    this.b.setup( this.bPointer, inTarget.b );
    this.a.setup( this.aPointer, inTarget.a );
		
	this.onInterpolated.length = 0;
	if( inCallback )
	{
		this.onInterpolated.push( inCallback );
	}
}

CCInterpolatorColour.prototype.setTargetAlpha = function(inTargetAlpha, inCallback)
{
    this.a.setup( this.aPointer, inTargetAlpha );
		
	this.onInterpolated.length = 0;
	if( inCallback )
	{
		this.onInterpolated.push( inCallback );
	}
}
    
CCInterpolatorColour.prototype.ready = function()
{
    this.rPointer.value = this.current.r;
    this.gPointer.value = this.current.g;
    this.bPointer.value = this.current.b;
    this.aPointer.value = this.current.a;

    this.r.ready();
    this.g.ready();
    this.b.ready();
    this.a.ready();
}
    
CCInterpolatorColour.prototype.update = function(delta)
{
    var deltaSpeed = delta * this.speed;
    var updating = this.r.update( deltaSpeed );
    updating |= this.g.update( deltaSpeed );
    updating |= this.b.update( deltaSpeed );
    updating |= this.a.update( deltaSpeed );

    if( updating )
    {
        this.current.r = this.rPointer.value;
        this.current.g = this.gPointer.value;
        this.current.b = this.bPointer.value;
        this.current.a = this.aPointer.value;
    }
	else
	{
		var onInterpolated = this.onInterpolated;
        var length = onInterpolated.length;
        for( var i=0; i<length; ++i )
        {
            onInterpolated[i].run();
        }
		onInterpolated.length = 0;
	}

    return updating;
}