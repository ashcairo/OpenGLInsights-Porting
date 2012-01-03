/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTile3D.h
 * Description : Base class for our Tile widgets.
 *
 * Created     : 14/09/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCTILE3D_H__
#define __CCTILE3D_H__


class CCControllerMovement;

class CCTile3D : public CCSceneCollideable, public virtual CCLazyCallback
{
public:
    typedef CCSceneCollideable super;

    CCTile3D();

    // Objects which move along with this tile, but contain handle their own collisions
    CCList<CCSceneObject> attachments;
};


#endif // __CCTILE3D_H__
