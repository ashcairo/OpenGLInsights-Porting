/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceControls.h
 * Description : Handles Android specific controls.
 *
 * Created     : 15/05/11
 * Author(s)   : Chris Bowers, Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCDEVICECONTROLS_H__
#define __CCDEVICECONTROLS_H__

struct UITouch
{
	CCPoint pos;
};

#include "CCControls.h"

class CCDeviceControls : public CCControls
{
public:
	CCDeviceControls();

	void touch(CCPoint position, int action, int finger);

	void touchBegin( UITouch *touch );
	void touchMove( UITouch *touch );
	void touchEnd( UITouch *touch );
	
protected:
	void touchHandle( UITouch *touch );

protected:
	UITouch deviceTouches[numberOfTouches];
};


#endif // __CCDEVICECONTROLS_H__
