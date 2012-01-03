/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCRenderThread.h
 * Description : Experimental run of the engine on another thread.
 *               Currently not used.
 *
 * Created     : 14/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCRENDERTHREAD_H__
#define __CCRENDERTHREAD_H__


#include <QThread>

class CCGLView;
class QSize;

class QGLRenderThread : public QThread
{
    Q_OBJECT

public:
    explicit QGLRenderThread(CCGLView *parent = NULL);
    void resizeViewport(const QSize &size);
    void run();

private:
    int w, h;

    CCGLView *glView;
};


#endif // __CCRENDERTHREAD_H__
