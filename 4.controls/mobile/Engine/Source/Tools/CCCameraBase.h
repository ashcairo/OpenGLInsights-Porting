/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCCameraBase.h
 * Description : Common functionality for scene cameras.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCCAMERABASE_H__
#define __CCCAMERABASE_H__


#include "CCVectors.h"
#include "CCMatrix.h"

enum CCCameraFrustumPlanes
{
	frustum_right,
	frustum_left,
	frustum_bottom,
	frustum_top,
	frustum_far,
	frustum_near,
	frustum_max,
};

struct CCCameraProjectionResults
{
    CCVector3 vNear;
    CCVector3 vFar;
    CCVector3 vDirection;
    CCVector3 vLookAt;
};

class CCCameraBase
{
public:
	CCCameraBase();

    void setupViewport(const CCCameraBase *inCamera);
    void setupViewport(const float x1=0.0f, const float y1=0.0f, const float x2=1.0f, const float y2=1.0f);
    void refreshViewport();
    void setViewport();
    void setNearFar(const float zNear, const float zFar);
	
    const bool project3D(float x, float y);
    void project3DY(CCVector3 *result, float x, float y, float offset=-1.0f);
    void project3DZ(CCVector3 *result, float x, float y, float offset=-1.0f);
    
    virtual void scanFOVBounds();
	struct FOVScan
    {
        CCPoint min;
        CCPoint max;
        CCPoint size;
	};
	enum FOVScanTypes
	{
		scan_flat,
		max_fov_scans
	};
	FOVScan fovScans[max_fov_scans];
	
	virtual void update();
	
	// Update lookAt and offset
    const bool setLookAt(const CCVector3 &lookAtTarget, const CCVector3 &offsetTarget);
	const CCVector3& getLookAt() { return lookAt; }
    
    inline const CCSize& getFrustumSize() { return frustumSize; }
    inline CCVector3& getRotationVector() { return rotation; }
    
    virtual void setRotationX(const float inRotation);
    virtual void setRotationY(const float inRotation);
    virtual void setRotationZ(const float inRotation);
    void incrementRotationX(const float increment);
    void incrementRotationY(const float increment);
    void incrementRotationZ(const float increment);
	
    const bool isUpdating() { return updating; }
	void flagUpdate() { updating = true; }
    
    inline void setIndex(const uint inIndex) { index = inIndex; }
    inline const uint getIndex() const { return index; }
	
	const CCVector3 getDirection();
	
    inline const CCVector3& getRotatedPosition() { return rotatedPosition; }
	inline const float getRotation() { return rotation.y; }
	
	inline CCMatrix& getModelViewMatrix() { return modelViewMatrix; }

    void updateControls();
	
    const CCVector3& getOffset() { return offset; }
    virtual void setOffset(const CCVector3 &offsetTarget);
protected:
	void updateOffset(const CCVector3 &offsetTarget);
    
public:
    // Don't use the octrees for scanning the visible objects
    // Optimization if you know what objects the camera will be drawing
    void assignVisibleObjects(CCList<CCSceneCollideable> *inVisiblesList);
protected:
	void updateVisibleObjects();
    
    // glu-based camera functionality
	void GluPerspective(float fovy, float aspect);
    
    // Orient camera matrix to look at
    void GluLookAt(CCMatrix &modelViewMatrix, 
                   float eyex, float eyey, float eyez,			   
                   float centerx, float centery, float centerz,
                   float upx, float upy, float upz);
    
    // Unproject a 2d screen space point into 3d space
    const bool GluUnProject(const float x, const float y, const float z, CCVector3 &result);
    
    // Project a 3d point into 2d screen space
    const bool GluProject(class CCRenderable *object, CCVector3 &result);
    
    void ExtractFrustum();
	
public:
	bool enabled;
    float aspectRatio;
    int viewport[4];
    float cameraX, cameraY, cameraW, cameraH, invCameraW, invCameraH;
	CCMatrix projectionMatrix;
    
    CCCameraProjectionResults projection;
    CCScreenTouches cameraTouches[CCControls::numberOfTouches];
    
    // Has the camera been updated this frame
    bool updatedPosition;
	
protected:
    uint index;
    bool updating;
	bool updateFOV;
	
    float zNear, zFar;
	
	CCVector3 offset;
	CCVector3 position;
	CCVector3 rotatedPosition;
	
	CCVector3 lookAt;
	CCVector3 rotation;
	
	CCMatrix modelViewMatrix;
	float frustum[frustum_max][4];
    
    CCPoint frustumMin, frustumMax;
    CCSize frustumSize;
    
    // List of objects that the camera will render
    CCList<CCSceneCollideable> *visiblesList;
};


#endif // __CCCAMERABASE_H__
