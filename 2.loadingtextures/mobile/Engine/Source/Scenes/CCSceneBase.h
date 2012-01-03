/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCSceneBase.h
 * Description : Handles the drawing and updating of objects.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCSCENEBASE_H__
#define __CCSCENEBASE_H__


class CCSceneObject;

class CCSceneBase : public CCBaseType
{
public:
	CCSceneBase();
	virtual void destruct();
	
	virtual void setup() {};
	virtual void restart() {}
	
    void deleteLater();
    void deleteLinkedScenesLater();
    inline const bool shouldDelete() { return deleteMe; }
	
	void update(const CCTime &gameTime);
    
protected:
    virtual void updateScene(const CCTime &gameTime);
	
public:
    virtual const bool render(const int pass, const bool alpha);
    
protected:
    virtual void renderObjects(const int pass, const bool alpha);
    
public:
	// Add object to the scene and place in the created list
	void addObject(CCSceneObject *object);
	void removeObject(CCSceneObject* object);
    
    void addCollideable(CCSceneCollideable *collideable);
    void removeCollideable(CCSceneCollideable *collideable);
    
    void setParent(CCSceneBase *inParent);
    void addChildScene(CCSceneBase *inScene);
    void removeChildScene(CCSceneBase *inScene);
    void linkScene(CCSceneBase *inScene);
    void unlinkScene(CCSceneBase *inScene);
	
protected:
    // HTTP requests
    void requestDownload(const char *url, const char *token);
    virtual void dataDownloaded(CCURLRequest *reply, const char *token);

public:
    bool enabled;
    
protected:
    bool deleteMe;

protected:
    CCList<CCSceneObject> objects;
    CCList<CCSceneCollideable> collideables;

    CCSceneBase *parentScene;
    CCList<CCSceneBase> childScenes;
    CCList<CCSceneBase> linkedScenes;

    float lifetime;
};


#endif // __CCSCENEBASE_H__
