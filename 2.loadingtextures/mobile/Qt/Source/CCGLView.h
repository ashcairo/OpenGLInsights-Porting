/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCGLView.h
 * Description : OpenGL view.
 *
 * Created     : 14/06/11
 * Author(s)   : Jerome Portier, Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCGLVIEW_H__
#define __CCGLVIEW_H__


#include <QtGui>
#include <QtOpenGL/QGLWidget>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include "CCMainWindow.h"
#include "CCRenderThread.h"

extern class CCGLView *gView;

class CCGraphicsView : public QGraphicsView, public MainWindowChild
{
public:
    CCGraphicsView(QGraphicsScene *scene, QWidget *parent) :
        QGraphicsView( scene, parent)
    {
    }

    void resizeView(int x1, int y1, int x2, int y2)
    {
        int width = x2 - x1;
        int height = y2 - y1;
        setMaximumSize( width, height );
        setGeometry( x1, y1, x2, y2 );
    }
};

class CCGLView : public QGLWidget, public MainWindowChild
{
    Q_OBJECT
	
public:
    explicit CCGLView(QWidget *parent);
    ~CCGLView();

    // QGLWidget stuff
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    // CCMainWindow
    void resizeView(int x1, int y1, int x2, int y2);

public:
    void shutdown();

protected:
    bool event(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public:
    bool runningGame, paused, engineThreadRunning;
    bool handlingTouchEvent;

    QTime timer;
    //QGLRenderThread renderThread;
};


#endif // __CCGLVIEW_H__
