/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCCollisionTools.h
 * Description : Functionality for testing collision.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCCOLLISIONTOOLS_H__
#define __CCCOLLISIONTOOLS_H__


class CCSceneCollideable;
typedef struct CCOctree CCOctree;

struct CCCollisionManager
{
	CCCollisionManager(const float octreeSize);
	~CCCollisionManager();
	
	CCOctree *tree;
	float pruneTrees;
    
    CCSceneCollideable *objects[MAX_OBJECTS];
    int length;
};



enum CCCollisionFlags
{
    collision_none          = 0x000000001,  // No collision
    collision_box           = 0x000000002,  // Anything with a collision box
    collision_static        = 0x000000004,  // Non-moveable objects
    collision_moveable      = 0x000000008,  // Moveable objects
    collision_character     = 0x000000010,  // Characters
    collision_ui            = 0x000000020   // UI
};



// Update the bounding boxes when an object is flagged
extern void CCUpdateCollisions(CCSceneCollideable *collideable, const bool dependantOnFlags=true);

// Collision test bounding boxes
extern const bool CCBasicBoxCollisionCheck(const CCVector3 &sourceMin, const CCVector3 &sourceMax, 
                                           const CCVector3 &targetMin, const CCVector3 &targetMax);

extern CCSceneCollideable* CCBasicCollisionCheck(CCSceneCollideable *sourceObject, const CCVector3 *targetLocation);
extern CCSceneCollideable* CCBasicOctreeCollisionCheck(CCSceneCollideable *sourceObject, 
                                                       const CCVector3 *targetLocation,
                                                       const bool requestCollisionReport=false,
                                                       const CCCollisionFlags flags=collision_box);

extern CCSceneCollideable* CCOctreeCollisionCheck(const CCVector3 &min, const CCVector3 &max, 
                                                  const CCSceneCollideable *sourceObject=NULL, 
                                                  const uint flags=collision_box);
extern CCSceneCollideable* CCMovementOctreeCollisionCheck(CCSceneCollideable *sourceObject, CCVector3 currentPosition, const CCVector3 &targetPosition);

// Collision test a ray
extern CCSceneCollideable* CCBasicLineCollisionCheck(CCSceneCollideable **list, 
                                                     const int length,
                                                     CCSceneCollideable *sourceObject,
                                                     const CCVector3 &start,
                                                     const CCVector3 &end,
                                                     CCVector3 *hitLocation,
                                                     const bool collideInsideObjects,
                                                     const CCCollisionFlags flags=collision_box,
                                                     const bool stopAtAnyCollision = false);

extern CCSceneCollideable* CCBasicLineCollisionCheck(const CCVector3 &start, 
                                                     const CCVector3 &end,
                                                     const float width,
                                                     CCSceneCollideable *source=NULL);

extern const bool CCBasicLineCollisionCheck(CCSceneCollideable *checkingObject, 
                                            CCSceneCollideable *sourceObject, 
                                            const CCVector3 &start, 
                                            const CCVector3 &end);

// Frustum testing
extern const uint CCSphereInFrustum(float frustum[6][4], const float x, const float y, const float z, const float radius);
extern const uint CCCubeInFrustum(const float frustum[6][4], const CCVector3 min, const CCVector3 max);


#endif // __CCCOLLISIONTOOLS_H__
