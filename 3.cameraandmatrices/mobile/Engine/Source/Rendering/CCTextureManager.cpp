/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTextureManager.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCTexturePNG.h"
#include "CCTextureFontPageFile.h"


CCTextureHandle::~CCTextureHandle()
{
    onLoad.deleteObjectsAndList();
    delete texture;
}



CCTextureManager::CCTextureManager()
{
	currentTextureHandle = 0;
    totalTexturesLoaded = 0;
    totalTextureSpace = 0;
    totalTexturesLoadedThisFrame = 0;
	
	CCDefaultTexCoords();
}


CCTextureManager::~CCTextureManager()
{
	textureHandles.deleteObjectsAndList();
	
    fontPages.deleteObjectsAndList();
}


static float textureLoadTimeOut;
static uint textureLoadLimit;
void CCTextureManager::prepareRender()
{
    bool cameraMoving = false;
    for( int i=0; i<gEngine->cameras.length; ++i )
    {
        if( gEngine->cameras.list[i]->updatedPosition )
        {
            cameraMoving = true;
            break;
        }
    }
    
    if( cameraMoving )
    {
        textureLoadTimeOut = 0.25f;
        textureLoadLimit = 1;
    }
    else
    {
        textureLoadTimeOut = 0.05f;
        textureLoadLimit = 2;
    }
    
    textureLoadingTimeout -= gEngine->gameTime.real;
    if( textureLoadingTimeout <= 0.0f )
    {
        totalTexturesLoadedThisFrame = 0;
    }
}


void CCTextureManager::load()
{
    // Load in a 1x1 white texture to use for untextured draws
    assignTextureIndex( "Resources/Textures/white.png", Resource_Packaged, true, false );
    setTextureIndex( 0 );
}


void CCTextureManager::loadFont(const char *font)
{
    CCText path = "Resources/Fonts/";
    path += font;
    CCTextureFontPageFile *fontPage = new CCTextureFontPageFile( font );
    fontPage->load( path.buffer, Resource_Packaged, true );
    fontPages.add( fontPage );
}


const uint CCTextureManager::assignTextureIndex(const char *filePath, const CCResourceType resourceType, 
                                                const bool alwaysResident, const bool mipmap)
{
    for( int i=0; i<textureHandles.length; ++i )
    {
        const CCTextureHandle *textureHandle = textureHandles.list[i];
        if( strcmp( textureHandle->filePath.buffer, filePath ) == 0 )
        {
            return i;
        }
    }

    CCTextureHandle *handle = new CCTextureHandle( filePath, resourceType );
    textureHandles.add( handle );
    
    if( mipmap )
    {
        handle->mipmap = mipmap;
    }

    // By default all textures don't hang around
    if( alwaysResident == true && handle->alwaysResident == false )
    {
        handle->alwaysResident = true;
        loadTexture( *handle );
    }
    
    uint index = (uint)( textureHandles.length-1 );
    return (uint)index;
}


const bool CCTextureManager::loadTexture(CCTextureHandle &textureHandle)
{
#if defined PROFILEON
    CCProfiler profile( "CCTextureManager::loadTexture()" );
#endif

	CCTextureBase *texture = new CCTexturePNG();
    if( texture->loadAndCreate( textureHandle.filePath.buffer, textureHandle.resourceType, textureHandle.mipmap ) == false )
	{
        //ASSERT( false );
		delete texture;
		texture = NULL;
        textureHandle.loadable = false;
	}
	
	if( texture != NULL )
    {
        totalTexturesLoaded++;
        totalTextureSpace += texture->getAllocatedWidth() * texture->getAllocatedHeight() * 4; // 32bit=4bytes
        
        // Estimated texture usage, need to cater for bit depth and mip maps for more accuracy
        const uint maxSpace = 32 * 1024 * 1024;
        if( totalTextureSpace > maxSpace )
        {
            //DEBUGLOG( "CCTextureManager::Textures Loaded %i %i \n", totalTexturesLoaded, totalTextureSpace );
            const uint usedSpace = 24 * 1024 * 1024;
            while( totalTextureSpace > usedSpace )
            {
                float longestTime = -1.0f;
                CCTextureHandle *longestHandle = NULL;
                for( int i=1; i<textureHandles.length; ++i )
                {
                    CCTextureHandle *handle = textureHandles.list[i];
                    //DEBUGLOG( "handle: %s \n", handle->filePath.buffer );
                    if( handle != NULL )
                    {
                        if( handle->alwaysResident == false )
                        {
                            if( handle->texture != NULL )
                            {
                                const float distance = gEngine->gameTime.lifetime - handle->lastTimeUsed;
                                if( distance > longestTime )
                                {
                                    longestTime = distance;
                                    longestHandle = handle;
                                }
                            }
                        }
                    }
                }

                ASSERT( longestHandle != NULL );
                if( longestHandle != NULL )
                {
                    totalTexturesLoaded--;
                    totalTextureSpace -= longestHandle->texture->getBytes();
                    delete longestHandle->texture;
                    longestHandle->texture = NULL;
                    //DEBUGLOG( "CCTextureManager::Textures Loaded %i %i \n\n", totalTexturesLoaded, totalTextureSpace );
                }
            }
        }
        textureHandle.texture = texture;
        return true;
    }
    
    return false;
}


void CCTextureManager::bindTexture(const uint glName)
{
	if( currentTextureHandle != glName )
	{
		glBindTexture( GL_TEXTURE_2D, glName );
		currentTextureHandle = glName;
	}
}


const bool CCTextureManager::setTextureIndex(const int handleIndex)
{
    CCTextureHandle *handle = textureHandles.list[handleIndex];
    if( handle != NULL && handle->loadable )
    {
        if( handle->texture == NULL )
        {
            if( totalTexturesLoadedThisFrame >= textureLoadLimit || loadTexture( *handle ) == false )
            {
                setTextureIndex( 0 );
                return false;
            }
            totalTexturesLoadedThisFrame++;
            textureLoadingTimeout = textureLoadTimeOut;

            //LAMBDA_EMIT_ONCE( handle->onLoad )
            if( handle->onLoad.length > 0 )
            {
                for( int i=0; i<handle->onLoad.length; ++i )
                {
                    handle->onLoad.list[i]->run();
                }
                handle->onLoad.deleteObjectsAndList();
            }
        }

        handle->lastTimeUsed = gEngine->gameTime.lifetime;
        bindTexture( handle->texture->glName );
        return true;
	}
	else
	{
		setTextureIndex( 0 );
		return false;
	}	
}


CCTextureHandle* CCTextureManager::getTextureHandle(const int handleIndex)
{
    return textureHandles.list[handleIndex];
}


const CCTextureBase* CCTextureManager::getTexture(const int handleIndex)
{
    CCTextureHandle *handle = textureHandles.list[handleIndex];
    if( handle != NULL )
    {
        if( handle->texture == NULL )
        {
            loadTexture( *handle );
        }
        return handle->texture;
    }
    return NULL;
}


const CCTextureBase* CCTextureManager::getTexture(const char *filePath, const CCResourceType resourceType)
{
    const uint textureHandleIndex = assignTextureIndex( filePath, resourceType, false, false );
    const CCTextureBase *texture = getTexture( textureHandleIndex );
    return texture;
}
