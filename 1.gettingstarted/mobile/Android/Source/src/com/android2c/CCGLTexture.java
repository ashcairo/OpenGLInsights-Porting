/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright ﾩ 2010 ￐ 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCGLTexture.java
 * Description : Interfaces with Bitmap class to load textures.
 *
 * Created     : 09/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

package com.android2c;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.opengles.GL11;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLUtils;

public class CCGLTexture
{
	static Bitmap bitmap = null;
	
	// On Android we always scaled our images to be square sizes
	static int scaledHeight = 0;
	
	static int NextPowerOfTwo(int x)
	{
		x = x - 1;
		x = x | ( x >> 1 );
		x = x | ( x >> 2 );
		x = x | ( x >> 4 );
		x = x | ( x >> 8 );
		x = x | ( x >>16 );
		return x + 1;
	}
	
	static public int load(final String filename, final boolean mipmap, final boolean packaged)
	{
		if( packaged )
		{
			int dotIndex = filename.lastIndexOf( '.' );
			
			final String pureFilename = ( dotIndex > 0 ) ? filename.substring( 0, dotIndex ) : filename;  
			int id = CCJNI.activity.getResources().getIdentifier( pureFilename, "drawable", "com.android2c" );
	
	        try 
	        {
	        	InputStream is = CCJNI.activity.getResources().openRawResource( id );
	            bitmap = BitmapFactory.decodeStream( is );
	    		is.close();
	        } 
	        catch(IOException e) 
	        {
	            // Ignore.
	        	e.printStackTrace();
	        }
		}
		
		// Cached
		else
		{
			final String filePath = CCJNI.dataPath + "/" + filename;
			
			try 
	        {
				FileInputStream is = new FileInputStream( filePath );
	            bitmap = BitmapFactory.decodeStream( is );
	    		is.close();
	        } 
	        catch(IOException e) 
	        {
	            // Ignore.
	        	e.printStackTrace();
	        }
		}
        
        if( bitmap == null )
        {
        	return 0;
        }
        else
        {
        	// Re-scale the width to be a power of 2 to avoid any weird stretching issues.
        	int width = bitmap.getWidth();
        	int height = scaledHeight = bitmap.getHeight();
        	int widthSquared = NextPowerOfTwo( width );
        	int heightSquared = NextPowerOfTwo( height );
        	
        	// Always re-scale as, a few devices have trouble using the provided textures
        	//if( width != widthSquared || height != heightSquared )
        	{
        		float scale = (float)widthSquared/(float)width;
        		scaledHeight = (int)( height * scale );
        		heightSquared = NextPowerOfTwo( scaledHeight );
        		Bitmap scaledBitmap = Bitmap.createScaledBitmap( bitmap, widthSquared, heightSquared, true );
        		bitmap.recycle();
        		bitmap = scaledBitmap;
        	}
        	
			int[] glName = new int[1];
			
			//GL11 gl = CCGLViewJNI.glContext;
			GL11 gl = CCGLViewJNI.glContext;
	        gl.glGenTextures( 1, glName, 0 );
	
	        gl.glBindTexture( GL10.GL_TEXTURE_2D, glName[0] );

	       	gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR );
	        if( false && mipmap )
	        {
	        	gl.glTexParameteri( GL10.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR_MIPMAP_LINEAR );
		        gl.glTexParameteri( GL10.GL_TEXTURE_2D, GL11.GL_GENERATE_MIPMAP, GL11.GL_TRUE );
	        }
	        else
	        {
	        	gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR );
	        }
	        
	        GLUtils.texImage2D( GL11.GL_TEXTURE_2D, 0, bitmap, 0 );
	        
	        gl.glBindTexture( GL10.GL_TEXTURE_2D, 0 );
	        
	        //CCGLViewJNI.checkGLError( "Load Texture", gl );
	        return glName[0];
        }
	}
	
	static public int getWidth()
	{
		return bitmap.getWidth();
	}
	
	static public int getHeight()
	{
		return bitmap.getHeight();
	}
	
	static public int getScaledHeight()
	{
		return scaledHeight;
	}

    static public void releaseRawData()
	{
    	bitmap.recycle();
    	bitmap = null;
	}
}
