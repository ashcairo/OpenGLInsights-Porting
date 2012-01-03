/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCVectors.js
 * Description : Collection of math tools.
 *
 * Created     : 29/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

var CC_SMALLFLOAT = 0.1;
var CC_MAXFLOAT = 1000000000;
var CC_PI = Math.PI;
var CC_HPI = CC_PI * 0.5;


CCTools.prototype.CC_DEGREES_TO_RADIANS = function(angle)
{
	return CC_PI * angle / 180.0;
}


CCTools.prototype.CCRandom = function(amount)
{
    // Value between 0 - amount
	var random = Math.random();
	var randomInt = Math.round( random * amount );
	return randomInt;
}


CCTools.prototype.CCRandomFloat = function()
{
	var randomFloat = Math.random();
	return randomFloat;
}


CCTools.prototype.CCRandomDualFloat = function()
{
	return ( this.CCRandomFloat() - 0.5 ) * 2.0;
}


CCTools.prototype.CCRandomDualInt = function()
{
	return this.CCRandom( 1 ) * 2 - 1;
}


CCTools.prototype.CCDistance = function(first, second )
{
    return Math.abs( first - second );
}


CCTools.prototype.CCMatrixMultiply = function(result, srcA, srcB)
{
    var tmp = mat4.create();
    for( var i=0; i<16; i+=4 )
    {
        tmp[i+0] = (srcA[i+0] * srcB[0]) + (srcA[i+1] * srcB[4+0]) + (srcA[i+2] * srcB[8+0]) + (srcA[i+3] * srcB[12+0]);
        tmp[i+1] = (srcA[i+0] * srcB[1]) + (srcA[i+1] * srcB[4+1]) + (srcA[i+2] * srcB[8+1]) + (srcA[i+3] * srcB[12+1]);
        tmp[i+2] = (srcA[i+0] * srcB[2]) + (srcA[i+1] * srcB[4+2]) + (srcA[i+2] * srcB[8+2]) + (srcA[i+3] * srcB[12+2]);
        tmp[i+3] = (srcA[i+0] * srcB[3]) + (srcA[i+1] * srcB[4+3]) + (srcA[i+2] * srcB[8+3]) + (srcA[i+3] * srcB[12+3]);
    }
	mat4.set( tmp, result );
}


CCTools.prototype.CCMatrixRotate = function(result, angle, x, y, z)
{
	angle = -angle;
    var mag = Math.sqrt(x * x + y * y + z * z);

    var sinAngle = Math.sin( angle * CC_PI / 180.0 );
    var cosAngle = Math.cos( angle * CC_PI / 180.0 );
    if ( mag > 0.0 )
    {
        var xx, yy, zz, xy, yz, zx, xs, ys, zs;
        var oneMinusCos;
        var rotMat = mat4.create();

        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0 - cosAngle;

        rotMat[0] = (oneMinusCos * xx) + cosAngle;
        rotMat[1] = (oneMinusCos * xy) - zs;
        rotMat[2] = (oneMinusCos * zx) + ys;
        rotMat[3] = 0.0;

        rotMat[1*4+0] = (oneMinusCos * xy) + zs;
        rotMat[1*4+1] = (oneMinusCos * yy) + cosAngle;
        rotMat[1*4+2] = (oneMinusCos * yz) - xs;
        rotMat[1*4+3] = 0.0;

        rotMat[2*4+0] = (oneMinusCos * zx) - ys;
        rotMat[2*4+1] = (oneMinusCos * yz) + xs;
        rotMat[2*4+2] = (oneMinusCos * zz) + cosAngle;
        rotMat[2*4+3] = 0.0;

        rotMat[3*4+0] = 0.0;
        rotMat[3*4+1] = 0.0;
        rotMat[3*4+2] = 0.0;
        rotMat[3*4+3] = 1.0;

        mat4.multiply( rotMat, result, result );
    }
}