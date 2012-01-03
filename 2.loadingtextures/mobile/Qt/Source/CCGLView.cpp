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
#include "CCDeviceControls.h"


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
    if( gEngine != NULL )
    {
        CCDeviceControls *controls = (CCDeviceControls*)gEngine->controls;

        QEvent::Type eventType = event->type();
        if( eventType != QEvent::Paint &&
            eventType != QEvent::UpdateLater &&
            eventType != QEvent::WindowActivate  &&
            eventType != QEvent::WindowDeactivate  &&
            eventType != QEvent::Enter &&
            eventType != QEvent::Leave &&
            eventType != QEvent::ToolTip )
        {
            if( eventType == QEvent::TouchBegin )
            {
                handlingTouchEvent = true;
                event->accept();
                QTouchEvent *touchEvent = static_cast<QTouchEvent*>( event );
                controls->touchBegin( touchEvent->touchPoints() );
                return true;
            }
            else if( eventType == QEvent::TouchUpdate )
            {
                event->accept();
                QTouchEvent *touchEvent = static_cast<QTouchEvent*>( event );
                controls->touchMove( touchEvent->touchPoints() );
                return true;
            }
            else if( eventType == QEvent::TouchEnd )
            {
                event->accept();
                QTouchEvent *touchEvent = static_cast<QTouchEvent*>( event );
                controls->touchEnd( touchEvent->touchPoints() );

                handlingTouchEvent = false;
                return true;
            }
        }
    }
    return QGLWidget::event( event );
}


static UITouch mouseTouch;
void CCGLView::mousePressEvent(QMouseEvent* event)
{
    CCDeviceControls *controls = (CCDeviceControls*)gEngine->controls;
    if( handlingTouchEvent == false )
    {
        QPointF posf = event->posF();
        mouseTouch.pos = CCPoint( posf.x(), posf.y() );
        controls->touchBegin( &mouseTouch );
    }
}


void CCGLView::mouseMoveEvent(QMouseEvent* event)
{
    CCDeviceControls *controls = (CCDeviceControls*)gEngine->controls;
    if( handlingTouchEvent == false )
    {
        QPointF posf = event->posF();
        mouseTouch.pos = CCPoint( posf.x(), posf.y() );
        controls->touchMove( &mouseTouch );
    }
}


void CCGLView::mouseReleaseEvent(QMouseEvent* event)
{
    CCDeviceControls *controls = (CCDeviceControls*)gEngine->controls;
    if( handlingTouchEvent == false )
    {
        QPointF posf = event->posF();
        mouseTouch.pos = CCPoint( posf.x(), posf.y() );
        controls->touchEnd( &mouseTouch );
    }
}
