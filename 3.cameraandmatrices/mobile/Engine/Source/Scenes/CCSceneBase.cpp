/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCSceneBase.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCSceneBase.h"
#include "CCObjects.h"


CCSceneBase::CCSceneBase()
{
	enabled = true;
    deleteMe = false;
	
    objects.allocate( MAX_OBJECTS );
	
    parentScene = NULL;
	lifetime = 0.0f;
}


void CCSceneBase::destruct()
{
    childScenes.deleteObjectsAndList();

    while( objects.length > 0 )
    {
        CCSceneObject *object = objects.list[0];
        DELETE_OBJECT( object );
    }
    ASSERT( collideables.length == 0 );
}


void CCSceneBase::deleteLater()
{
    deleteMe = true;
    deleteLinkedScenesLater();
}


void CCSceneBase::deleteLinkedScenesLater()
{
    for( int i=0; i<linkedScenes.length; ++i )
    {
        CCSceneBase *linkedScene = linkedScenes.list[i];
        linkedScene->unlinkScene( this );
        linkedScene->deleteLater();
    }
    linkedScenes.length = 0;
}


void CCSceneBase::update(const CCTime &gameTime)
{
    // Remove any redundant scenes
    for( int i=0; i<childScenes.length; ++i )
    {
        CCSceneBase *scene = childScenes.list[i];
        if( scene->shouldDelete() )
        {
            removeChildScene( scene );
            i--;
        }
    }

    if( enabled )
    {
        lifetime += gameTime.real;

        updateScene( gameTime );
        
        // Update our child scenes
        for( int i=0; i<childScenes.length; ++i )
        {
            CCSceneBase *scene = childScenes.list[i];
            scene->update( gameTime );
        }
        
        updateCamera( gameTime );
        for( int i=0; i<childScenes.length; ++i )
        {
            childScenes.list[i]->updateCamera( gameTime );
        }
    }
}


void CCSceneBase::updateScene(const CCTime &gameTime)
{
    for( int i=0; i<objects.length; ++i )
    {
        CCSceneObject *object = objects.list[i];
        if( object->isActive() )
        {
            object->update( gameTime );
        }
        else if( object->deleteMe > 0 )
        {
            if( --object->deleteMe == 0 )
            {
                DELETE_OBJECT( object );
                --i;
            }
        }
    }
}


void CCSceneBase::updateCamera(const CCTime &gameTime)
{
    
}


const bool CCSceneBase::render(const CCCameraBase *inCamera, const int pass, const bool alpha)
{
    bool rendered = false;
    if( inCamera->getIndex() == 0 )
	{
        renderObjects( pass, alpha );
        rendered = true;
	}

    for( int i=0; i<childScenes.length; ++i )
    {
        rendered |= childScenes.list[i]->render( inCamera, pass, alpha );
    }

    return rendered;
}


void CCSceneBase::renderObjects(const int pass, const bool alpha)
{
#if defined PROFILEON
    CCProfiler profile( "CCSceneBase::renderObjects()" );
#endif

    for( int i=0; i<objects.length; ++i )
    {
        CCSceneObject *object = objects.list[i];
        if( object->renderPass == pass )
        {
            if( object->isActive() && ( object->octreeRender == false ) )
            {
                object->render( alpha );
            }
        }
    }
}


void CCSceneBase::renderOctreeObject(CCSceneObject *object, const CCCameraBase *inCamera, const int pass, const bool alpha)
{
	if( inCamera->getIndex() == 0 )
	{
        object->render( alpha );
	}
}


void CCSceneBase::render2DBackground(const uint inCameraIndex)
{
    for( int i=0; i<childScenes.length; ++i )
    {
        childScenes.list[i]->render2DBackground( inCameraIndex );
    }
}


void CCSceneBase::render2DForeground(const uint inCameraIndex)
{
    for( int i=0; i<childScenes.length; ++i )
    {
        childScenes.list[i]->render2DForeground( inCameraIndex );
    }
}


void CCSceneBase::addObject(CCSceneObject *object)
{
    ASSERT( object->inScene == NULL );
    object->inScene = this;
    objects.add( object );
    ASSERT( objects.length < MAX_OBJECTS );
}


void CCSceneBase::removeObject(CCSceneObject* object)
{
    //DEBUGLOG( "removeObject - %@", [object description] );
	
    ASSERT( object->inScene == this );

    object->inScene = NULL;
    const bool removed = objects.remove( object );
    if( removed == false )
    {
        ASSERT( removed );
    }
}


void CCSceneBase::addCollideable(CCSceneCollideable *collideable)
{
    collideables.add( collideable );
	gEngine->addCollideable( collideable );
}


void CCSceneBase::removeCollideable(CCSceneCollideable *collideable)
{
    collideables.remove( collideable );
	gEngine->removeCollideable( collideable );
}


void CCSceneBase::setParent(CCSceneBase *inParent)
{
    ASSERT( parentScene == NULL );
    parentScene = inParent;
}


void CCSceneBase::addChildScene(CCSceneBase *inScene)
{
    inScene->setParent( this );
    childScenes.add( inScene );
    inScene->setup();
}


void CCSceneBase::removeChildScene(CCSceneBase *inScene)
{
    if( childScenes.remove( inScene ) )
    {
        DELETE_OBJECT( inScene );
    }
}


void CCSceneBase::linkScene(CCSceneBase *inScene)
{
    linkedScenes.add( inScene );
}


void CCSceneBase::unlinkScene(CCSceneBase *inScene)
{
    linkedScenes.remove( inScene );
}


void CCSceneBase::requestDownload(const char *url, const char *token)
{
    class Callback : public CCURLCallback
    {
    public:
        Callback(CCSceneBase *scene, const char *token)
        {
            this->scene = scene;
            this->token = token;
        }

        void run()
        {
            scene->dataDownloaded( reply, token );
        }

    private:
        CCSceneBase *scene;
        const char *token;
    };

    gEngine->urlManager->requestURL( url, new Callback( this, token ) );
}


void CCSceneBase::dataDownloaded(CCURLRequest *reply, const char *type)
{
    // e.g.
//    if( reply->state >= URLRequest::succeeded )
//    {
//        if( strcmp( type, "refresh" ) == 0 )
//        {
//            const char *data = reply->data.buffer;
//            if( data != NULL )
//            {
//            }
//        }
//    }
}
