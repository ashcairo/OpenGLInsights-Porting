/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCCameraAppUI.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"


CCCameraAppUI::CCCameraAppUI()
{
    offsetInterpolator.setDuration( 0.3f );
}


void CCCameraAppUI::update()
{
    super::update();
}


const bool CCCameraAppUI::interpolateCamera(const float delta, const float speed)
{
	if( updating )
	{   
        updating = false;
        
        if( currentOffsetTarget.target != targetOffset )
        {
            currentOffsetTarget.target = targetOffset;
            offsetInterpolator.setup( &currentOffsetTarget.current, currentOffsetTarget.target );
        }
        
        if( currentLookAtTarget.target != targetLookAt )
        {
            currentLookAtTarget.target = targetLookAt;
            lookAtInterpolator.setup( &currentLookAtTarget.current, currentLookAtTarget.target );
        }
        
        if( offsetInterpolator.update( delta ) )
        {
            refreshCameraSize();
            updating = true;
        }
        
        if( lookAtInterpolator.update( delta * speed ) )
        {
            updating = true;
            updatedPosition = true;
        }
        
        // Now update our camera
        {
            lookAt = currentLookAtTarget.current;
            updateOffset( currentOffsetTarget.current );
        }
        return true;
	}

	return false;
}


void CCCameraAppUI::setOffset(const CCVector3 &offsetTarget)
{
    currentOffsetTarget.current = currentOffsetTarget.target = offsetTarget;
    updateOffset( offsetTarget );
}


void CCCameraAppUI::setCameraWidth(const float inWidth)
{
    cameraWidth = inWidth;
    targetOffset.z = cameraWidth;
    if( gEngine->renderer->isPortrait() )
    {
        targetOffset.z /= getFrustumSize().width;
        cameraHeight = targetOffset.z * getFrustumSize().height;
    }
    else
    {
        targetOffset.z /= getFrustumSize().height;
        cameraHeight = targetOffset.z * getFrustumSize().width;
    }
    
    cameraHWidth = cameraWidth * 0.5f;
    cameraHHeight = cameraHeight * 0.5f;
}


void CCCameraAppUI::setCameraHeight(const float inHeight)
{
    cameraHeight = inHeight;
    targetOffset.z = cameraHeight;
    if( gEngine->renderer->isPortrait() )
    {
        targetOffset.z /= getFrustumSize().height;
    }
    else
    {
        targetOffset.z /= getFrustumSize().width;
    }
    
    cameraWidth = targetOffset.z * getFrustumSize().height;
    cameraHWidth = cameraWidth * 0.5f;
    cameraHHeight = cameraHeight * 0.5f;
}


void CCCameraAppUI::refreshCameraSize()
{
    CCVector3 &currentOffset = currentOffsetTarget.current;
    if( gEngine->renderer->isPortrait() )
    {
        cameraWidth = currentOffset.z * getFrustumSize().width;
        cameraHeight = currentOffset.z * getFrustumSize().height;
    }
    else
    {
        cameraWidth = currentOffset.z * getFrustumSize().height;
        cameraHeight = currentOffset.z * getFrustumSize().width;
    }
    
    cameraHWidth = cameraWidth * 0.5f;
    cameraHHeight = cameraHeight * 0.5f;
}