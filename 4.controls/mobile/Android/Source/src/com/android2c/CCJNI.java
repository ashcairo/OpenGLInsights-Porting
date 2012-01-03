/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright � 2010 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCJNI.java
 * Description : Interfaces with ndk library.
 *
 * Created     : 09/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

package com.android2c;

import android.util.Log;
import android.webkit.CookieManager;


// Wrapper for native library
public class CCJNI
{
	static final String library = "ccjni";
	static final boolean debug = false;
	
	static CCActivity activity;
	static String dataPath;

    public static void setActivity(CCActivity inActivity)
    {
    	CCJNI.activity = inActivity;
    	dataPath = activity.getFilesDir().getAbsolutePath();
    	fileManagerSetPaths( activity.getPackageResourcePath(), dataPath );
    }
	
	// Load our library
	static 
 	{
 		System.loadLibrary( CCJNI.library );
 		System.out.println();
 	}

	
 	// main
	public static native void onSurfaceChanged(int width, int height);
	public static native void onDrawFrame();
	
	private void Assert(final String file, final int line, final String message)
	{
        Log.e( CCJNI.library, String.format( "%s: Assert: %i %s", line, message ) );
	}
	
	// CCGLTexture
	private int textureLoad(final String filename, final boolean mipmap, final boolean packaged)
	{
		return CCGLTexture.load( filename, mipmap, packaged );
	}
	
	private int textureGetWidth()
	{
		return CCGLTexture.getWidth();
	}
	
	private int textureGetHeight()
	{
		return CCGLTexture.getHeight();
	}
	
	private int textureGetScaledHeight()
	{
		return CCGLTexture.getScaledHeight();
	}
	
	private void textureReleaseRawData()
	{
		CCGLTexture.releaseRawData();
	}

	
	// CCDeviceControls
	public static native void controlsHandleTouch(float x, float y, int actionType, int finger);

	public static native boolean controlsHandleBackButton();
	
	
	// CCDeviceFileManager
	public static native void fileManagerSetPaths(String apkPath, String dataPath);
	
	
	// CCDeviceURLManager
	static CCURLManagerJNI urlManager = null;
 
	private void urlManagerProcessRequest(final String url)
	{
		new Thread(new Runnable()
		{
			public void run() 
		    {
			 	if( urlManager == null )
			 	{
				 	urlManager = new CCURLManagerJNI();
			 	}
			 	urlManager.processRequest( url );
		 	}
	
		}).start();
	}

 	public static native void urlManagerDownloadFinished(String url, boolean success, byte[] data, int dataLength, 
 														 String[] headerNames, String[] headerValues, int headerLength);
 	
 	
 	// CCJNI
 	private void advertsToggle(boolean toggle)
 	{
 		activity.toggleAdverts( toggle );
 	}
 	
 	
 	private void webViewOpen(String url)
	{
	 	activity.openWebBrowser( url );
 	}
 	
 	private void webViewClose()
	{
	 	activity.closeWebBrowser();
 	}
 	
 	private void webViewClearData()
	{
 		CookieManager cookieManager = CookieManager.getInstance();
 		cookieManager.removeAllCookie();
 	}

 	public static native void webViewURLLoaded(String url);
 	
 	
  	private void videoViewStart(String file)
 	{
 	 	activity.startVideoView( file );
  	}
  	
  	private void videoViewStop()
 	{
 	 	activity.stopVideoView();
  	}
}
