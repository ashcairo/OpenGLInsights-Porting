/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCTile3D.js
 * Description : A dynamic button widget.
 *
 * Created     : 21/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCTile3D()
{
	this.construct();
}
CopyPrototype( CCTile3D, CCRenderable, "CCRenderable" );


CCTile3D.prototype.construct = function()
{
	this.CCRenderable_construct();
	
    this.enabled = true;
	this.deleteMe = 0;
    this.parent = false;
	this.model = false;
    this.renderPass = 1;
	
	this.collideable = true;
    this.min = vec3.create();
    this.max = vec3.create();
    
    this.drawOrder = 100;
	
    this.touching = false;
	this.touchReleased = false;
    
    this.onPress = new Array();
    this.onRelease = new Array();
}


CCTile3D.prototype.destruct = function()
{
	this.CCRenderable_destruct();
}


// CCRenderable
CCTile3D.prototype.setScene = function(scene)
{
    scene.addObject( this );
}


CCTile3D.prototype.isActive = function()
{
	return this.deleteMe == 0;
}


CCTile3D.prototype.render = function(renderer, alpha)
{
    if( this.shouldRender )
    {
        if( alpha == false || this.transparentParent )
        {
            GLPushMatrix();
            {
                this.refreshModelMatrix();
                GLMultMatrix( this.modelMatrix );
                
                if( alpha == this.transparent )
                {
                    this.model.render( renderer, alpha );
                }
            }
            GLPopMatrix();
        }
    }
}


CCTile3D.prototype.isTransparent = function() 
{
    return this.transparent; 
}


CCTile3D.prototype.setTransparent = function(toggle)
{
    this.transparent = toggle;
    this.setTransparentParent( toggle );
}


CCTile3D.prototype.setTransparentParent = function(toggle)
{
    this.transparentParent = toggle;
    if( this.parent )
    {
        this.parent.setTransparentParent( toggle );
    }
}


CCTile3D.prototype.setHCollisionBounds = function(hWidth, hHeight, hDepth)
{
    this.collisionBounds = vec3.create( [hWidth, hHeight, hDepth] );
    
    var collisionBoundsLength = this.collisionBoundsLength = new CCPoint();
    collisionBoundsLength.x = hWidth > hDepth ? hWidth : hDepth;
    collisionBoundsLength.x *= 2.0;
    collisionBoundsLength.y = hHeight * 2.0;
    
    this.inverseBoundsLength = new CCPoint();
	this.inverseBoundsLength.x = 1.0 / collisionBoundsLength.x;
	this.inverseBoundsLength.y = 1.0 / collisionBoundsLength.y;
    
    this.updateCollisions = true;
}


CCTile3D.prototype.refreshCollisions = function()
{
    if( this.updateCollisions )
    {
        this.updateCollisions = false;

        var position = this.position;
        var bounds = this.collisionBounds;
        var min = this.min;
        var max = this.max;

        min[0] = position[0] - bounds[0];
        min[1] = position[1] - bounds[1];
        min[2] = position[2] - bounds[2];
        
        max[0] = position[0] + bounds[0];
        max[1] = position[1] + bounds[1];
        max[2] = position[2] + bounds[2];
    }
}


CCTile3D.prototype.onTouchPress = function()
{
    if( this.onPress )
    {
        var onPress = this.onPress;
        var length = onPress.length;
        for( var i=0; i<length; ++i )
        {
            onPress[i].run();
        }
    }
}


CCTile3D.prototype.onTouchMove = function()
{
}


CCTile3D.prototype.onTouchRelease = function()
{
    if( this.onRelease )
    {
        var onRelease = this.onRelease;
        var length = onRelease.length;
        for( var i=0; i<length; ++i )
        {
            onRelease[i].run();
        }
    }
}


CCTile3D.prototype.onTouchLoss = function()
{
}


CCTile3D.prototype.positionTileBelow = function(fromTile)
{
    this.setPosition( fromTile.position );
    this.translate( 0.0, -( fromTile.collisionBounds[1] + this.collisionBounds[1] ), 0.0 );
}