/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCCameraBase.js
 * Description : Common functionality for scene cameras.
 *
 * Created     : 20/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCCameraBase()
{
	this.construct();
}

CCCameraBase.prototype.construct = function()
{
    this.renderer = gEngine.renderer;
	this.gl = this.renderer.gl;
	this.enabled = true;
    
	// Create our variables
	this.modelViewMatrix = mat4.create();
	this.projectionMatrix = mat4.create();
	this.projectionNear = vec3.create();
	this.projectionFar = vec3.create();
	this.projectionDirection = vec3.create();
    
    this.offset = vec3.create( [0,0,0] );
    this.position = vec3.create( [0,0,0] );
    this.lookAt = vec3.create( [0,0,0] );
    
    this.targetOffset = vec3.create( [0,0,0] );
    this.targetLookAt = vec3.create( [0,0,0] );
	
	this.viewport = vec4.create();
	
	this.frustum_right	= 0*4;
	this.frustum_left	= 1*4;
	this.frustum_bottom	= 2*4;
	this.frustum_top	= 3*4;
	this.frustum_far	= 4*4;
	this.frustum_near	= 5*4;
	this.frustum_max	= 6;
	this.frustum = new Float32Array( this.frustum_max * 4 );
	this.frustumClip = new Float32Array( 16 );
	
	this.cameraTouch = new Touch();
}


CCCameraBase.prototype.resize = function()
{	
	this.setupViewportFromCamera( this );
    this.setCameraWidth( this.cameraWidth );
}


CCCameraBase.prototype.setupViewportFromCamera = function(inCamera)
{
	this.setupViewport( inCamera.cameraX, inCamera.cameraY, inCamera.cameraW, inCamera.cameraH );
}


CCCameraBase.prototype.setupViewport = function(x, y, width, height)
{
	var viewport = this.viewport;
	var renderer = this.renderer;
	var canvas = renderer.canvas;
	
	var invY = ( 1.0-height ) - y;
    
    var screenWidth = canvas.width;
    var screenHeight = canvas.height;
    var definedWidth = width * screenWidth;
    var definedHeight = height * screenHeight;
    viewport[0] = x * screenWidth;
    viewport[1] = invY * screenHeight;
    viewport[2] = definedWidth;
    viewport[3] = definedHeight;

    this.cameraX = x;
    this.cameraY = y;
    this.cameraW = width;
    this.cameraH = height;
    this.invCameraW = 1.0 / width;
    this.invCameraH = 1.0 / height;
    
    this.aspectRatio = definedWidth / definedHeight;
    
    this.setupPerspective();
}


CCCameraBase.prototype.setViewport = function()
{
    var viewport = this.viewport;
    this.gl.viewport( viewport[0], viewport[1], viewport[2], viewport[3] );
    this.gl.scissor( viewport[0], viewport[1], viewport[2], viewport[3] );
}


CCCameraBase.prototype.setupPerspective = function()
{
    this.gluPerspective( this.projectionMatrix, 60, this.aspectRatio, 1.0, 3000.0 );
}


CCCameraBase.prototype.update = function()
{
    var modelViewMatrix = this.modelViewMatrix;
    mat4.identity( this.modelViewMatrix );
    
    var position = this.position;
    var lookAt = this.lookAt;
    this.gluLookAt( position[0], position[1], position[2],
                    lookAt[0], lookAt[1], lookAt[2],
                    0.0, 1.0, 0.0 );
					
	mat4.set( this.modelViewMatrix, mvMatrix );

	var pMatrix = this.projectionMatrix;
	this.gl.uniformMatrix4fv( this.renderer.shaderProgram.pMatrixUniform, false, pMatrix );
	
	this.ExtractFrustum();
}


CCCameraBase.prototype.interpolateLookAt = function(delta, speed)
{
    return false;
}


CCCameraBase.prototype.cameraControls = function(screenTouch)
{
	var cameraTouch = this.cameraTouch;
	
	cameraTouch.x = screenTouch.x;
    cameraTouch.y = screenTouch.y;
    cameraTouch.startX = screenTouch.startX;
    cameraTouch.startY = screenTouch.startY;
    cameraTouch.deltaX = screenTouch.deltaX;
    cameraTouch.deltaY = screenTouch.deltaY;
    cameraTouch.totalDeltaX = screenTouch.totalDeltaX;
    cameraTouch.totalDeltaY = screenTouch.totalDeltaY;

    cameraTouch.touching = screenTouch.touching;
    cameraTouch.timeHeld = screenTouch.timeHeld;
	
	var cameraX = this.cameraX;
	var invCameraW = this.invCameraW;
	var cameraY = this.cameraY;
	var invCameraH = this.invCameraH;
	
	cameraTouch.startX -= cameraX;
	cameraTouch.startX *= invCameraW;
	cameraTouch.startY -= cameraY;
	cameraTouch.startY *= invCameraH;

	cameraTouch.x -= cameraX;
	cameraTouch.x *= invCameraW;
	cameraTouch.y -= cameraY;
	cameraTouch.y *= invCameraH;

	cameraTouch.deltaX *= invCameraW;
	cameraTouch.deltaY *= invCameraH;
	cameraTouch.totalDeltaX *= invCameraW;
	cameraTouch.totalDeltaY *= invCameraH;
	
	return cameraTouch;
}


CCCameraBase.prototype.gluPerspective = function(matrix, fovy, aspect, zNear, zFar)
{
    var top = zNear * Math.tan( fovy * Math.PI / 360.0 );
    var right = top * aspect;
    
    var zNearScale = 1.0 / zNear;
    this.frustumWidth = right * 2.0 * zNearScale;
    this.frustumHeight = top * 2.0 * zNearScale;
    
    return mat4.frustum( -right, right, -top, top, zNear, zFar, matrix );
}


// Set's the camera's width
CCCameraBase.prototype.setCameraWidth = function(inWidth)
{
    this.cameraWidth = inWidth;
    this.targetOffset[2] = this.cameraWidth / this.frustumWidth;
    
    this.cameraHeight = this.targetOffset[2] * this.frustumHeight;
    this.cameraHWidth = this.cameraWidth * 0.5;
    this.cameraHHeight = this.cameraHeight * 0.5;
}


CCCameraBase.prototype.project3D = function(x, y)
{
	var viewport = this.viewport;
	y = 1.0 - y;
	
	x *= viewport[2];
	y *= viewport[3];
    y += viewport[1];

	if( this.gluUnProject( x, y, 0.0, this.projectionNear ) &&
        this.gluUnProject( x, y, 1.0, this.projectionFar ) )
	{
		// Figure out our ray's direction
        vec3.set( this.projectionFar, this.projectionDirection );
		vec3.subtract( this.projectionDirection, this.projectionNear );
		vec3.unitize( this.projectionDirection );
		return true;
	}
	
	return false;
}


CCCameraBase.prototype.gluLookAt = function(eyex, eyey, eyez, 
                                      centerx, centery, centerz,
                                      upx, upy, upz)
{
    var x = vec3.create();
    var y = vec3.create();
    var z = vec3.create();
	
	/* Make rotation matrix */
	
	/* Z vector */
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;
	var mag = Math.sqrt( z[0] * z[0] + z[1] * z[1] + z[2] * z[2] );
	if( mag )
	{	/* mpichler, 19950515 */
		z[0] /= mag;
		z[1] /= mag;
		z[2] /= mag;
	}
	
	/* Y vector */
	y[0] = upx;
	y[1] = upy;
	y[2] = upz;
	
	/* X vector = Y cross Z */
	x[0] = y[1] * z[2] - y[2] * z[1];
	x[1] = -y[0] * z[2] + y[2] * z[0];
	x[2] = y[0] * z[1] - y[1] * z[0];
	
	/* Recompute Y = Z cross X */
	y[0] = z[1] * x[2] - z[2] * x[1];
	y[1] = -z[0] * x[2] + z[2] * x[0];
	y[2] = z[0] * x[1] - z[1] * x[0];
	
	/* mpichler, 19950515 */
	/* cross product gives area of parallelogram, which is < 1.0 for
	 * non-perpendicular unit-length vectors; so normalize x, y here
	 */
	
	mag = Math.sqrt( x[0] * x[0] + x[1] * x[1] + x[2] * x[2] );
	if( mag )
	{
		x[0] /= mag;
		x[1] /= mag;
		x[2] /= mag;
	}
	
	mag = Math.sqrt( y[0] * y[0] + y[1] * y[1] + y[2] * y[2] );
	if( mag )
	{
		y[0] /= mag;
		y[1] /= mag;
		y[2] /= mag;
	}
	
    var m = mat4.create();
	m[4*0+0] = x[0];
	m[4*0+1] = x[1];
	m[4*0+2] = x[2];
	m[4*0+3] = 0.0;
	m[4*1+0] = y[0];
	m[4*1+1] = y[1];
	m[4*1+2] = y[2];
	m[4*1+3] = 0.0;
	m[4*2+0] = z[0];
	m[4*2+1] = z[1];
	m[4*2+2] = z[2];
	m[4*2+3] = 0.0;
	m[4*3+0] = 0.0;
	m[4*3+1] = 0.0;
	m[4*3+2] = 0.0;
	m[4*3+3] = 1.0;
    
    var modelViewMatrix = this.modelViewMatrix;
    mat4.multiply( m, modelViewMatrix, modelViewMatrix );
	
	/* Translate Eye to Origin */
    var eyeVector = vec3.create();
    eyeVector[0] = -eyex;
    eyeVector[1] = -eyey;
    eyeVector[2] = -eyez;
    mat4.translate( modelViewMatrix, eyeVector );
}


CCCameraBase.prototype.gluUnProject = function(winX, winY, winZ, resultVector)
{
	var finalMatrix = mat4.create();
	var input = vec4.create();
	var out = vec4.create();
	
	mat4.multiply( this.modelViewMatrix, this.projectionMatrix, finalMatrix );
	if( mat4.inverse( finalMatrix, finalMatrix ) == false ) 
	{
		return false;
	}
	
	input[0] = winX;
	input[1] = winY;
	input[2] = winZ;
	input[3] = 1.0;
	
	/* Map x and y from window coordinates */
	var viewport = this.viewport;
	input[0] = ( input[0] - viewport[0] ) / viewport[2];
	input[1] = ( input[1] - viewport[1] ) / viewport[3];
	
	/* Map to range -1 to 1 */
	input[0] = input[0] * 2 - 1;
	input[1] = input[1] * 2 - 1;
	input[2] = input[2] * 2 - 1;
	
	mat4.multiplyVec4( finalMatrix, input, out );
	if( out[3] == 0.0 )
	{
		return false;
	}
	
	out[0] /= out[3];
	out[1] /= out[3];
	out[2] /= out[3];
	resultVector[0] = out[0];
	resultVector[1] = out[1];
	resultVector[2] = out[2];
	
	return true;
}


CCCameraBase.prototype.ExtractFrustum = function()
{
	var proj = this.projectionMatrix;
	var	modv = this.modelViewMatrix;
	var clip = this.frustumClip;
	var t;
	var frustum = this.frustum;
	var frustum_right	= this.frustum_right;
	var frustum_left	= this.frustum_left;
	var frustum_bottom	= this.frustum_bottom;
	var frustum_top		= this.frustum_top;
	var frustum_far		= this.frustum_far;
	var frustum_near	= this.frustum_near;
	
	/* Combine the two matrices (multiply projection by modelview) */
	clip[ 0] = modv[ 0] * proj[ 0] + modv[ 1] * proj[ 4] + modv[ 2] * proj[ 8] + modv[ 3] * proj[12];
	clip[ 1] = modv[ 0] * proj[ 1] + modv[ 1] * proj[ 5] + modv[ 2] * proj[ 9] + modv[ 3] * proj[13];
	clip[ 2] = modv[ 0] * proj[ 2] + modv[ 1] * proj[ 6] + modv[ 2] * proj[10] + modv[ 3] * proj[14];
	clip[ 3] = modv[ 0] * proj[ 3] + modv[ 1] * proj[ 7] + modv[ 2] * proj[11] + modv[ 3] * proj[15];
	
	clip[ 4] = modv[ 4] * proj[ 0] + modv[ 5] * proj[ 4] + modv[ 6] * proj[ 8] + modv[ 7] * proj[12];
	clip[ 5] = modv[ 4] * proj[ 1] + modv[ 5] * proj[ 5] + modv[ 6] * proj[ 9] + modv[ 7] * proj[13];
	clip[ 6] = modv[ 4] * proj[ 2] + modv[ 5] * proj[ 6] + modv[ 6] * proj[10] + modv[ 7] * proj[14];
	clip[ 7] = modv[ 4] * proj[ 3] + modv[ 5] * proj[ 7] + modv[ 6] * proj[11] + modv[ 7] * proj[15];
	
	clip[ 8] = modv[ 8] * proj[ 0] + modv[ 9] * proj[ 4] + modv[10] * proj[ 8] + modv[11] * proj[12];
	clip[ 9] = modv[ 8] * proj[ 1] + modv[ 9] * proj[ 5] + modv[10] * proj[ 9] + modv[11] * proj[13];
	clip[10] = modv[ 8] * proj[ 2] + modv[ 9] * proj[ 6] + modv[10] * proj[10] + modv[11] * proj[14];
	clip[11] = modv[ 8] * proj[ 3] + modv[ 9] * proj[ 7] + modv[10] * proj[11] + modv[11] * proj[15];
	
	clip[12] = modv[12] * proj[ 0] + modv[13] * proj[ 4] + modv[14] * proj[ 8] + modv[15] * proj[12];
	clip[13] = modv[12] * proj[ 1] + modv[13] * proj[ 5] + modv[14] * proj[ 9] + modv[15] * proj[13];
	clip[14] = modv[12] * proj[ 2] + modv[13] * proj[ 6] + modv[14] * proj[10] + modv[15] * proj[14];
	clip[15] = modv[12] * proj[ 3] + modv[13] * proj[ 7] + modv[14] * proj[11] + modv[15] * proj[15];
	
	/* Extract the numbers for the RIGHT plane */
	frustum[frustum_right+0] = clip[ 3] - clip[ 0];
	frustum[frustum_right+1] = clip[ 7] - clip[ 4];
	frustum[frustum_right+2] = clip[11] - clip[ 8];
	frustum[frustum_right+3] = clip[15] - clip[12];
	
	/* Normalize the result */
	t = Math.sqrt( frustum[frustum_right+0] * frustum[frustum_right+0] + frustum[frustum_right+1] * frustum[frustum_right+1] + frustum[frustum_right+2] * frustum[frustum_right+2] );
	frustum[frustum_right+0] /= t;
	frustum[frustum_right+1] /= t;
	frustum[frustum_right+2] /= t;
	frustum[frustum_right+3] /= t;
	
	/* Extract the numbers for the LEFT plane */
	frustum[frustum_left+0] = clip[ 3] + clip[ 0];
	frustum[frustum_left+1] = clip[ 7] + clip[ 4];
	frustum[frustum_left+2] = clip[11] + clip[ 8];
	frustum[frustum_left+3] = clip[15] + clip[12];
	
	/* Normalize the result */
	t = Math.sqrt( frustum[frustum_left+0] * frustum[frustum_left+0] + frustum[frustum_left+1] * frustum[frustum_left+1] + frustum[frustum_left+2] * frustum[frustum_left+2] );
	frustum[frustum_left+0] /= t;
	frustum[frustum_left+1] /= t;
	frustum[frustum_left+2] /= t;
	frustum[frustum_left+3] /= t;
	
	/* Extract the BOTTOM plane */
	frustum[frustum_bottom+0] = clip[ 3] + clip[ 1];
	frustum[frustum_bottom+1] = clip[ 7] + clip[ 5];
	frustum[frustum_bottom+2] = clip[11] + clip[ 9];
	frustum[frustum_bottom+3] = clip[15] + clip[13];
	
	/* Normalize the result */
	t = Math.sqrt( frustum[frustum_bottom+0] * frustum[frustum_bottom+0] + frustum[frustum_bottom+1] * frustum[frustum_bottom+1] + frustum[frustum_bottom+2] * frustum[frustum_bottom+2] );
	frustum[frustum_bottom+0] /= t;
	frustum[frustum_bottom+1] /= t;
	frustum[frustum_bottom+2] /= t;
	frustum[frustum_bottom+3] /= t;
	
	/* Extract the TOP plane */
	frustum[frustum_top+0] = clip[ 3] - clip[ 1];
	frustum[frustum_top+1] = clip[ 7] - clip[ 5];
	frustum[frustum_top+2] = clip[11] - clip[ 9];
	frustum[frustum_top+3] = clip[15] - clip[13];
	
	/* Normalize the result */
	t = Math.sqrt( frustum[frustum_top+0] * frustum[frustum_top+0] + frustum[frustum_top+1] * frustum[frustum_top+1] + frustum[frustum_top+2] * frustum[frustum_top+2] );
	frustum[frustum_top+0] /= t;
	frustum[frustum_top+1] /= t;
	frustum[frustum_top+2] /= t;
	frustum[frustum_top+3] /= t;
	
	/* Extract the FAR plane */
	frustum[frustum_far+0] = clip[ 3] - clip[ 2];
	frustum[frustum_far+1] = clip[ 7] - clip[ 6];
	frustum[frustum_far+2] = clip[11] - clip[10];
	frustum[frustum_far+3] = clip[15] - clip[14];
	
	/* Normalize the result */
	t = Math.sqrt( frustum[frustum_far+0] * frustum[frustum_far+0] + frustum[frustum_far+1] * frustum[frustum_far+1] + frustum[frustum_far+2] * frustum[frustum_far+2] );
	frustum[frustum_far+0] /= t;
	frustum[frustum_far+1] /= t;
	frustum[frustum_far+2] /= t;
	frustum[frustum_far+3] /= t;
	
	/* Extract the NEAR plane */
	frustum[frustum_near+0] = clip[ 3] + clip[ 2];
	frustum[frustum_near+1] = clip[ 7] + clip[ 6];
	frustum[frustum_near+2] = clip[11] + clip[10];
	frustum[frustum_near+3] = clip[15] + clip[14];
	
	/* Normalize the result */
	t = Math.sqrt( frustum[frustum_near+0] * frustum[frustum_near+0] + frustum[frustum_near+1] * frustum[frustum_near+1] + frustum[frustum_near+2] * frustum[frustum_near+2] );
	frustum[frustum_near+0] /= t;
	frustum[frustum_near+1] /= t;
	frustum[frustum_near+2] /= t;
	frustum[frustum_near+3] /= t;
}