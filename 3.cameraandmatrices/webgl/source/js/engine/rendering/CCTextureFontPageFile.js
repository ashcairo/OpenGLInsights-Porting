/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCTextureFontPageFile.js
 * Description : Handles loading font description files.
 *
 * Created     : 23/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCTextureFontPageFile(inName)
{
    function Letter()
    {
        this.start = new CCPoint();
        this.end = new CCPoint();
        this.size = new CCSize();
    }
    var letters = this.letters = new Array();
    for( var i=0; i<256; ++i )
    {
        letters.push( new Letter() );
    }
    
    this.name = inName;
    
    // Set up for RenderText3D
    var MAX_TEXT_LINES = this.MAX_TEXT_LINES = 10;
    var MAX_TEXT_LENGTH = 1024;
    
    var lineSize = this.lineSize = new Array();
    for( var i=0; i<MAX_TEXT_LINES; ++i )
    {
        lineSize.push( new CCPoint() );
    }
    
    var charSize = this.charSize = new Array();
    for( var i=0; i<MAX_TEXT_LINES; ++i )
    {
        charSize[i] = new Array();
        for( var j=0; j<MAX_TEXT_LENGTH; ++j )
        {
            charSize[i][j] = new CCPoint();
        }
    }
    
    this.start = new CCPoint();
    this.currentStart = vec3.create();
    this.currentEnd = vec3.create();
    
    this.texCoords = new Float32Array( 8 );
}


CCTextureFontPageFile.prototype.load = function(path)
{
	var textureFile = path + ".png";
    this.texturePageIndex = gEngine.textureManager.assignTextureIndex( textureFile );
    
    var dataFile = path + ".data";
    
    var HTTPCallback = function(fontPage)
    {
        this.fontPage = fontPage;
    }
    HTTPCallback.prototype.run = function(status, responseText, httpObject)
    {
        if( status >= CCURLRequest_succeeded )
        {
            var letters = this.fontPage.letters
            
            var lettersSplit = responseText.split( "\n" );
            for( var i=0; i<lettersSplit.length; ++i )
            {
                var rawLetterData = lettersSplit[i];
                
                var letterDataSplit = rawLetterData.split( "," );
                //ASSERT( letterDataSplit->length == 4 );
                
                var letter = letters[i];
                letter.start.x = parseFloat( letterDataSplit[0] );
                letter.start.y = parseFloat( letterDataSplit[1] );
                letter.end.x = parseFloat( letterDataSplit[2] );
                letter.end.y = parseFloat( letterDataSplit[3] );
                
                // 16.0f because there's 16 tiles per font page
                letter.size.width = ( letter.end.x - letter.start.x ) * 16.0;
                letter.size.height = ( letter.end.y - letter.start.y ) * 16.0;
                
                // WebGL likes proper UVs
                letter.start.y = 1.0 - letter.start.y;
                letter.end.y = 1.0 - letter.end.y;
            }
        }
    }
    gURLManager.requestURL( dataFile, null, new HTTPCallback( this ) );
	
	return true;
}


CCTextureFontPageFile.prototype.bindTexturePage = function(texture)
{
    gEngine.textureManager.setTextureIndex( this.texturePageIndex );
}


CCTextureFontPageFile.prototype.getLetter = function(character)
{
    var asciiCharacter = character.charCodeAt( 0 );
	if( asciiCharacter >= 0 )
    {
        return this.letters[asciiCharacter];
	}
	else if( asciiCharacter == -62 )
    {
        return this.letters[128];
	}
	
	return false;
}



CCTextureFontPageFile.prototype.getWidth = function(text, length, size)
{
    var totalWidth = 0.0;
	for( var i=0; i<length; ++i )
	{
        var character = text[i];
        if( character == '\n' )
        {
            break;
        }
        else
        {
            var letter = this.getLetter( text[i] );
            if( letter )
            {
                totalWidth += letter.size.width * size;
            }
        }
	}
	
	return totalWidth;
}


CCTextureFontPageFile.prototype.getHeight = function(text, length, size)
{
    var maxHeight = 0.0;
	for( var i=0; i<length; ++i )
	{
        var letter = this.getLetter( text[i] );
		if( letter )
		{
            maxHeight = Math.max( maxHeight, letter.size.height * size );
		}
	}
	
	return maxHeight;
}

CCTextureFontPageFile.prototype.renderText3D = function(text, length, 
                                                        x, y, z, 
                                                        height, centeredX)
{   
    // TODO: I draw these wrong, please fix me to work with back face culling
    //glCullFace( GL_FRONT );
    
    //ASSERT( length < MAX_TEXT_LENGTH );
    
    this.bindTexturePage();
    
    // Find out our width so we can center the text
    var lineIndex = 0;
    var MAX_TEXT_LINES = this.MAX_LINES;
    
    var lineSize = this.lineSize;
    lineSize[lineIndex].x = lineSize[lineIndex].y = 0.0;
    
    var charSize = this.charSize;
    var characterIndex = 0;
    for( var i=0; i<length; ++i )
    {
        var character = text[i];
        if( character == '\n' )
        {
            lineIndex++;
            characterIndex = 0;
            lineSize[lineIndex].x = lineSize[lineIndex].y = 0.0;
            //ASSERT( lineIndex < MAX_TEXT_LINES );
        }
        else
        {
            var letter = this.getLetter( character );
            if( letter )
            {
                var size = charSize[lineIndex][characterIndex];
                size.x = letter.size.width * height;
                size.y = letter.size.height * height;
                
                lineSize[lineIndex].x += size.x;
                lineSize[lineIndex].y = Math.max( lineSize[lineIndex].y, size.y );
                characterIndex++;
            }
        }
    }
    
    var start = this.start;
    start.x = x;
    start.y = y;
    if( centeredX )
    {
        start.x -= lineSize[0].x * 0.5;
    }
    start.y += lineSize[0].y * 0.5;
    
    var currentStart = this.currentStart;
    var currentEnd = this.currentEnd;
    currentStart[0] = start.x;
    currentStart[1] = start.y;
    currentStart[2] = -z;
    currentEnd[2] = z;

    var renderer = gEngine.renderer;
    renderer.setMatrixUniforms();
    var texCoords = this.texCoords;
    
    //var gl = renderer.gl;
    //gl.enable( gl.BLEND );
    
    lineIndex = 0;
    characterIndex = 0;
    for( var i=0; i<length; ++i )
    {
        var character = text[i];
        if( character == '\n' )
        {
            currentStart.x = start.x;
            currentStart.y -= lineSize[lineIndex].y;
            lineIndex++;
            characterIndex = 0;
        }
        else
        {
            var letter = this.getLetter( character );
            if( letter )
            {
                var size = charSize[lineIndex][characterIndex];
                
                // Calculate end point
                currentEnd[0] = currentStart[0] + size.x;
                currentEnd[1] = currentStart[1] - size.y;
                
                texCoords[0] = letter.start.x;
                texCoords[1] = letter.start.y;
                texCoords[2] = letter.end.x;
                texCoords[3] = letter.start.y;
                texCoords[4] = letter.start.x;
                texCoords[5] = letter.end.y;
                texCoords[6] = letter.end.x;
                texCoords[7] = letter.end.y;
                renderer.CCSetTexCoords( texCoords );
                
                renderer.CCRenderHorizontalSquare( currentStart, currentEnd );
                currentStart[0] += size.x;
                characterIndex++;
            }
        }
    }
    
    renderer.CCDefaultSquareVertexPointer();
    renderer.CCDefaultTexCoords();
    
    //gl.disable( gl.BLEND );
    //glCullFace( GL_BACK );    
}