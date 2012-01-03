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

    // CCRenderable
    virtual void dirtyModelMatrix();
    virtual void setPositionXYZ(const float x, const float y, const float z);
    virtual void translate(const float x, const float y, const float z);
    
    // Positioning Tiles
    void positionTileY(float &y);
    virtual void positionTileBelow(CCTile3D *fromTile);
    void positionTileAbove(CCTile3D *fromTile);
    void positionTileRight(CCTile3D *fromTile);

    // Objects which move along with this tile, but contain handle their own collisions
    CCList<CCSceneObject> attachments;
    CCControllerMovement *movementController;
};


#endif // __CCTILE3D_H__
