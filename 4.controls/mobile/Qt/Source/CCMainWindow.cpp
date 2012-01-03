/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCMainWindow.cpp
 *-----------------------------------------------------------
 */

#include "CCMainWindow.h"
#include "CCViewManager.h"


CCMainWindow *CCMainWindow::instance = NULL;


CCMainWindow::CCMainWindow(QWidget *parent)
    : QMainWindow( parent )
{
    instance = this;
    viewManager = new CCViewManager();
    setWindowTitle( "2c" );
}


CCMainWindow::~CCMainWindow()
{
    viewManager->shutdown();
    children.deleteObjectsAndList();
}


void CCMainWindow::resizeEvent(QResizeEvent *event)
{
    if( children.length == 0 )
    {
        viewManager->startup();
        //children.add( gView );
        return;
    }

    const int windowWidth = width();
    const int windowHeight = height();
    for( int i=0; i<children.length; ++i )
    {
        children.list[i]->resizeView( 0, 0, windowWidth, windowHeight );
    }
}


void CCMainWindow::addChild(MainWindowChild *child)
{
    children.add( child );
    const int windowWidth = width();
    const int windowHeight = height();
    child->resizeView( 0, 0, windowWidth, windowHeight );
}


void CCMainWindow::removeChild(MainWindowChild *child)
{
    children.remove( child );
}
