/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceControls.h
 * Description : Qt specific controls interfaces.
 *
 * Created     : 27/04/10
 * Author(s)   : Jerome Portier, Ashraf Samy Hegab, Chris Wilson
 *-----------------------------------------------------------
 */

#ifndef __CCDEVICECONTROLS_H__
#define __CCDEVICECONTROLS_H__

#include <QTouchEvent>
struct UITouch
{
    UITouch()
    {
        touchID = -1;
    }

    CCPoint pos;
    int touchID;
};

#include "CCControls.h"

class CCDeviceControls : public CCControls
{
public:
    CCDeviceControls();
	
    void touchBegin(UITouch *touch);
    void touchMove(UITouch *touch);
    void touchEnd(UITouch *touch);
	
protected:
    void touchHandle(UITouch *touch);

public:
    void touchBegin(QList<QTouchEvent::TouchPoint> touches);
    void touchMove(QList<QTouchEvent::TouchPoint> touches);
    void touchEnd(QList<QTouchEvent::TouchPoint> touches);

protected:
    void touchHandle(QList<QTouchEvent::TouchPoint> touches);

protected:
    UITouch* getUnusedUITouch();
    UITouch uiTouches[numberOfTouches];
};


#endif // __CCDEVICECONTROLS_H__
