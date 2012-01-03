/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCGLView.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QGesture>

#include "CCGLView.h"


CCGLView::CCGLView(QWidget *parent) :
    QGLWidget( parent )
    //renderThread( this )
{
    // Windows cannot handle this awesomeness
#if !defined( Q_OS_WIN )
    QGLFormat glFormat;
    glFormat.setSampleBuffers( true );
    glFormat.setSwapInterval( 1 );
    setFormat( glFormat );
#endif

    setAttribute( Qt::WA_AcceptTouchEvents );
    handlingTouchEvent = false;

    gView = this;
    runningGame = paused = engineThreadRunning = false;

    //setAutoBufferSwap(false);
    //doneCurrent();
    //RenderThread.start();
    timer.start();
}


CCGLView::~CCGLView()
{
}


void CCGLView::initializeGL()
{
    QGLWidget::initializeGL();

    gEngine = new CCAppEngine();
    gEngine->setupNativeThread();
    gEngine->setupEngineThread();
    runningGame = true;
    engineThreadRunning = true;
}


void CCGLView::resizeGL(int width, int height)
{
    gEngine->renderer->setupScreenSizeParams();
    gEngine->renderer->setOrientation( 0.0f, true );
    gEngine->refreshCameras();
}


//void CCGLView::resizeEvent(QResizeEvent *evt)
//{
//    RenderThread.resizeViewport( evt->size() );
//}


void CCGLView::resizeView(int x1, int y1, int x2, int y2)
{
    int width = x2 - x1;
    int height = y2 - y1;
    setMaximumSize( width, height );
    setGeometry( x1, y1, x2, y2 );
}


void CCGLView::paintGL()
{
    if( runningGame )
    {
        gEngine->updateNativeThread();
        if( paused )
        {
            // Sleep at 20 fps
            usleep( 50000 );
        }
        else
        {
            gEngine->updateEngineThread();
        }

        // Queue another update
        //updateGL();
        update();
    }
}


void CCGLView::shutdown()
{
    //RenderThread.stop();

    delete gEngine;
    gEngine = NULL;
}


bool CCGLView::event(QEvent *event)
{
    return QGLWidget::event( event );
}


void CCGLView::mousePressEvent(QMouseEvent* event)
{
}


void CCGLView::mouseMoveEvent(QMouseEvent* event)
{
}


void CCGLView::mouseReleaseEvent(QMouseEvent* event)
{
}
