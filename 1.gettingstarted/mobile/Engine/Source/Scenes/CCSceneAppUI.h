/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCSceneAppUI.h
 * Description : AppUI scene template.
 *
 * Created     : 06/06/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

class CCTile3D;

class CCSceneAppUI : public CCSceneBase, public virtual CCLazyCallback
{
public:
    typedef CCSceneBase super;

	CCSceneAppUI();
	virtual void destruct();
    
    // CCSceneBase
	virtual void setup();
    
protected:
	virtual void updateScene(const CCTime &gameTime);
};
