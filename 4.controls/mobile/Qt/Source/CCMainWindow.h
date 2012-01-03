/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCMainWindow.h
 * Description : Qt View controller.
 *
 * Created     : 23/07/11
 * Author(s)   : Jerome Portier, Ashraf Samy Hegab
 *-----------------------------------------------------------
 */


#ifndef __CCMAINWINDOW_H__
#define __CCMAINWINDOW_H__

#include <QtGui/QMainWindow>

#include "CCDefines.h"
class CCViewManager;

class MainWindowChild
{
public:
    virtual ~MainWindowChild() {}
    virtual void resizeView(int x1, int y1, int x2, int y2) = 0;
};

class CCMainWindow : public QMainWindow
{
	Q_OBJECT

public:
    CCMainWindow(QWidget *parent=0);
    ~CCMainWindow();
	
private:
    void resizeEvent(QResizeEvent *event);

public:
    void addChild(MainWindowChild *child);
    void removeChild(MainWindowChild *child);

private:
    CCList<MainWindowChild> children;

public:
    static CCMainWindow *instance;
    CCViewManager *viewManager;
};

#endif // __CCMAINWINDOW_H__
