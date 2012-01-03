/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright ﾩ 2010 ￐ 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCGLViewJNI.java
 * Description : Creates a single threaded OpenGL view for NDK debugging on older devices.
 * 				 Based off APIDemos sample
 * 				 http://developer.android.com/guide/samples/ApiDemos/src/com/example/android/apis/graphics/index.html
 *
 * Created     : 27/11/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

package com.android2c;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.opengles.GL11;

import android.content.Context;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;


public class CCGLViewJNIDebug extends SurfaceView implements SurfaceHolder.Callback 
{
	private SurfaceHolder mHolder;
	public static GL11 glContext;

    private int mWidth;
    private int mHeight;
    private Renderer mRenderer;
    private EglHelper mEglHelper;
    
	Handler updateHandler = new Handler();
    boolean recreateSurface = false;
    boolean surfaceCreated = false;
    
    public boolean onTouchEvent(final MotionEvent event) 
    {
    	int action = event.getAction() & MotionEvent.ACTION_MASK;
    	int index = ( event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK ) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
    	int finger = event.getPointerId( index );
    	float x = event.getX();
    	float y = event.getY();
    	
		CCJNI.controlsHandleTouch( x, y, action, finger );
        return true;
    }
    
	private static class CCRenderer implements CCGLViewJNIDebug.Renderer 
    {
		public int[] getConfigSpec()
		{
	        // We don't need a depth buffer, and don't care about our color depth.
	        int[] configSpec = { EGL10.EGL_DEPTH_SIZE, 0, EGL10.EGL_NONE };
	        return configSpec;
	    }

		public void surfaceCreated(GL10 gl)
		{
		}

		public void shutdown(GL10 gl) 
		{
		}

		public void sizeChanged(GL10 gl, int width, int height) 
		{
			glContext = (GL11)gl;
            CCJNI.onSurfaceChanged( width, height );
		}

		public void drawFrame(GL10 gl)
		{
			glContext = (GL11)gl;
        	CCJNI.onDrawFrame();
		}
    }
	
    public CCGLViewJNIDebug(Context context) 
    {
        super( context );

        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed
        mHolder = getHolder();
        mHolder.addCallback( this );
        mHolder.setType( SurfaceHolder.SURFACE_TYPE_GPU );
        
        mWidth = 0;
        mHeight = 0;
        
    	mEglHelper = new EglHelper();
    	mRenderer = new CCRenderer();
    	
    	toggleUpdate();
    }

    public void surfaceCreated(SurfaceHolder holder) 
    {
    }

    public void surfaceDestroyed(SurfaceHolder holder) 
    {
        // Surface will be destroyed when we return
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h)
    {
        // Surface size or format has changed.
    	mWidth = w;
        mHeight = h;
        recreateSurface = true;
        setup();
    }
    
    public void onPause()
    {
         mEglHelper.finish();
         surfaceCreated = false;
    }

    public void onResume() 
    {
        recreateSurface = true;
        setup();
    }

    // Inform the view that the window focus has changed.
    @Override 
    public void onWindowFocusChanged(boolean hasFocus) 
    {
        super.onWindowFocusChanged( hasFocus );
    }

    @Override
    protected void onDetachedFromWindow()
    {
        super.onDetachedFromWindow();

        if( glContext != null ) 
        {
            mRenderer.shutdown( glContext );
        }
        mEglHelper.finish();
    }
    
    public void setup() 
    {
    	if( mWidth > 0 && mHeight > 0 )
    	{
    		if( recreateSurface )
    		{
    			recreateSurface = false;
	        	int[] configSpec = mRenderer.getConfigSpec();
	            mEglHelper.start(configSpec);
	            
	            glContext = (GL11)mEglHelper.createSurface(mHolder);
	            mRenderer.sizeChanged( glContext, mWidth, mHeight );
	            
	            surfaceCreated = true;
    		}
    	}
    }

    private void toggleUpdate()
    {
		final Runnable r = new Runnable()
		{
		    public void run() 
		    {
		    	toggleUpdate();
		    }
		};
		updateHandler.postDelayed( r, 50 );
		update();
    }
    
    private void update()
    {
    	if( surfaceCreated )
    	{
	        mRenderer.drawFrame( glContext );
	                
	        /*
	         * Once we're done with GL, we need to call swapBuffers()
	         * to instruct the system to display the rendered frame
	         */
	        mEglHelper.swap();
    	}
    }

    // ----------------------------------------------------------------------

    // A generic renderer interface.
    public interface Renderer 
    {
        /**
         * @return the EGL configuration specification desired by the renderer.
         */
        int[] getConfigSpec();

        /**
         * Surface created.
         * Called when the surface is created. Called when the application
         * starts, and whenever the GPU is reinitialized. This will
         * typically happen when the device awakes after going to sleep.
         * Set your textures here.
         */
        void surfaceCreated(GL10 gl);
        
        /**
         * Called when the rendering thread is about to shut down.  This is a
         * good place to release OpenGL ES resources (textures, buffers, etc).
         * @param gl
         */
        void shutdown(GL10 gl);
        
        /**
         * Surface changed size.
         * Called after the surface is created and whenever
         * the OpenGL ES surface size changes. Set your viewport here.
         * @param gl
         * @param width
         * @param height
         */
        void sizeChanged(GL10 gl, int width, int height);
        /**
         * Draw the current frame.
         * @param gl
         */
        void drawFrame(GL10 gl);
    }

    // An EGL helper class.
    private class EglHelper 
    {
        public EglHelper()
        {
        }

        /**
         * Initialize EGL for a given configuration spec.
         * @param configSpec
         */
        private static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        public void start(int[] configSpec)
        {
            /*
             * Get an EGL instance
             */
            mEgl = (EGL10)EGLContext.getEGL();

            /*
             * Get to the default display.
             */
            mEglDisplay = mEgl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);

            /*
             * We can now initialize EGL for that display
             */
            int[] version = new int[2];
            mEgl.eglInitialize( mEglDisplay, version );

            EGLConfig[] configs = new EGLConfig[1];
            int[] num_config = new int[1];
            mEgl.eglChooseConfig( mEglDisplay, configSpec, configs, 1, num_config );
            mEglConfig = configs[0];

            /*
            * Create an OpenGL ES context. This must be done only once, an
            * OpenGL context is a somewhat heavy object.
            */
            int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
            mEglContext = mEgl.eglCreateContext( mEglDisplay, mEglConfig, EGL10.EGL_NO_CONTEXT, attrib_list );
            CCGLViewJNI.checkEglError( "After eglCreateContext", mEgl );

            mEglSurface = null;
        }

        /*
         * Create and return an OpenGL surface
         */
        public GL createSurface(SurfaceHolder holder) 
        {
            /*
             *  The window size has changed, so we need to create a new
             *  surface.
             */
            if( mEglSurface != null )
            {
                /*
                 * Unbind and destroy the old EGL surface, if
                 * there is one.
                 */
                mEgl.eglMakeCurrent( mEglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT );
                mEgl.eglDestroySurface( mEglDisplay, mEglSurface );
            }

            /*
             * Create an EGL surface we can render into.
             */
            mEglSurface = mEgl.eglCreateWindowSurface( mEglDisplay, mEglConfig, holder, null );

            /*
             * Before we can issue GL commands, we need to make sure
             * the context is current and bound to a surface.
             */
            mEgl.eglMakeCurrent( mEglDisplay, mEglSurface, mEglSurface, mEglContext );


            GL gl = mEglContext.getGL();
            return gl;
        }

        /**
         * Display the current render surface.
         * @return false if the context has been lost.
         */
        public boolean swap() 
        {
            mEgl.eglSwapBuffers(mEglDisplay, mEglSurface);

            /*
             * Always check for EGL_CONTEXT_LOST, which means the context
             * and all associated data were lost (For instance because
             * the device went to sleep). We need to sleep until we
             * get a new surface.
             */
            return mEgl.eglGetError() != EGL11.EGL_CONTEXT_LOST;
        }

        public void finish() 
        {
            if( mEglSurface != null )
            {
                mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
                mEgl.eglDestroySurface(mEglDisplay, mEglSurface);
                mEglSurface = null;
            }
            if( mEglContext != null ) 
            {
                mEgl.eglDestroyContext(mEglDisplay, mEglContext);
                mEglContext = null;
            }
            if( mEglDisplay != null ) 
            {
                mEgl.eglTerminate(mEglDisplay);
                mEglDisplay = null;
            }
        }

        EGL10 mEgl = null;
        EGLDisplay mEglDisplay = null;
        EGLSurface mEglSurface = null;
        EGLConfig mEglConfig = null;
        EGLContext mEglContext = null;
    }
}
