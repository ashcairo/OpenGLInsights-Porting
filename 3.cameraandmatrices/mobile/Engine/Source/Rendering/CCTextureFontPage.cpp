/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTextureFontPage.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCTextureFontPage.h"
#include "CCTextureManager.h"


const float CCTextureFontPage::getWidth(const char *text, const uint length, const float size) const
{
	float totalWidth = 0.0f;
	for( uint i=0; i<length; ++i )
	{
        char character = text[i];
        if( character == '\n' )
        {
            break;
        }
        else
        {
            const Letter *letter = getLetter( text[i] );
            if( letter != NULL )
            {
                totalWidth += letter->size.width * size;
            }
        }
	}
	
	return totalWidth;
}


const float CCTextureFontPage::getHeight(const char *text, const uint length, const float size) const
{
	float maxHeight = 0.0f;
	for( uint i=0; i<length; ++i )
	{
        const Letter *letter = getLetter( text[i] );
		if( letter != NULL )
		{
            maxHeight = MAX( maxHeight, letter->size.height * size );
		}
	}
	
	return maxHeight;
}


static float texCoords[8];
#define MAX_TEXT_LENGTH 1024
#define MAX_TEXT_LINES 10

void CCTextureFontPage::renderText(const char *text, const uint length, 
                                   const float x, const float y,
                                   const bool centeredX, const bool centeredY, const float size) const
{	
    ASSERT( length < MAX_TEXT_LENGTH );

    bindTexturePage();

	// Find out our width so we can center the text
    CCPoint lineSize[MAX_TEXT_LINES];
    CCPoint charSize[MAX_TEXT_LENGTH];
	uint numberOfLines = 0;
	for( uint i=0; i<length; ++i )
	{
        const Letter *letter = getLetter( text[i] );
		if( letter != NULL )
		{
            charSize[i].x = letter->size.width * size;
            charSize[i].y = letter->size.height * size;
			
			// New line
			if( text[i] == 10 )
			{
				numberOfLines++;
                ASSERT( numberOfLines < MAX_TEXT_LINES );
			}
			
            lineSize[numberOfLines].x += charSize[i].x;
            lineSize[numberOfLines].y = MAX( lineSize[numberOfLines].y, charSize[i].y );
		}
	}
	
    CCPoint max( lineSize[0].x, lineSize[0].y );
	for( uint i=1; i<numberOfLines; ++i )
	{
        max.x = MAX( max.x, lineSize[i].x );
        max.y += lineSize[i].y;
	}
	
    CCPoint start = CCPoint( x, y );
	if( centeredX )
	{
        start.x -= max.x * 0.5f;
	}
	if( centeredY )
	{
        start.y -= max.y * gEngine->renderer->aspectRatio;
	}
	
    CCPoint currentStart = start;
    CCPoint currentEnd;
	
	CCSetTexCoords( texCoords );
	
	uint lineNumber = 0;
	for( uint i=0; i<length; ++i )
	{
		const Letter *letter = getLetter( text[i] );
		if( letter != NULL )
		{
			// Calculate end point
            currentEnd.x = currentStart.x + charSize[i].x;
            currentEnd.y = currentStart.y + charSize[i].y;
			
			// New line
			if( text[i] == 10 )
			{
                currentStart.y += lineSize[lineNumber++].y;
				//currentStart.x = lineStart[lineNumber];
			}
			else
			{
				texCoords[0] = letter->start.x;
				texCoords[1] = letter->start.y;
				texCoords[2] = letter->end.x;
				texCoords[3] = letter->start.y;
				texCoords[4] = letter->start.x;
				texCoords[5] = letter->end.y;
				texCoords[6] = letter->end.x;
				texCoords[7] = letter->end.y;
			
				CCRenderSquare2D( currentStart, currentEnd, false );
                currentStart.x += charSize[i].x;
			}
		}
	}
}


void CCTextureFontPage::renderText3D(const char *text, const uint length,
                                     const float x, const float y, const float z,
                                     const float height, const bool centeredX) const
{
#if defined PROFILEON
    CCProfiler profile( "CCTextureFontPage::renderText3D()" );
#endif

    // TODO: I draw these wrong, please fix me to work with back face culling
    glCullFace( GL_FRONT );

    ASSERT( length < MAX_TEXT_LENGTH );

    bindTexturePage();

    // Find out our width so we can center the text
    int lineIndex = 0;
    
    CCPoint lineSize[MAX_TEXT_LINES];
    static CCPoint charSize[MAX_TEXT_LINES][MAX_TEXT_LENGTH];
    int characterIndex = 0;
    for( uint i=0; i<length; ++i )
    {
        char character = text[i];
        if( character == '\n' )
        {
            lineIndex++;
            characterIndex = 0;
            ASSERT( lineIndex < MAX_TEXT_LINES );
        }
        else
        {
            const Letter *letter = getLetter( character );
            if( letter != NULL )
            {
                CCPoint &size = charSize[lineIndex][characterIndex];
                size.x = letter->size.width * height;
                size.y = letter->size.height * height;

                lineSize[lineIndex].x += size.x;
                lineSize[lineIndex].y = MAX( lineSize[lineIndex].y, size.y );
                characterIndex++;
            }
        }
    }

    CCPoint start( x, y );
    if( centeredX )
    {
        start.x -= lineSize[0].x * 0.5f;
    }
    start.y += lineSize[0].y * 0.5f;

    static CCVector3 currentStart, currentEnd;
    currentStart.x = start.x;
    currentStart.y = start.y;
    currentStart.z = -z;
    currentEnd.z = z;

    CCSetTexCoords( texCoords );
    
    lineIndex = 0;
    characterIndex = 0;
    for( uint i=0; i<length; ++i )
    {
        char character = text[i];
        if( character == '\n' )
        {
            currentStart.x = start.x;
            currentStart.y -= lineSize[lineIndex].y;
            lineIndex++;
            characterIndex = 0;
        }
        else
        {
            const Letter *letter = getLetter( character );
            if( letter != NULL )
            {
                CCPoint &size = charSize[lineIndex][characterIndex];
                
                // Calculate end point
                currentEnd.x = currentStart.x + size.x;
                currentEnd.y = currentStart.y - size.y;

                texCoords[0] = letter->start.x;
                texCoords[1] = letter->start.y;
                texCoords[2] = letter->end.x;
                texCoords[3] = letter->start.y;
                texCoords[4] = letter->start.x;
                texCoords[5] = letter->end.y;
                texCoords[6] = letter->end.x;
                texCoords[7] = letter->end.y;

                CCRenderSquare( currentStart, currentEnd );
                currentStart.x += size.x;
                characterIndex++;
            }
        }
    }

    glCullFace( GL_BACK );
}


void CCTextureFontPage::view() const
{	
	bindTexturePage();

    const CCPoint start = CCPoint( 0.0f, 0.4f );
    const CCPoint end = CCPoint( start.x, start.y );
	
	CCRenderSquare2D( start, end, false );
	
	gEngine->textureManager->setTextureIndex( 0 );
}


const CCTextureFontPage::Letter* CCTextureFontPage::getLetter(const char character) const
{
	if( character >= 0 )
    {
        return &letters[(uint)character];
	}
	else if( character == -62 )
    {
        return &letters[128];
	}
	
	return NULL;
}
