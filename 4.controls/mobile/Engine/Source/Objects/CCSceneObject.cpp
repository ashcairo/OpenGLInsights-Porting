/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCSceneObject.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCObjects.h"
#include "CCSceneBase.h"


CCSceneObject::CCSceneObject()
{
    enabled = true;
    inScene = NULL;
    deleteMe = 0;
	
	parent = NULL;
	
	model = NULL;

    renderPass = render_main;
    octreeRender = false;
    transparent = false;
    transparentParent = false;
}


void CCSceneObject::destruct()
{
    if( inScene != NULL )
    {
        removeFromScene();
    }
    else if( parent == NULL )
    {
        ASSERT( false );
    }
    else
    {
        parent->removeChild( this );
    }

    children.deleteObjectsAndList();
    updaters.deleteObjectsAndList();

    DELETE_OBJECT( model );

    super::destruct();
}


// RenerableBase
void CCSceneObject::dirtyWorldMatrix()
{
    super::dirtyWorldMatrix();

    for( int i=0; i<children.length; ++i )
    {
        children.list[i]->dirtyWorldMatrix();
    }
}


void CCSceneObject::setScene(CCSceneBase *scene)
{
    scene->addObject( this );
}


void CCSceneObject::removeFromScene()
{
	deactivate();
    inScene->removeObject( this );
}


void CCSceneObject::deleteLater()
{
    // Mark to delete in 2 frames
    deleteMe = 2;
    deactivate();
}


void CCSceneObject::addChild(CCSceneObject *object)
{
    children.add( object );

    object->parent = this;

    if( transparentParent == false )
    {
        if( object->transparentParent )
        {
            setTransparentParent( true );
        }
    }
}


const bool CCSceneObject::removeChild(CCSceneObject *object)
{
    if( children.remove( object ) )
    {
        if( children.length == 0 )
        {
            children.freeList();
        }

        return true;
    }

    return false;
}


void CCSceneObject::moveChildToScene(CCSceneObject *object, CCSceneBase *scene)
{
    object->translate( &position );
    object->setScene( scene );
    ASSERT( removeChild( object ) );
}


void CCSceneObject::addUpdater(CCUpdater *updater)
{
    updaters.add( updater );
}


void CCSceneObject::removeUpdater(CCUpdater *updater)
{
    updaters.remove( updater );
}


const bool CCSceneObject::shouldCollide(CCSceneCollideable *collideWith, const bool initialCall)
{
    // It's not me is it?
    if( this == collideWith )
    {
        return false;
    }

    // Ask my parent if we should collide
    if( parent != NULL )
    {
        return parent->shouldCollide( collideWith, initialCall );
    }

    // Yeah, let's collide baby
    return true;
}


void CCSceneObject::update(const CCTime &gameTime)
{
    for( int i=0; i<updaters.length; ++i )
    {
        updaters.list[i]->update( gameTime.delta );
    }

    for( int i=0; i<children.length; ++i )
    {
        children.list[i]->update( gameTime );
    }
}


void CCSceneObject::render(const bool alpha)
{	
    if( shouldRender )
    {
        if( alpha == false || transparentParent )
        {
            GLPushMatrix();
            {
                refreshModelMatrix();
                GLMultMatrixf( modelMatrix );

                for( int i=0; i<children.length; ++i )
                {
                    children.list[i]->render( alpha );
                }

                if( alpha == transparent )
                {
                    renderModels( alpha );
                }
            }
            GLPopMatrix();
        }
    }
}


void CCSceneObject::renderModels(const bool alpha)
{
    if( model )
    {
        model->render( alpha );
    }
}


void CCSceneObject::setTransparent(const bool toggle)
{
    transparent = toggle;
    setTransparentParent( toggle );
}


void CCSceneObject::setTransparentParent(const bool toggle)
{
    transparentParent = toggle;
    if( parent != NULL )
    {
        parent->setTransparentParent( toggle );
    }
}

