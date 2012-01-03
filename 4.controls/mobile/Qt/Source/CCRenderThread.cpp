/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCRenderThread.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCGLView.h"
#include "CCRenderThread.h"


QGLRenderThread::QGLRenderThread(CCGLView *parent) :
    QThread(),
    glView( parent )
{
}

void QGLRenderThread::resizeViewport(const QSize &size)
{
    w = size.width();
    h = size.height();
}


void QGLRenderThread::run()
{
    glView->makeCurrent();

    gEngine = new CCAppEngine();
    gEngine->setupNativeThread();
    gEngine->setupEngineThread();

    do
    {
        gEngine->updateNativeThread();
        gEngine->updateEngineThread();

        glView->swapBuffers();
    } while( glView->runningGame );
}
