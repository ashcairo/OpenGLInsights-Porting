/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCSceneCollideable.h
 * Description : A scene managed collideable object.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

class CCSceneCollideable : public CCSceneObject
{
public:
	typedef CCSceneObject super;

	CCSceneCollideable();
	virtual void destruct();
	
	// CCRenderable
	virtual void setPositionXYZ(const float x, const float y, const float z);
	virtual void translate(const float x, const float y, const float z);
	
	// CCSceneObject
	virtual void setScene(CCSceneBase *scene);
	virtual void removeFromScene();
	virtual void deactivate();
	virtual const bool shouldCollide(CCSceneCollideable *collideWith, const bool initialCall);
    virtual void renderModels(const bool alpha);
	
	void renderCollisionBox();
	
	void setHSquareCollisionBounds(const float hSize);
	void setHSquareCollisionBounds(const float hWidth, const float hHeight);
	void setHCollisionBounds(const float hWidth, const float hHeight, const float hDepth);
	
	// Ask to report a collision to the collidedWith object
	virtual CCSceneCollideable* requestCollisionReport(CCSceneCollideable *collidedWith);
	
	// Ask the collidedWith object if we've collided
	virtual CCSceneCollideable* reportCollision(CCSceneCollideable *collidedWith, const float x, const float y, const float z);
	
	virtual const bool reportAttack(CCSceneObject *attackedBy, const float force, const float x, const float y, const float z);

    virtual const bool isMoveable() { return false; }

	virtual void ownObject(CCSceneCollideable *object);
	virtual void unOwnObject(CCSceneCollideable *object);

protected:
	void setOwner(CCSceneCollideable *newOwner);
	void removeOwner(CCSceneCollideable *currentOwner);

public:
	uint collideableType;
	CCVector3 collisionBounds;
    CCPoint collisionBoundsLength;
    CCPoint inverseBoundsLength;

	CCVector3 min, max;
	bool updateCollisions;

	CCList<CCOctree> octrees;

	uint drawOrder;
    bool visible;

protected:
	// Since parenting disables collisions, the owner system leaves our owned objects alone, but informs us when an event occurs
	CCSceneCollideable *owner;
    CCList<CCSceneCollideable> owns;
};
