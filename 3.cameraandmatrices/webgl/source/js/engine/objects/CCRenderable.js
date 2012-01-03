/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCRenderable.js
 * Description : A renderable component.
 *
 * Created     : 23/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCRenderable()
{
	this.construct();
}


CCRenderable.prototype.construct = function()
{
    this.modelMatrix = mat4.create();
    this.position = vec3.create();
    this.rotation = vec3.create();
    this.scale = vec3.create( [1,1,1] );
	
	this.shouldRender = true;
    this.dirtyModelMatrix();
}


CCRenderable.prototype.destruct = function()
{
}


CCRenderable.prototype.dirtyModelMatrix = function()
{
	this.updateModelMatrix = true;
}


CCRenderable.prototype.refreshModelMatrix = function()
{
	if( this.updateModelMatrix )
    {        
        var modelMatrix = this.modelMatrix;
        mat4.identity( modelMatrix );
        mat4.translate( modelMatrix, this.position );

        mat4.scale( modelMatrix, this.scale );

        var rotation = this.rotation;
        mat4.rotate( modelMatrix, rotation[0], [1.0, 0.0, 0.0] );
        mat4.rotate( modelMatrix, rotation[1], [0.0, 1.0, 0.0] );
        mat4.rotate( modelMatrix, rotation[2], [0.0, 0.0, 1.0] );
		
        this.updateModelMatrix = false;
    }
}


CCRenderable.prototype.setPosition = function(vector)
{
    this.setPositionXYZ( vector[0], vector[1], vector[2] );
}


CCRenderable.prototype.setPositionXYZ = function(x, y, z)
{
    var position = this.position;
    position[0] = x;
    position[1] = y;
    position[2] = z;
    this.updateModelMatrix = true;
    this.updateCollisions = true;
    this.refreshCollisions();
}


CCRenderable.prototype.setPositionX = function(x)
{
	this.setPositionXYZ( x, this.position[1], this.position[2] );
}


CCRenderable.prototype.translate = function(x, y, z)
{
	var position = this.position;
    position[0] += x;
	position[1] += y;
	position[2] += z;
    this.updateModelMatrix = true;
    this.updateCollisions = true;
    this.refreshCollisions();
}


CCRenderable.prototype.refreshCollisions = function()
{
}
