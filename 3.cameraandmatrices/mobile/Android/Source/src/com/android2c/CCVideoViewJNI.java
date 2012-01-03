/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 Ð 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCVideoViewJNI.java
 * Description : Plays video.
 *
 * Created     : 09/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

package com.android2c;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnBufferingUpdateListener;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.OnVideoSizeChangedListener;
import android.util.Log;
import android.view.Display;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.widget.RelativeLayout;


public class CCVideoViewJNI extends SurfaceView implements
        OnBufferingUpdateListener, OnCompletionListener,
        OnPreparedListener, OnVideoSizeChangedListener, SurfaceHolder.Callback 
{
    private int mVideoWidth;
    private int mVideoHeight;
    private MediaPlayer mMediaPlayer;
    private SurfaceHolder holder;
    private boolean mIsVideoSizeKnown = false;
    private boolean mIsVideoReadyToBePlayed = false;
    private String file;

    public CCVideoViewJNI(Context context, final String inFile) 
    {
        super( context );
        
        WindowManager windowManager = (WindowManager)context.getSystemService( Context.WINDOW_SERVICE ); 
        Display display = windowManager.getDefaultDisplay();
        int screenWidth = display.getWidth();
        int screenHeight = display.getHeight();
        
        RelativeLayout.LayoutParams viewParams = new RelativeLayout.LayoutParams( screenWidth, screenHeight );
        viewParams.leftMargin = 0;
        viewParams.topMargin = 0;
    	setLayoutParams( viewParams );
    	
        holder = getHolder();
        holder.addCallback(this);
        holder.setType( SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS );
        
        file = inFile;
    }

    protected void destroy()
    {
        releaseMediaPlayer();
        doCleanUp();
    }

    void play()
    {
        doCleanUp();
        try 
        {
            // Create a new media player and set the listeners
            mMediaPlayer = new MediaPlayer();

            AssetFileDescriptor afd = CCJNI.activity.getAssets().openFd( file ); 
            mMediaPlayer.setDataSource( afd.getFileDescriptor(), afd.getStartOffset(), afd.getLength() );
            
            mMediaPlayer.setLooping( true );
            mMediaPlayer.setDisplay( holder );
            mMediaPlayer.prepare();
            mMediaPlayer.setOnBufferingUpdateListener( this );
            mMediaPlayer.setOnCompletionListener( this );
            mMediaPlayer.setOnPreparedListener( this );
            mMediaPlayer.setOnVideoSizeChangedListener( this );
            mMediaPlayer.setAudioStreamType( AudioManager.STREAM_MUSIC );
        } 
        catch (Exception e) 
        {
            Log.e( CCJNI.library, "error: " + e.getMessage(), e );
        }
    }

    public void onBufferingUpdate(MediaPlayer arg0, int percent) 
    {
        Log.d( CCJNI.library, "onBufferingUpdate percent:" + percent );
    }

    public void onCompletion(MediaPlayer arg0)
    {
        Log.d( CCJNI.library, "onCompletion called" );
    }

    public void onVideoSizeChanged(MediaPlayer mp, int width, int height) 
    {
        Log.v( CCJNI.library, "onVideoSizeChanged called" );
        if( width == 0 || height == 0 )
        {
            Log.e( CCJNI.library, "invalid video width(" + width + ") or height(" + height + ")" );
            return;
        }
        mIsVideoSizeKnown = true;
        mVideoWidth = width;
        mVideoHeight = height;
        if( mIsVideoReadyToBePlayed && mIsVideoSizeKnown ) 
        {
            startVideoPlayback();
        }
    }

    public void onPrepared(MediaPlayer mediaplayer) 
    {
        Log.d( CCJNI.library, "onPrepared called" );
        mIsVideoReadyToBePlayed = true;
        if( mIsVideoReadyToBePlayed && mIsVideoSizeKnown ) 
        {
            startVideoPlayback();
        }
    }

    public void surfaceChanged(SurfaceHolder surfaceholder, int i, int j, int k)
    {
        Log.d( CCJNI.library, "surfaceChanged called" );

    }

    public void surfaceDestroyed(SurfaceHolder surfaceholder) 
    {
        Log.d( CCJNI.library, "surfaceDestroyed called" );
    }


    public void surfaceCreated(SurfaceHolder holder) 
    {
        Log.d( CCJNI.library, "surfaceCreated called");
        play();
    }

    protected void onPause() 
    {
        releaseMediaPlayer();
        doCleanUp();
    }

    private void releaseMediaPlayer() 
    {
        if (mMediaPlayer != null) 
        {
            mMediaPlayer.release();
            mMediaPlayer = null;
        }
    }

    private void doCleanUp() 
    {
        mVideoWidth = 0;
        mVideoHeight = 0;
        mIsVideoReadyToBePlayed = false;
        mIsVideoSizeKnown = false;
    }

    private void startVideoPlayback() 
    {
        Log.v( CCJNI.library, "startVideoPlayback" );
        holder.setFixedSize( mVideoWidth, mVideoHeight );
        mMediaPlayer.start();
    }
}
