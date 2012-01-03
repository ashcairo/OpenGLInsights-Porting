/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCModelText.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCObjects.h"
#include "CCTextureManager.h"
#include "CCTextureFontPage.h"


CCModelText::CCModelText(CCSceneCollideable *inParent)
{
    primitive = NULL;
    parent = NULL;
    setParent( inParent );
    
    shader = "alphacolour";
    
    colourInterpolator.setDuration( 0.5f );
}


void CCModelText::destruct()
{
    super::destruct();
}


void CCModelText::setParent(CCSceneCollideable *inParent)
{
    ASSERT( parent == NULL );
    parent = inParent;
    if( inParent != NULL )
    {
        if( parent->model == NULL )
        {
            parent->model = this;
        }
        else
        {
            parent->model->addModel( this );
        }
    }
}


void CCModelText::setText(const char *text, const float height, const char *font)
{
    if( primitive == NULL )
    {
        translate( 0.0f, 0.0f, CC_SMALLFLOAT );
        setColour( CCColour( 0.0f ) );
        primitive = new CCPrimitiveText( text );
        addPrimitive( primitive );
    }
    else
    {
        primitive->setText( text );
    }
    
    if( height != -1.0f )
    {
        setHeight( height );
    }
    
    if( font != NULL )
    {
        setFont( font );
    }
    else
    {
        setFont( "HelveticaNeueLight" );
    }
}


void CCModelText::setHeight(const float height)
{
    primitive->setHeight( height );
}


void CCModelText::setCentered(const bool centered)
{
    if( centered )
    {
        setPositionX( 0.0f );
    }
    else if( parent != NULL )
    {
        setPositionX( -parent->collisionBounds.x );
    }
    primitive->setCentered( centered );
}


void CCModelText::setColour(const CCColour &inColour)
{
    super::setColour( inColour );
    colourInterpolator.setup( colour, *colour );
}


void CCModelText::setFont(const char *font)
{
    primitive->setFont( font );
}



// CCPrimitiveText
CCPrimitiveText::CCPrimitiveText(const char *inText, const float inHeight)
{	
    setText( inText );
    setHeight( inHeight );
    setCentered( true );
}


void CCPrimitiveText::destruct()
{	
	super::destruct();
}


// CCPrimitiveBase
void CCPrimitiveText::renderVertices(const bool textured)
{	
    if( text.buffer != NULL )
    {
        fontPage->renderText3D( text.buffer, text.length, 
                                0.0f, 0.0f, 0.0f,
                                height,
                                centered );
    }
}


void CCPrimitiveText::setText(const char *inText)
{
    text = inText;
}


void CCPrimitiveText::setFont(const char *font)
{
    for( int i=0; i<gEngine->textureManager->fontPages.length; ++i )
    {
        CCTextureFontPage *page = gEngine->textureManager->fontPages.list[i];
        const char *name = page->getName();
        if( CCText::Equals( font, name ) )
        {
            fontPage = page;
            return;
        }
    }
    ASSERT( false );
}


const float CCPrimitiveText::getWidth()
{
    return fontPage->getWidth( text.buffer, text.length, height );
}

