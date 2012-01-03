/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 Ð 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCWebViewJNI.java
 * Description : Interfaces with ndk on web page load.
 *
 * Created     : 09/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

package com.android2c;

import java.lang.reflect.InvocationTargetException;

import android.content.Context;
import android.view.Display;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.RelativeLayout;
import android.view.WindowManager;


class CCWebViewJNI extends WebView
{
    public CCWebViewJNI(Context context) 
    {
        super( context );
        
        WindowManager windowManager = (WindowManager)context.getSystemService( Context.WINDOW_SERVICE ); 
        Display display = windowManager.getDefaultDisplay();
        int screenWidth = display.getWidth();
        int screenHeight = display.getHeight();
        
        float width = screenWidth * 1.0f;
        float height = screenHeight * 0.75f;
        float x = screenWidth * 0.0f * 0.5f;
        float y = screenHeight * 0.25f * 0.5f;
        
        RelativeLayout.LayoutParams viewParams = new RelativeLayout.LayoutParams( (int)width, (int)height );
        viewParams.leftMargin = (int)x;
        viewParams.topMargin = (int)y;
    	setLayoutParams( viewParams );
    	
    	// Allow keyboard input
    	requestFocus( View.FOCUS_DOWN );
    	
    	setWebViewClient(new WebViewClient() 
    	{
    	    public boolean shouldOverrideUrlLoading(WebView view, String url)
    	    {
    	        // do your handling codes here, which url is the requested url
    	        // probably you need to open that url rather than redirect:
    	        view.loadUrl( url );
    	        CCJNI.webViewURLLoaded( url );
    	        return false; // then it is not handled by default action
    	   }
    	});
    }
    
    public void toggle(boolean toggle)
    {
    	String method = toggle ? "onResume" : "onPause";
    	
    	try 
    	{
			Class.forName( "android.webkit.WebView" ).getMethod( method, (Class[]) null).invoke( this, (Object[]) null );
		}
    	catch (IllegalArgumentException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		catch (SecurityException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		catch (IllegalAccessException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (InvocationTargetException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (NoSuchMethodException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (ClassNotFoundException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
}
