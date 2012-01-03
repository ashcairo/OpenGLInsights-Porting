/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCCameraAppUI.js
 * Description : AppUI Scene camera functionality.
 *
 * Created     : 20/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCCameraAppUI()
{
    this.construct();
	
	this.LookAtTarget = vec3.create();
	this.offsetTarget = vec3.create();
	
	this.lookAtInterpolator = new CCInterpolatorCurveV3( CCInterpolatorCurveX2 ).setup( this.lookAt );
	var offsetInterpolator = this.offsetInterpolator = new CCInterpolatorCurveV3( CCInterpolatorCurve ).setup( this.offset );
	offsetInterpolator.setDuration( 0.3 );
}
CopyPrototype( CCCameraAppUI, CCCameraBase, "CCCameraBase" );


CCCameraAppUI.prototype.interpolateLookAt = function(delta, speed)
{
    var updating = false;
	
	var offsetInterpolator = this.offsetInterpolator;
	var offsetTarget = this.offsetTarget;	
    var targetOffset = this.targetOffset;
	var offset = this.offset;
	if( !vec3.equals( offsetTarget, targetOffset ) )
	{
		vec3.set( targetOffset, offsetTarget );
		offsetInterpolator.setup( offset, offsetTarget );
	}
	
	if( offsetInterpolator.update( delta ) )
	{
		updating = true;
	}
	
	var lookAtInterpolator = this.lookAtInterpolator;
	var LookAtTarget = this.LookAtTarget;	
    var targetLookAt = this.targetLookAt;
	var lookAt = this.lookAt;
	if( !vec3.equals( LookAtTarget, targetLookAt ) )
	{
		vec3.set( targetLookAt, LookAtTarget );
		lookAtInterpolator.setup( lookAt, LookAtTarget );
	}
	
	if( lookAtInterpolator.update( delta * speed ) )
	{
		updating = true;
	}
    
    if( updating )
    {
        var position = this.position;
        vec3.set( lookAt, position );
        position[2] += offset[2];
    }
    
    return false;
}
