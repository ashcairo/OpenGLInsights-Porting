/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCSceneObject.h
 * Description : A scene managed object.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

class CCSceneObject : public CCRenderable
{
public:
	typedef CCRenderable super;
	
    CCSceneObject();
    virtual void destruct();

    // CCRenderable
    virtual void dirtyWorldMatrix();

    virtual void setScene(CCSceneBase *scene);
    virtual void removeFromScene();

    // Delete the object in 2 frames.
    void deleteLater();

    inline const bool isActive() { return deleteMe == 0; }
    virtual void deactivate() {};

    void addChild(CCSceneObject *object);
    const bool removeChild(CCSceneObject *object);

    // Remove an object from our child list and add it into the scene
    void moveChildToScene(CCSceneObject *object, CCSceneBase *scene);

    void addUpdater(CCUpdater *updater);
    void removeUpdater(CCUpdater *updater);

    virtual const bool shouldCollide(CCSceneCollideable *collideWith, const bool initialCall);

    virtual void update(const CCTime &gameTime);
    virtual void render(const bool alpha);
    virtual void renderModels(const bool alpha);

    const bool isTransparent() { return transparent; }
    void setTransparent(const bool toggle=true);
    void setTransparentParent(const bool toggle=true);

    bool enabled;
    CCSceneBase *inScene;
    uint deleteMe;

    CCSceneObject *parent;

    CCModelBase *model;

    CCRenderPass renderPass;
    bool octreeRender;

protected:
    bool transparent, transparentParent;

    // If I have children, they will be offset from my position
    CCDestructList<CCSceneObject> children;

    CCDestructList<CCUpdater> updaters;
};