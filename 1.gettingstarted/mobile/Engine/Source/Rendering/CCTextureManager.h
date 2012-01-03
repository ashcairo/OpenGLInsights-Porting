/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCTextureManager.h
 * Description : Manages the loading and setting of textures.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCTEXTUREMANAGER_H__
#define __CCTEXTUREMANAGER_H__


class CCTextureBase;
class CCTextureFontPage;

struct CCTextureHandle
{
    CCText filePath;
    CCResourceType resourceType;
	CCTextureBase *texture;
    bool loadable;
	bool alwaysResident;
    bool mipmap;
    float lastTimeUsed;
    LAMBDA_SIGNAL onLoad;
	
    CCTextureHandle(const char *inFilePath, const CCResourceType inResourceType)
    {
        filePath = inFilePath;
        resourceType = inResourceType;
		texture = NULL;
        loadable = true;
		alwaysResident = false;
        mipmap = false;
        lastTimeUsed = 0.0f;
	}
	
	~CCTextureHandle();
};


class CCTextureManager
{
public:
	CCTextureManager();
	~CCTextureManager();

    void prepareRender();
	void load();
    
    const uint assignTextureIndex(const char *filePath, const CCResourceType resourceType, 
                                  const bool alwaysResident, const bool mipmap);
    const bool loadTexture(CCTextureHandle &textureHandle);
	
    // Used for direct OpenGL access binding
	void bindTexture(const uint glName=0);
    
    // Used for assignging textures
	const bool setTextureIndex(const int textureIndex);
	
    CCTextureHandle* getTextureHandle(const int handleIndex);
    const CCTextureBase *getTexture(const int handleIndex);
    const CCTextureBase *getTexture(const char *filePath, const CCResourceType resourceType);
	
protected:
	uint currentTextureHandle;
    uint totalTexturesLoaded;
    uint totalTextureSpace;
    uint totalTexturesLoadedThisFrame;
    float textureLoadingTimeout;
	CCList<CCTextureHandle> textureHandles;
};


#endif // __CCTEXTUREMANAGER_H__
