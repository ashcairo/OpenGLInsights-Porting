/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTexturePNG.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCTexturePNG.h"
#include "CCTextureManager.h"


CCTexturePNG::CCTexturePNG()
{
}


CCTexturePNG::~CCTexturePNG()
{
}

#include <QImageReader>
const bool CCTexturePNG::load(const char *path, const CCResourceType resourceType, const bool generateMipMap)
{
    QString completeFilename = QtRootPath();
    completeFilename.append( path );

    //qDebug() << QImageReader::supportedImageFormats ();

    QImage image;
    bool loadedSuccessfully = image.load( completeFilename );
    ASSERT( loadedSuccessfully );
    if( loadedSuccessfully )
    {
        // Get information about image
        QImage::Format qtFormat = image.format();
        int bpp = image.depth();

        if( bpp < 8 || bpp > 32 || ( qtFormat == QImage::Format_Indexed8 ) )
        {
            // This loader does not support all possible CGImage types, such as paletted images
            // (no need to free any mem, the QImage is on the stack, so it will be deleted
            //  and the data it is using will be freed as the last and only handle
            // has been deleted...?)
            //ASSERT( false );
            return false ;
        }

        // Decide formats using bpp
        GLenum format, internalFormat;
        switch( bpp )
        {
                // Easy formats
        case 24 : { internalFormat = format = GL_RGB; break; }
        case 16 : { internalFormat = format = GL_LUMINANCE_ALPHA; break; }
        case 8  : { internalFormat = format = GL_LUMINANCE; break; }

        // Possibly trickier format
        case 32 :
            {
                internalFormat = format = GL_RGBA;
                switch( qtFormat )
                {
                case QImage::Format_RGB32:		// 0xffRRGGBB
                    format = GL_BGRA;
                    break;

                case QImage::Format_ARGB32:
                case QImage::Format_ARGB32_Premultiplied:
                    // Alpha first requires swap
                    format = GL_BGRA;
                    break;

                default:
                    ASSERT( false );
                    return false;
                }
                break;
            }

        // Error
        default:
            {
                ASSERT( false );
                return false;
            }
        }

        // Get pixel data
        GLubyte *pixels = (GLubyte*)image.bits();
        allocatedWidth = imageWidth = image.width();
        allocatedHeight = imageHeight = image.height();

        const uint components = ( bpp >> 3 );

        // Shuffle image data if format is one we don't support
        uint rowBytes = image.bytesPerLine();
        if( format == GL_BGRA )
        {
            uint imgWide = rowBytes / components;
            uint num = imgWide * imageHeight;
            uint32_t *p = (uint32_t*)pixels;	// quint32?

            // All current iPhoneOS devices support BGRA via an extension.
            static const bool BGRASupport = extensionSupported( "GL_IMG_texture_format_BGRA8888" );
            if( BGRASupport == false )
            {
                format = GL_RGBA;

                // Convert from BGRA to RGBA
                for( uint i=0; i<num; ++i )
                {
//#if __LITTLE_ENDIAN__
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
                    p[i] = ( ( p[i] >> 16 ) & 0xFF ) | ( p[i] & 0xFF00FF00 ) | ( ( p[i] & 0xFF ) << 16 );

                    // We swap the MSB of each word, so BGRA becomes RGBA
#else
                    p[i] = ( ( p[i] & 0xFF00 ) << 16 ) | ( p[i] & 0xFF00FF ) | ( ( p[i] >> 16 ) & 0xFF00 );

                    // We swap the LSB of each word, i.e. if BGRA is stored as GBAR in memory
                    // (with G at add+0 and R at add+3), it becomes GRAB.
                    // That assumes the data is read word by word!?
#endif
                }
            }
        }

        // How much space is the texture?
        allocatedBytes = allocatedWidth * allocatedHeight * components;

        glGenTextures( 1, &glName );
        gEngine->textureManager->bindTexture( glName );
        glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, allocatedWidth, allocatedHeight, 0, format, GL_UNSIGNED_BYTE, pixels );

        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        gEngine->textureManager->bindTexture( 0 );

        return true;
    }

    return false;
}


const bool CCTexturePNG::createGLTexture()
{
	return true;	
}
