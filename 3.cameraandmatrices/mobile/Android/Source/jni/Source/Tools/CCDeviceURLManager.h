/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceURLManager.h
 * Description : Android specific url manager.
 *
 * Created     : 09/06/11
 * Author(s)   : Ashraf Samy Hegab, Chris Bowers
 *-----------------------------------------------------------
 */

#ifndef __CCDEVICEURLMANAGER_H__
#define __CCDEVICEURLMANAGER_H__

struct CCURLRequest;

class CCDeviceURLManager
{
public:
    CCDeviceURLManager();
    void processRequest(CCURLRequest *inRequest);
    void clear();
    void downloadFinished(const char *url, const bool success,
    		const char *data, const int length,
    		CCList<CCText> &headerNames, CCList<CCText> &headerValues);
    const bool readyToRequest();

protected:
    CCList<CCURLRequest> currentRequests;
};

#endif // __CCDEVICEURLMANAGER_H__
