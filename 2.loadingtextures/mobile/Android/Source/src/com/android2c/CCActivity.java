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

//import com.google.ads.*;


public class CCActivity extends Activity 
{
	RelativeLayout layout;
    CCGLViewJNI glView = null;
    CCWebViewJNI webView = null;
    CCVideoViewJNI videoView = null;
    
    //AdView adView = null;

    @Override protected void onCreate(Bundle icicle) 
    {
        super.onCreate( icicle );
        this.setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_PORTRAIT );
        
        CCJNI.setActivity( this );
        
        glView = new CCGLViewJNI( getApplication() );
        layout = new RelativeLayout( this );
        layout.addView( glView );
        
        // Admob
//        if( false )
//        {
//	    	// Create ad view
//	    	adView = new AdView( this, AdSize.BANNER, "a14e9b3ad1bb801" );
//	    	
//	    	// Re-position to footer
//	    	WindowManager windowManager = (WindowManager)getSystemService( Context.WINDOW_SERVICE ); 
//	        Display display = windowManager.getDefaultDisplay();
//	        int screenWidth = display.getWidth();
//	        int screenHeight = display.getHeight();
//	        int bannerWidth = AdSize.BANNER.getWidth();
//	        int bannerHeight = AdSize.BANNER.getHeight();
//	        float scale = (float)screenWidth / (float)bannerWidth;
//	        int scaledHeight = (int)( (float)bannerHeight * scale );
//	        int y = screenHeight - scaledHeight;
//	        
//	        RelativeLayout.LayoutParams viewParams = new RelativeLayout.LayoutParams( screenWidth, scaledHeight );
//	        viewParams.topMargin = y;
//	        viewParams.alignWithParent = true;
//	    	adView.setLayoutParams( viewParams );
//	    	
//	    	layout.addView( adView );
//
//	        // Initiate a generic request to load it with an ad
//	        adView.loadAd( new AdRequest() );
//	        
//	        toggleAdverts( false );
//        }
        
        setContentView( layout );
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) 
    {
        if( keyCode == KeyEvent.KEYCODE_BACK )
        {
        	if( CCJNI.controlsHandleBackButton() )
        	{
        		return true;
        	}
        }
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
//    	if( adView != null )
//    	{
//    		adView.destroy();
//    	}
    	super.onDestroy();
    }
    
    
    public void toggleAdverts(final boolean toggle)
    {
//    	runOnUiThread(new Runnable() 
//    	{
//		    public void run() 
//		    {
//		    	if( adView != null )	
//		    	{
//		    		if( toggle )
//		    		{
//		    			adView.setVisibility( View.VISIBLE );
//		    			
//		    			// Fade the ad in over half of a second.
////		    	        AlphaAnimation animation = new AlphaAnimation( 0.0f, 1.0f );
////		    	        animation.setDuration( 500 );
////		    	        animation.setFillAfter( true );
////		    	        animation.setInterpolator( new AccelerateInterpolator() );
////		    	        adView.startAnimation( animation );
//		    		}
//		    		else
//		    		{
//		    			adView.setVisibility( View.GONE );
//		    			
//		    			// Disabled as it keeps the view active for touches
//		    			// Fade the ad out over half a second.
////		    	        AlphaAnimation animation = new AlphaAnimation( 1.0f, 0.0f );
////		    	        animation.setDuration( 500 );
////		    	        animation.setFillAfter( true );
////		    	        animation.setInterpolator( new AccelerateInterpolator() );
////		    	        adView.startAnimation( animation );
//		    		}
//		    	}
//		    }
//		});
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
