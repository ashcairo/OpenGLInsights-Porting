/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCFileManager.h
 * Description : Handles loading files between platforms.
 *
 * Created     : 11/05/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCFILEMANAGER_H_
#define __CCFILEMANAGER_H_


#include "CCBaseTypes.h"
#include "CCCallbacks.h"
#include <sys/stat.h>

class CCIOCallback : public CCLambdaCallback
{
public:
    CCIOCallback(const int inPriority)
    {
        priority = inPriority;
        exists = false;
    }
    
    // Run this before IO is performed to verify the lambda of the call is still active
    virtual const bool isCallbackActive() = 0;
    
    CCText filePath;
    int priority;
    bool exists;
};


class CCFileManager
{
public:
    // data pointer must be freed with a call to FREE_POINTER
    static const int getFileData(const char *fullFilePath, char **data, CCResourceType resourceType, const bool assertOnFail=true);
    
    static void getFilePath(CCText &fullFilePath, const char *filePath, CCResourceType resourceType);

    static const int getPackagedFile(const char *filePath, char **data, const bool assertOnFail=true, struct stat *info=NULL);

    static const int getCachedFile(const char *filePath, char **data, const bool assertOnFail=true, struct stat *info=NULL);
    static const bool saveCachedFile(const char *filePath, const char *data, const int length);
    static const bool deleteCachedFile(const char *filePath);

    static void readyIO();
    static void doesCachedFileExistAsync(const char *filePath, CCIOCallback *inCallback);
    static const bool doesCachedFileExist(const char *filePath);
};


#endif // __CCFILEMANAGER_H_
