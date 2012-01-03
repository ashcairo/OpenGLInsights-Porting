/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright ﾩ 2010 ￐ 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCActivity.java
 * Description : Android entry point.
 *
 * Created     : 15/06/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

package com.android2c;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.view.KeyEvent;
import android.widget.RelativeLayout;


public class CCActivity extends Activity 
{
	RelativeLayout layout;
    CCGLViewJNI glView = null;
    CCWebViewJNI webView = null;
    CCVideoViewJNI videoView = null;

    @Override protected void onCreate(Bundle icicle) 
    {
        super.onCreate( icicle );
        this.setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_PORTRAIT );
        
        CCJNI.setActivity( this );
        
        glView = new CCGLViewJNI( getApplication() );
        layout = new RelativeLayout( this );
        layout.addView( glView );
        
        setContentView( layout );
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) 
    {
        return super.onKeyDown(keyCode, event);
    }

    @Override
    protected void onPause() 
    {
        super.onPause();
        glView.onPause();
        
        if( webView != null )
        {
        	webView.toggle( false );
        }
        
        if( videoView != null )
        {
        	stopVideoView();
        }
    }

    @Override
    protected void onResume() 
    {   
        super.onResume();
        glView.onResume();

        if( webView != null )
        {
        	webView.toggle( true );
        }
    }
    
    @Override
    public void onDestroy() 
    {
    	super.onDestroy();
    }
    
    
    public void toggleAdverts(final boolean toggle)
    {
    }
    
    
    public void openWebBrowser(final String url)
    {
    	final CCActivity activity = this;
		runOnUiThread(new Runnable() 
		{
		    public void run() 
		    {
		    	if( webView == null )
		    	{
    	            webView = new CCWebViewJNI( activity );
    	            layout.addView( webView );
    	    	}
    	    	webView.loadUrl( url );
		    }
		});
    }
    
    public void closeWebBrowser()
    {
    	runOnUiThread(new Runnable() 
		{
		    public void run() 
		    {
		    	if( webView != null )
		    	{
		    		layout.removeView( webView );
		    		webView.destroy();
		    		webView = null;
		    	}
		    }
		});
    }
    
    
    public void startVideoView(final String file)
    {
    	final CCActivity activity = this;
    	
    	runOnUiThread(new Runnable() 
		{
		    public void run() 
		    {		    	
		    	// Introduced a 100ms timeout to allow for edge cases when a video
		    	// is requested as the OpenGL context is still starting up
		    	// Without this, the video player context can appear on top of the OpenGL context
		    	final Handler handler = new Handler();
				final Runnable r = new Runnable()
				{
				    public void run() 
				    {
				    	if( videoView == null )
				    	{
		    	            videoView = new CCVideoViewJNI( activity, file );
		    	            layout.addView( videoView );
		    	    	}
				    }
				};

				handler.postDelayed( r, 100 );
		    }
		});
    }
    
    public void stopVideoView()
    {
		runOnUiThread(new Runnable() 
		{
		    public void run() 
		    {		    	
		    	if( videoView != null )
		    	{
		    		layout.removeView( videoView );
		    		videoView.destroy();
		    		videoView = null;
		    	}
		    }
		});
    }
}
