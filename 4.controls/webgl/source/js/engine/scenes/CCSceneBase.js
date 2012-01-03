/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCSceneBase.js
 * Description : Handles the drawing and updating of objects.
 *
 * Created     : 03/08/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCSceneBase()
{
    this.construct();
}

CCSceneBase.prototype.construct = function()
{
	this.enabled = true;
	this.deleteMe = false;
	
    this.objects = new Array();
	
	this.parentScene = false;
	this.childScenes = new Array();
	this.linkedScenes = new Array();
	
	this.lifetime = 0.0;
}


CCSceneBase.prototype.resize = function()
{   
	
}


CCSceneBase.prototype.destruct = function()
{
	{
		var childScenes = this.childScenes;
		var length = childScenes.length;
		for( var i=0; i<length; ++i )
		{
			childScenes[i].destruct();
		}
		childScenes.length = 0;
	}
	
	//var objects = this.objects;
	//while( objects.length > 0 )
	//{
	//	objects[0].destruct();
	//}
}


CCSceneBase.prototype.deleteLater = function()
{
	this.deleteMe = true;
	this.deleteLinkedScenesLater();
}


CCSceneBase.prototype.deleteLinkedScenesLater = function()
{
	var linkedScenes = this.linkedScenes;
    var length = linkedScenes.length;
	for( var i=0; i<length; ++i )
	{
		var linkedScene = linkedScenes[i];
		linkedScene.unlinkScene( this );
		linkedScene.deleteLater();
	}
	linkedScenes.length = 0;
}


CCSceneBase.prototype.handleControls = function(ui, state, touchOverTable)
{
	{
		var childScenes = this.childScenes;
		var length = childScenes.length;
		for( var i=0; i<length; ++i )
		{
			childScenes[i].handleControls( ui, state, touchOverTable );
		}
	}
    return false;
}


CCSceneBase.prototype.update = function(delta)
{
	var childScenes = this.childScenes;
	var childScenesLength = childScenes.length;
	for( var i=0; i<childScenesLength; ++i )
	{
		var scene = childScenes[i];
		if( scene.shouldDelete() )
		{
			this.removeChildScene( scene );
			--i;
		}
	}

    if( this.enabled )
	{
		this.lifetime += delta;
		
		this.updateScene( delta );
		for( var i=0; i<childScenesLength; ++i )
		{
			childScenes[i].update( delta );
		}
		
		this.updateCamera( delta );
		for( var i=0; i<childScenesLength; ++i )
		{
			childScenes[i].update( delta );
		}
	}
}


CCSceneBase.prototype.updateScene = function(delta)
{
	var objects = this.objects;
    var length = objects.length;
    for( var i=0; i<length; ++i )
    {
		var object = objects[i];
		if( object.isActive() )
		{
			object.update( delta );
		}
		else if( object.deleteMe > 0 )
		{
			if( --object.deleteMe == 0 )
			{
				object.destruct();
				--i;
			}
		}
    }
}


CCSceneBase.prototype.updateCamera = function(delta)
{
}


CCSceneBase.prototype.render = function(renderer, camera, pass, alpha)
{
	var frustum = camera.frustum;
	
    var objects = this.objects;
    var length = objects.length;
    for( var i=0; i<length; ++i )
    {
        var object = objects[i];
        if( object.renderPass == pass )
        {
			if( this.CCCubeInFrustum( frustum, object.min, object.max ) )
			{
				object.render( renderer, alpha );
			}
        }
    }
	
	{
		var childScenes = this.childScenes;
		var length = childScenes.length;
		for( var i=0; i<length; ++i )
		{
			childScenes[i].render( renderer, alpha );
		}
	}
}


CCSceneBase.prototype.addObject = function(object)
{
	object.inScene = this;
	this.objects.push( object );
}


CCSceneBase.prototype.lineCheckGetIntersection = function(dist1, dist2, vec3Point1, vec3Point2, vec3HitLocation) 
{
	if( ( dist1 * dist2 ) >= 0.0 )
	{
		return false;
	}
	
	if( dist1 == dist2 )
	{ 
		return false;
	}
	
    // point1 + ( point2 - point1 ) * ( -dst1 / ( dst2 - dst1 ) );
    vec3.set( vec3Point2, vec3HitLocation );
    vec3.subtract( vec3HitLocation, vec3Point1 );
	vec3.scale( vec3HitLocation, -dist1 / ( dist2 - dist1 ) );
    vec3.add( vec3HitLocation, vec3Point1 );
	
	return true;
}

CCSceneBase.prototype.lineCheckInBox = function(hit, boxMin, boxMax, axis) 
{
	if( axis == 1 && hit[2] >= boxMin[2] && hit[2] < boxMax[2] && hit[1] >= boxMin[1] && hit[1] < boxMax[1] )
	{
		return true;
	}
	
	if( axis == 2 && hit[2] >= boxMin[2] && hit[2] < boxMax[2] && hit[0] > boxMin[0] && hit[0] < boxMax[0] )
	{
		return true;
	}
	
	if( axis == 3 && hit[0] >= boxMin[0] && hit[0] < boxMax[0] && hit[1] >= boxMin[1] && hit[1] < boxMax[1] )
	{
		return true;
	}
	
	return false;
}

// returns true if line (start, end) intersects with the box (boxMin, boxMax)
// returns intersection point in hitLocation
CCSceneBase.prototype.lineCheckBox = function(start, end, boxMin, boxMax, hitLocation)
{
	if( end[0] < boxMin[0] && start[0] < boxMin[0] )
	{
		return false;
	}
	
	if( end[0] > boxMax[0] && start[0] > boxMax[0] )
	{
		return false;
	}
	
	if( end[1] < boxMin[1] && start[1] < boxMin[1] )
	{
		return false;
	}
	
	if( end[1] > boxMax[1] && start[1] > boxMax[1] )
	{
		return false;
	}
	
	if( end[2] < boxMin[2] && start[2] < boxMin[2] )
	{
		return false;
	}
	
	if( end[2] > boxMax[2] && start[2] > boxMax[2] )
	{
		return false;
	}
	
	if( start[0] > boxMin[0] && start[0] < boxMax[0] &&
	    start[1] > boxMin[1] && start[1] < boxMax[1] &&
	    start[2] > boxMin[2] && start[2] < boxMax[2] ) 
	{
		vec3.set( start, hitLocation );
		return true;
	}
	
	var hits = new Array( 6 );
	for( var i=0; i<6; ++i )
	{
		hits[i] = vec3.create();
	}
	var hitResult = new Array( 6 );
	
	var lineCheckGetIntersection = this.lineCheckGetIntersection;
    var lineCheckInBox = this.lineCheckInBox;
	hitResult[0] = lineCheckGetIntersection( start[0]-boxMin[0], end[0]-boxMin[0], start, end, hits[0] ) && lineCheckInBox( hits[0], boxMin, boxMax, 1 ); 
	hitResult[1] = lineCheckGetIntersection( start[1]-boxMin[1], end[1]-boxMin[1], start, end, hits[1] ) && lineCheckInBox( hits[1], boxMin, boxMax, 2 ); 
	hitResult[2] = lineCheckGetIntersection( start[2]-boxMin[2], end[2]-boxMin[2], start, end, hits[2] ) && lineCheckInBox( hits[2], boxMin, boxMax, 3 );  
	hitResult[3] = lineCheckGetIntersection( start[0]-boxMax[0], end[0]-boxMax[0], start, end, hits[3] ) && lineCheckInBox( hits[3], boxMin, boxMax, 1 );  
	hitResult[4] = lineCheckGetIntersection( start[1]-boxMax[1], end[1]-boxMax[1], start, end, hits[4] ) && lineCheckInBox( hits[4], boxMin, boxMax, 2 ); 
	hitResult[5] = lineCheckGetIntersection( start[2]-boxMax[2], end[2]-boxMax[2], start, end, hits[5] ) && lineCheckInBox( hits[5], boxMin, boxMax, 3 );
	
    var distance = vec3.create();
    var hitDistance = false;
    for( var i=0; i<6; ++i )
    {
        if( hitResult[i] )
        {
			vec3.subtract( start, hits[i], distance );
            var currentHitDistance = vec3.lengthSquared( distance );
            if( hitDistance == false || currentHitDistance < hitDistance )
            {
                hitDistance = currentHitDistance;
				vec3.set( hits[i], hitLocation );
                hit = true;
            }
        }
    }

	return hitDistance;
}

CCSceneBase.prototype.basicLineCollisionCheck = function(list, length,
														 start, end,
														 hitPosition,
														 collideInsideObjects)
{	
	var hitDistance = CC_MAXFLOAT;
	var hitObject = false;

	var checkingHitPosition = vec3.create();
    for( var i=0; i<length; ++i )
	{
		var checkingHitObject = list[i];
		if( checkingHitObject.collideable != false )
		{
			checkingHitObject.refreshCollisions();

			var checkingHitDistance = this.lineCheckBox( start, end, checkingHitObject.min, checkingHitObject.max, checkingHitPosition );
			if( checkingHitDistance != false )
			{
				if( checkingHitDistance < hitDistance && ( collideInsideObjects || checkingHitDistance > 0.0 ) )
				{
					hitDistance = checkingHitDistance;
					hitObject = checkingHitObject;
					if( hitPosition != false )
					{
						vec3.set( checkingHitPosition, hitPosition );
					}
				}
			}
		}
	}
	
	return hitObject;
}


CCSceneBase.prototype.CCCubeInFrustum = function(frustum, min, max)
{
	var c;
	var c2 = 0;
	
	var end = 6*4;
	for( var i=0; i<end; i+=4 )
	{
		var minX = frustum[i+0] * min[0];
		var minY = frustum[i+1] * min[1];
		var minZ = frustum[i+2] * min[2];
		
		var maxX = frustum[i+0] * max[0];
		var maxY = frustum[i+1] * max[1];
		var maxZ = frustum[i+2] * max[2];
		
		c = 0;
		if( minX + minY + minZ + frustum[i+3] > 0 )
		{
			c++;
		}
		if( maxX + minY + minZ + frustum[i+3] > 0 )
		{
			c++;
		}
		if( minX + maxY + minZ + frustum[i+3] > 0 )
		{
			c++;
		}
		if( maxX + maxY + minZ + frustum[i+3] > 0 )
		{
			c++;
		}
		if( minX + minY + maxZ + frustum[i+3] > 0 )
		{
			c++;
		}
		if( maxX + minY + maxZ + frustum[i+3] > 0 )
		{
			c++;
		}
		if( minX + maxY + maxZ + frustum[i+3] > 0 )
		{
			c++;
		}
		if( maxX + maxY + maxZ + frustum[i+3] > 0 )
		{
			c++;
		}
		
		if( c == 0 )
		{
			return 0;
		}
		else if( c == 8 )
		{
			c2++;
		}
	}
	return (c2 == 6) ? 2 : 1;
}