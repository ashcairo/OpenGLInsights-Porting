/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCModelBase.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCPrimitives.h"


CCModelBase::CCModelBase()
{	
	colour = colourOutline = NULL;
    
    shader = "basic";
}


void CCModelBase::destruct()
{
	if( scale )
	{
        FREE_POINTER( scale );
	}
	
	if( colour != NULL )
	{
		delete colour;
	}
	
	if( colourOutline != NULL )
	{
		delete colourOutline;
	}
	
	models.deleteObjectsAndList();
	primitives.deleteObjectsAndList();
	
	super::destruct();
}


void CCModelBase::render(const bool alpha)
{
#if defined PROFILEON
    CCProfiler profile( "CCModelBase::render()" );
#endif

    if( shouldRender )
    {
        GLPushMatrix();
        {
            refreshModelMatrix();
            GLMultMatrixf( modelMatrix );
            
            gEngine->renderer->setShader( shader );

            if( colourOutline != NULL )
            {
#if defined PROFILEON
            	CCProfiler profile( "CCModelBase::render()::colourOutline" );
#endif
                CCSetColour( *colourOutline );
                if( alpha == false && CCColourHasAlpha() )
                {
                    glEnable( GL_BLEND );
                }

                CCDefaultTexCoords();
                gEngine->textureManager->setTextureIndex( 0 );
                for( int i=0; i<primitives.length; ++i )
                {
                    primitives.list[i]->renderOutline();
                }

                if( alpha == false && CCColourHasAlpha() )
                {
                    glDisable( GL_BLEND );
                }
            }

            if( colour != NULL )
            {
                CCSetColour( *colour );
            }

            if( alpha == false || colour == NULL || ( colour != NULL && colour->alpha > 0.0f ) )
            {
                for( int i=0; i<primitives.length; ++i )
                {
                    CCPrimitiveBase *primitive = primitives.list[i];
                    primitive->render();
                }
            }

            for( int i=0; i<models.length; ++i )
            {
                CCModelBase *model = models.list[i];
                model->render( alpha );
                if( colour != NULL )
                {
                    CCSetColour( *colour );
                }
            }
        }
        GLPopMatrix();
    }
}


void CCModelBase::addModel(CCModelBase *model)
{
    ASSERT( model != this );
	models.add( model );
}


void CCModelBase::addPrimitive(CCPrimitiveBase *primitive)
{
	primitives.add( primitive );
}


void CCModelBase::setColour(const CCColour &inColour)
{
	if( colour == NULL )
	{
		colour = new CCColour();
	}
	
	*colour = inColour;
}


void CCModelBase::setColourAlpha(const float inAlpha)
{
    if( colour == NULL )
    {
        colour = new CCColour();
    }
    colour->alpha = inAlpha;
    setColour( *colour );
}


void CCModelBase::setOutline(const CCColour &inColour)
{
	if( colourOutline == NULL )
	{
		colourOutline = new CCColour();
	}
	
	*colourOutline = inColour;
}
