/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCRenerable.h
 * Description : A renderable component.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCRENDERABLE_H__
#define __CCRENDERABLE_H__


class CCRenderable : public CCBaseType
{
public:
    typedef CCBaseType super;
	
    CCRenderable();

    // CCBaseType
    virtual void destruct();
    
    virtual void render(const bool alpha) = 0;

public:
    bool shouldRender;
};


#endif // __CCRENDERABLE_H__
