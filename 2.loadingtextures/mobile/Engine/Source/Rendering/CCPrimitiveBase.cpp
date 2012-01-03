/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCPrimitiveBase.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCPrimitives.h"


CCPrimitiveBase::CCPrimitiveBase()
{
	vertices = NULL;
	normals = NULL;
	textureInfo = NULL;
}


void CCPrimitiveBase::destruct()
{	
    FREE_POINTER( vertices );
	
	if( normals != NULL )
	{
        FREE_POINTER( normals );
	}
    
    removeTexture();
}


void CCPrimitiveBase::setTexture(const char *name, const CCResourceType resourceType, 
                                 const bool alwaysResident, const bool mipmap)
{
	if( textureInfo == NULL )
	{
		textureInfo = new TextureInfo();
	}
    textureInfo->primaryIndex = gEngine->textureManager->assignTextureIndex( name, resourceType, alwaysResident, mipmap );
    
    const int textureHandleIndex = textureInfo->primaryIndex;
    CCTextureHandle *textureHandle = gEngine->textureManager->getTextureHandle( textureHandleIndex );
    if( textureHandle->texture != NULL )
    {
        adjustTextureUVs();
    }
    else
    {
        //LAMBDA_CONNECT( textureHandle->onLoad, CCPrimitiveBase, this, adjustTextureUVs() );
        //LAMBDA_1( EventCallback, CCPrimitiveBase, primitive,
        //    primitive->adjustTextureUVs();
        //)
        class EventCallback : public CCLambdaCallback
        {
        public:
            EventCallback(CCPrimitiveBase *primitive)
            {
                this->primitive = primitive;
                this->lazyPointer = primitive->lazyPointer;
                this->lazyID = primitive->lazyID;
            }
            void run()
            {
                if( CCLazyCallback::isCallbackActive( lazyPointer, lazyID ) )
                {
                    primitive->adjustTextureUVs();
                }
            }
        private:
            CCPrimitiveBase *primitive;
            void *lazyPointer;
            long lazyID;
        };
        textureHandle->onLoad.add( new EventCallback( this ) );
    }
}


void CCPrimitiveBase::removeTexture()
{
	if( textureInfo != NULL )
	{
        DELETE_POINTER( textureInfo );
	}
}


void CCPrimitiveBase::render()
{	
#if defined PROFILEON
    CCProfiler profile( "CCPrimitiveBase::render()" );
#endif

	bool usingTexture = false;
	if( textureInfo != NULL && textureInfo->primaryIndex > 0 )
	{
		if( gEngine->textureManager->setTextureIndex( textureInfo->primaryIndex ) )
		{
			usingTexture = true;
            
            if( textureInfo->secondaryIndex > 0 )
            {
                // Why would you want to use the same texture twice? you wouldn't.. bad!
                ASSERT( textureInfo->primaryIndex != textureInfo->secondaryIndex );
#ifndef QT
                glActiveTexture( GL_TEXTURE1 );
                gEngine->textureManager->setTextureIndex( textureInfo->secondaryIndex );
                glActiveTexture( GL_TEXTURE0 );
#endif
            }
		}
	}
	else
	{
		gEngine->textureManager->setTextureIndex( 0 );
	}
	
	renderVertices( usingTexture );
}
