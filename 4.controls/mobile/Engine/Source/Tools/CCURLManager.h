/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCURLManager.h
 * Description : Manager for HTTP requests.
 *
 * Created     : 02/06/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCURLMANAGER_H__
#define __CCURLMANAGER_H__


class CCURLCallback : public CCLambdaCallback
{
public:
    CCURLCallback()
    {
        reply = NULL;
    }
    struct CCURLRequest *reply;
};

struct CCURLRequest
{
	enum RequestState 
    {
		not_started,
        in_flight,
		failed,
        timed_out,
        data_error,
        succeeded,
        cached
	};
	
	CCURLRequest()
	{
        state = CCURLRequest::not_started;
        priority = 0;
        checkCache = true;
        cacheChecked = false;
        timeout = 0.0f;
        timeRequested = -1.0f;
	}

    ~CCURLRequest()
    {
        onComplete.deleteObjectsAndList();
    }
	
    CCText url;                   // The URL to request
    int priority;
    LAMBDA_SIGNAL onComplete;   // The on complete callback
    RequestState state;         // The state of the request
    CCText cacheFile;             // Cache the reply?
    bool checkCache;            // Should we check the cahce?
    bool cacheChecked;          // Have we checked the cache?
    float timeout;              // The time we can process this request
    float timeRequested;        // The time when the request can be and was made
    CCData data;

    CCPairList<CCText, CCText> header;
};


class CCDeviceURLManager;

class CCURLManager
{
public:
    CCURLManager();
    ~CCURLManager();
    
    void updateEngineThread();
    void updateNativeThread();
    
    void flushPendingRequests();
     
    void requestURL(const char *url,                    // The URL to request
                    CCURLCallback *inCallback=NULL,     // The on complete callback
                    const int priority=0,               // The priority
                    const char *cacheFile=NULL,         // The file to save the result to
                    const bool checkCache=true,         // Check for existing cache file?
                    const float timeout=0.0f);          // Length of time to wait before requesting

protected:
    void finishURL(CCURLRequest *request);

public:
    void setDomainTimeOut(const char *domain, float timeout);
    const bool processingHighPriority() { return highPriorityRequestsPending; }

protected:
    const bool readyToRequest();
    
public:
    CCDeviceURLManager *deviceURLManager;
    
protected:
    CCList<CCURLRequest> currentRequests;
    CCList<CCURLRequest> requestQueue;
    bool highPriorityRequestsPending;
    
    struct DomainTimeOut
    {
        DomainTimeOut()
        {
            lastRequested = 0.0f;
        }
        CCText name;
        float timeout;
        float lastRequested;
    };
    CCList<DomainTimeOut> domainTimeOuts;
};


#endif // __CCURLMANAGER_H__
