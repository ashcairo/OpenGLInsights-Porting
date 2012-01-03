/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCCollisionTools.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCObjects.h"


CCCollisionManager::CCCollisionManager(const float octreeSize)
{
    tree = new CCOctree( NULL, CCVector3(), octreeSize );
	pruneTrees = 0.0f;
	length = 0;	
}


CCCollisionManager::~CCCollisionManager()
{
	if( tree != NULL )
	{
		CCOctreeDeleteLeafs( tree );
        FREE_POINTER( tree );
	}
}


const bool CCBasicBoxCollisionCheck(const CCVector3 *sourceMin, const CCVector3 *sourceMax, const CCVector3 *targetMin, const CCVector3 *targetMax)
{
	if( sourceMax->z > targetMin->z && sourceMin->z < targetMax->z )
	{
		if( sourceMax->x > targetMin->x && sourceMin->x < targetMax->x )
		{
			if( sourceMax->y > targetMin->y && sourceMin->y < targetMax->y )
			{
				return true;
			}
		}
	}
	
	return false;
}


CCSceneCollideable* CCBasicCollisionCheck(CCSceneCollideable *sourceObject, const CCVector3 *targetLocation)
{
	CCCollisionManager *collideables = gEngine->collisionManager;
	
	if( HasFlag( sourceObject->collideableType, collision_box ) )
	{
		CCVector3 sourceMin = CCVector3( targetLocation->x - sourceObject->collisionBounds.x,
                                       targetLocation->y - sourceObject->collisionBounds.y,
                                       targetLocation->z - sourceObject->collisionBounds.z );
		
		CCVector3 sourceMax = CCVector3( targetLocation->x + sourceObject->collisionBounds.x,
                                       targetLocation->y + sourceObject->collisionBounds.y,
                                       targetLocation->z + sourceObject->collisionBounds.z );
		
		for( int i=0; i<collideables->length; ++i )
		{
            CCSceneCollideable *checkingObject = collideables->objects[i];
			
			if( checkingObject != sourceObject )
			{
				if( HasFlag( checkingObject->collideableType, collision_box ) )
				{
					CCVector3 *targetMin = &checkingObject->min;
					CCVector3 *targetMax = &checkingObject->max;
					
					if( CCBasicBoxCollisionCheck( &sourceMin, &sourceMax, targetMin, targetMax ) )
					{
						return checkingObject;
					}
				}
			}
		}
	}
	
	return NULL;
}


#define MAX_LEAFS 64
#define MAX_COLLIDEABLES 256
static CCSceneCollideable *collideables[MAX_COLLIDEABLES];
static int numberOfCollideables;
static void setCollideablesList(const CCVector3 *sourceMin, const CCVector3 *sourceMax)
{
	static const CCOctree *leafsList[MAX_LEAFS];
	static int numberOfLeafs;
	numberOfLeafs = 0;
	CCOctreeListLeafs( gEngine->collisionManager->tree, sourceMin, sourceMax, leafsList, &numberOfLeafs );
	ASSERT( numberOfLeafs < MAX_LEAFS );
    LOG_NEWMAX( "Max leafs per scan", maxLeafsPerScan, numberOfLeafs );
	
	numberOfCollideables = 0;
	CCOctreeListCollideables( collideables, &numberOfCollideables, leafsList, numberOfLeafs );
	ASSERT( numberOfCollideables < MAX_COLLIDEABLES );
    LOG_NEWMAX( "Max collideables per scan", maxCollideablesPerScan, numberOfCollideables );
}


CCSceneCollideable* CCBasicOctreeCollisionCheck(CCSceneCollideable *sourceObject, 
											 const CCVector3 *targetLocation, 
											 const bool requestCollisionReport,
											 const CCCollisionFlags flags)
{
	if( HasFlag( sourceObject->collideableType, collision_box ) && sourceObject->octrees.length > 0 )
	{
		CCVector3 sourceMin = CCVector3( targetLocation->x - sourceObject->collisionBounds.x,
                                       targetLocation->y - sourceObject->collisionBounds.y,
                                       targetLocation->z - sourceObject->collisionBounds.z );
		
		CCVector3 sourceMax = CCVector3( targetLocation->x + sourceObject->collisionBounds.x,
                                       targetLocation->y + sourceObject->collisionBounds.y,
                                       targetLocation->z + sourceObject->collisionBounds.z );
		
		setCollideablesList( &sourceMin, &sourceMax );
		
		CCSceneCollideable *closestCollision = NULL;
		
		for( int i=0; i<numberOfCollideables; ++i )
		{
			CCSceneCollideable *checkingObject = collideables[i];
			
			if( checkingObject != sourceObject )
			{
				if( HasFlag( checkingObject->collideableType, flags ) )
				{
					if( sourceObject->shouldCollide( checkingObject, true ) )
					{
						CCVector3 *targetMin = &checkingObject->min;
						CCVector3 *targetMax = &checkingObject->max;
						
						if( HasFlag( checkingObject->collideableType, collision_box ) )
						{
							if( CCBasicBoxCollisionCheck( &sourceMin, &sourceMax, targetMin, targetMax ) )
							{
								closestCollision = checkingObject;
								if( requestCollisionReport && closestCollision != NULL )
								{
									closestCollision = sourceObject->requestCollisionReport( closestCollision );
								}
								if( closestCollision != NULL )
								{
									break;
								}
							}
						}
					}
				}
			}
		}
		
		return closestCollision;
	}
	
	return NULL;
}


CCSceneCollideable* CCOctreeCollisionCheck(const CCVector3 *min, const CCVector3 *max, const CCSceneCollideable *sourceObject, const uint flags)
{	
	setCollideablesList( min, max );
	
	CCSceneCollideable *closestCollision = NULL;
	for( int i=0; i<numberOfCollideables; ++i )
	{
		CCSceneCollideable *checkingObject = collideables[i];
	
		if( checkingObject != sourceObject )
		{
			CCVector3 *targetMin = &checkingObject->min;
			CCVector3 *targetMax = &checkingObject->max;
			
			if( HasFlag( checkingObject->collideableType, collision_box ) )
			{
				if( flags == collision_box || HasFlag( checkingObject->collideableType, flags ) )
				{
					if( CCBasicBoxCollisionCheck( min, max, targetMin, targetMax ) )
					{
						closestCollision = checkingObject;
						break;
					}
				}
			}
		}
	}
	
	return closestCollision;
}


CCSceneCollideable* CCMovementOctreeCollisionCheck(CCSceneCollideable *sourceObject, CCVector3 currentPosition, const CCVector3 *targetPosition)
{
	CCSceneCollideable *collidedWith = NULL;
	
	const float velocityX = targetPosition->x - currentPosition.x;
	const float velocityZ = targetPosition->z - currentPosition.z;
	
	const float velocityVsBoundingX = velocityX * sourceObject->inverseBoundsLength.x;
	const float velocityVsBoundingZ = velocityZ * sourceObject->inverseBoundsLength.x;
	const float absVelocityVsBoundingX = fabsf( velocityVsBoundingX );
	const float absVelocityVsBoundingZ = fabsf( velocityVsBoundingZ );
	if( absVelocityVsBoundingX > 1.0f || absVelocityVsBoundingZ > 1.0f )
	{
		const float furthestIncrement = absVelocityVsBoundingX > absVelocityVsBoundingZ ? absVelocityVsBoundingX : absVelocityVsBoundingZ;
		const uint numberOfIncrements = roundf( furthestIncrement + 0.5f );
		
		const float inverseNumberOfIncrements = 1.0f / numberOfIncrements;
		const float incrementsX = velocityX * inverseNumberOfIncrements;
		const float incrementsZ = velocityZ * inverseNumberOfIncrements;
		uint i = 0;
		do
		{
			currentPosition.x += incrementsX;
			currentPosition.z += incrementsZ;
			collidedWith = CCBasicOctreeCollisionCheck( sourceObject, &currentPosition, false, collision_static );
			i++;
		} while( i < numberOfIncrements && collidedWith == NULL );
	}
	else
	{
		currentPosition.x += velocityX;
		currentPosition.z += velocityZ;
		collidedWith = CCBasicOctreeCollisionCheck( sourceObject, &currentPosition, false, collision_static );
	}
	
	return collidedWith;
}


static bool lineCheckGetIntersection(const float dist1, const float dist2, const CCVector3 &point1, const CCVector3 &point2, CCVector3 &hitLocation) 
{
	if( ( dist1 * dist2 ) >= 0.0f )
	{
		return false;
	}
	
	if( dist1 == dist2 )
	{ 
		return false;
	}
	
    // point1 + ( point2 - point1 ) * ( -dst1 / ( dst2 - dst1 ) );
    hitLocation = point2;
    hitLocation.sub( point1 );
    hitLocation.mul( -dist1 / ( dist2 - dist1 ) );
    hitLocation.add( point1 );
	
	return true;
}


static bool lineCheckInBox(const CCVector3 &hit, const CCVector3 &boxMin, const CCVector3 &boxMax, const int axis) 
{
	if( axis == 1 && hit.z >= boxMin.z && hit.z < boxMax.z && hit.y >= boxMin.y && hit.y < boxMax.y )
	{
		return true;
	}
	
	if( axis == 2 && hit.z >= boxMin.z && hit.z < boxMax.z && hit.x > boxMin.x && hit.x < boxMax.x )
	{
		return true;
	}
	
	if( axis == 3 && hit.x >= boxMin.x && hit.x < boxMax.x && hit.y >= boxMin.y && hit.y < boxMax.y )
	{
		return true;
	}
	
	return false;
}


// returns true if line (start, end) intersects with the box (boxMin, boxMax)
// returns intersection point in hitLocation
static bool lineCheckBox(const CCVector3 &start, const CCVector3 &end, 
                         const CCVector3 &boxMin, const CCVector3 &boxMax, 
                         CCVector3 &hitLocation, float &hitDistance)
{
	if( end.x < boxMin.x && start.x < boxMin.x )
	{
		return false;
	}
	
	if( end.x > boxMax.x && start.x > boxMax.x )
	{
		return false;
	}
	
	if( end.y < boxMin.y && start.y < boxMin.y )
	{
		return false;
	}
	
	if( end.y > boxMax.y && start.y > boxMax.y )
	{
		return false;
	}
	
	if( end.z < boxMin.z && start.z < boxMin.z )
	{
		return false;
	}
	
	if( end.z > boxMax.z && start.z > boxMax.z )
	{
		return false;
	}
	
	if( start.x > boxMin.x && start.x < boxMax.x &&
	    start.y > boxMin.y && start.y < boxMax.y &&
	    start.z > boxMin.z && start.z < boxMax.z ) 
	{
		hitLocation = start;
		return true;
	}
	
	static CCVector3 hits[6];
	static bool hitResult[6];
	
	hitResult[0] = lineCheckGetIntersection( start.x-boxMin.x, end.x-boxMin.x, start, end, hits[0] ) && lineCheckInBox( hits[0], boxMin, boxMax, 1 ); 
	hitResult[1] = lineCheckGetIntersection( start.y-boxMin.y, end.y-boxMin.y, start, end, hits[1] ) && lineCheckInBox( hits[1], boxMin, boxMax, 2 ); 
	hitResult[2] = lineCheckGetIntersection( start.z-boxMin.z, end.z-boxMin.z, start, end, hits[2] ) && lineCheckInBox( hits[2], boxMin, boxMax, 3 );  
	hitResult[3] = lineCheckGetIntersection( start.x-boxMax.x, end.x-boxMax.x, start, end, hits[3] ) && lineCheckInBox( hits[3], boxMin, boxMax, 1 );  
	hitResult[4] = lineCheckGetIntersection( start.y-boxMax.y, end.y-boxMax.y, start, end, hits[4] ) && lineCheckInBox( hits[4], boxMin, boxMax, 2 ); 
	hitResult[5] = lineCheckGetIntersection( start.z-boxMax.z, end.z-boxMax.z, start, end, hits[5] ) && lineCheckInBox( hits[5], boxMin, boxMax, 3 );
	
    static bool hit;
    static CCVector3 distance;
    hit = false;
    hitDistance = MAXFLOAT;
    for( uint i=0; i<6; ++i )
    {
        if( hitResult[i] )
        {
            distance = CCVector3SubResult( start, hits[i] );
            float currentHitDistance = CCVector3LengthSquared( distance );
            if( currentHitDistance < hitDistance )
            {
                hitDistance = currentHitDistance;
                hitLocation = hits[i];
                hit = true;
            }
        }
    }

	return hit;
}


CCSceneCollideable* CCBasicLineCollisionCheck(CCSceneCollideable **list, 
                                              const int length,
                                              CCSceneCollideable *sourceObject,
                                              const CCVector3 &start,
                                              const CCVector3 &end,
                                              CCVector3 *hitLocation,
                                              const bool collideInsideObjects,
                                              const CCCollisionFlags flags,
                                              const bool stopAtAnyCollision)
{	
	float hitDistance = MAXFLOAT;
	CCSceneCollideable *hitObject = NULL;
	static CCVector3 currentHitLocation;
    static float currentHitDistance;
	
    for( int i=0; i<length; ++i )
	{
		CCSceneCollideable *checkingObject = list[i];
		
		if( checkingObject != sourceObject )
		{
            if( checkingObject->enabled && checkingObject->isActive() && HasFlag( checkingObject->collideableType, flags ) )
            {
                const CCVector3 &targetMin = checkingObject->min;
                const CCVector3 &targetMax = checkingObject->max;

                if( lineCheckBox( start, end, targetMin, targetMax, currentHitLocation, currentHitDistance ) )
                {
                    if( currentHitDistance < hitDistance && ( collideInsideObjects || currentHitDistance > 0.0f ) )
                    {
                        hitDistance = currentHitDistance;
                        hitObject = checkingObject;
                        if( hitLocation != NULL )
                        {
                            *hitLocation = currentHitLocation;
                        }

                        if( stopAtAnyCollision )
                        {
                            return hitObject;
                        }
                    }
                }
			}
		}
	}
	
	// Finally test our source object if we have nothing else
	if( hitObject == NULL && sourceObject != NULL )
	{
		CCSceneCollideable *checkingObject = sourceObject;
		const CCVector3 &targetMin = checkingObject->min;
		const CCVector3 &targetMax = checkingObject->max;
		
		if( lineCheckBox( start, end, targetMin, targetMax, currentHitLocation, currentHitDistance ) )
		{
			if( currentHitDistance < hitDistance && ( collideInsideObjects || currentHitDistance > 0.0f ) )
			{
				hitDistance = currentHitDistance;
				hitObject = checkingObject;
				*hitLocation = currentHitLocation;
			}
		}
	}
	
	return hitObject;
}


CCSceneCollideable* CCBasicLineCollisionCheck(const CCVector3 &start, 
                                              const CCVector3 &end,
                                              const float width,
                                              CCSceneCollideable *source)
{	
    static CCVector3 hitLocation;
    CCSceneCollideable *hitObject = CCBasicLineCollisionCheck( gEngine->collisionManager->objects,
                                                               gEngine->collisionManager->length,
                                                               source,
                                                               start, end,
                                                               &hitLocation,
                                                               true,
                                                               collision_static,
                                                               true );
    if( hitObject == NULL )
    {
        static CCVector3 minStart, minEnd, maxStart, maxEnd;
        minStart.set( start.x - width, start.y, start.z - width );
        minEnd.set( end.x - width, start.y, end.z - width );
        maxStart.set( start.x + width, start.y, start.z + width );
        maxEnd.set( end.x - width, start.y, end.z - width );

        hitObject = CCBasicLineCollisionCheck( gEngine->collisionManager->objects,
                                               gEngine->collisionManager->length,
                                               NULL,
                                               minStart, maxEnd,
                                               &hitLocation,
                                               true,
                                               collision_static,
                                               true );

        if( hitObject == NULL )
        {
            hitObject = CCBasicLineCollisionCheck( gEngine->collisionManager->objects,
                                                   gEngine->collisionManager->length,
                                                   NULL,
                                                   maxStart, minEnd,
                                                   &hitLocation,
                                                   true,
                                                   collision_static,
                                                   true );
        }
    }

    return hitObject;
}


const bool CCBasicLineCollisionCheck(CCSceneCollideable *checkingObject, CCSceneCollideable *sourceObject, 
                                     const CCVector3 &start, const CCVector3 &end)
{
    const CCVector3 &targetMin = checkingObject->min;
    const CCVector3 &targetMax = checkingObject->max;

    static CCVector3 currentHitLocation;
    static float currentHitDistance;
    if( lineCheckBox( start, end, targetMin, targetMax, currentHitLocation, currentHitDistance ) )
    {
        return true;
    }

    return false;
}


const uint CCSphereInFrustum(float frustum[6][4], const float x, const float y, const float z, const float radius)
{
	int c = 0;
	float d;
	
	for( uint i=0; i<6; ++i )
	{
		d = frustum[i][0] * x + frustum[i][1] * y + frustum[i][2] * z + frustum[i][3];
		if( d <= -radius )
		{
			return 0;
		}
		
		if( d > radius )
		{
			++c;
		}
	}
	
	return (c == 6) ? 2 : 1;
}


const uint CCCubeInFrustum(const float frustum[6][4], const CCVector3 min, const CCVector3 max)
{
	int c;
	int c2 = 0;
	
	for( uint i=0; i<6; ++i )
	{
		const float minX = frustum[i][0] * min.x;
		const float minY = frustum[i][1] * min.y;
		const float minZ = frustum[i][2] * min.z;
		
		const float maxX = frustum[i][0] * max.x;
		const float maxY = frustum[i][1] * max.y;
		const float maxZ = frustum[i][2] * max.z;
		
		c = 0;
		if( minX + minY + minZ + frustum[i][3] > 0 )
		{
			c++;
		}
		if( maxX + minY + minZ + frustum[i][3] > 0 )
		{
			c++;
		}
		if( minX + maxY + minZ + frustum[i][3] > 0 )
		{
			c++;
		}
		if( maxX + maxY + minZ + frustum[i][3] > 0 )
		{
			c++;
		}
		if( minX + minY + maxZ + frustum[i][3] > 0 )
		{
			c++;
		}
		if( maxX + minY + maxZ + frustum[i][3] > 0 )
		{
			c++;
		}
		if( minX + maxY + maxZ + frustum[i][3] > 0 )
		{
			c++;
		}
		if( maxX + maxY + maxZ + frustum[i][3] > 0 )
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
