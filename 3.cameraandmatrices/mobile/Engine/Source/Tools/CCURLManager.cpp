/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCURLManager.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCDeviceURLManager.h"
#include "CCFileManager.h"
#include <time.h>


CCURLManager::CCURLManager()
{
    deviceURLManager = new CCDeviceURLManager();
    
    highPriorityRequestsPending = false;
}


CCURLManager::~CCURLManager()
{
    flushPendingRequests();

    DELETE_POINTER( deviceURLManager );
    
    domainTimeOuts.deleteObjectsAndList();
}


void CCURLManager::updateEngineThread()
{
    if( currentRequests.length > 0 )
    {
        CCEngineThreadLock();
        for( int i=0; i<currentRequests.length; ++i )
        {
            CCURLRequest *currentRequest = currentRequests.list[i];
            if( currentRequest->state >= CCURLRequest::failed )
            {
                finishURL( currentRequest );
            }
        }
        CCEngineThreadUnlock();
    }
}


void CCURLManager::updateNativeThread()
{
    if( readyToRequest() )
	{
		CCEngineThreadLock();
        
        // Start a request
        // There's three streams going, two for anything, one for higher priority requests
        while( currentRequests.length < 2 && requestQueue.length > 0 )
		{
            CCURLRequest *pendingRequest = requestQueue.list[0];
            ASSERT( pendingRequest->state == CCURLRequest::not_started );
            
            // Don't process any low priority requests if we already have one in progress
            if( currentRequests.length == 1 )
            {
                if( pendingRequest->priority == 0 )
                {
                    break;
                }
            }
            
#ifdef DEBUGON
            
            if( pendingRequest->priority == 0 )
            {
                for( int i=1; i<requestQueue.length; ++i )
                {
                    CCURLRequest *otherRequest = requestQueue.list[i];
                    ASSERT( otherRequest->priority == 0 );
                }
            }
            
#endif
            
            const bool removed = requestQueue.remove( pendingRequest );
            ASSERT( removed );
            currentRequests.add( pendingRequest );
        }
        
        // Either cache or kick off our download if we haven't started our current requests
        for( int i=0; i<currentRequests.length; ++i )
        {
            CCURLRequest *currentRequest = currentRequests.list[i];

#ifdef DEBUGON
            const int requestIndex = requestQueue.find( currentRequest );
            ASSERT( requestIndex == -1 );
#endif

            if( currentRequest->state == CCURLRequest::not_started )
            {
                if( gEngine->gameTime.lifetime < currentRequest->timeout )
                {
                    continue;
                }
                
                if( currentRequest->checkCache )
                {
                    if( currentRequest->cacheChecked == false )
                    {
                        currentRequest->cacheChecked = true;

                        // See if the data has been cached
                        if( currentRequest->cacheFile.length > 0 )
                        {
                            char *fileData = NULL;
                            struct stat fileInfo;
                            const int fileSize = CCFileManager::getCachedFile( currentRequest->cacheFile.buffer, &fileData, false, &fileInfo );
                            if( /*false*/ fileSize > 0 )
                            {
//                                time_t timeNow = time( NULL );
//#if defined Q_OS_WIN || defined ANDROID
//                                time_t timeSince = timeNow - fileInfo.st_mtime;
//#else
//                                time_t timeSince = timeNow - fileInfo.st_mtimespec.tv_sec;
//#endif
                                
                                currentRequest->state = CCURLRequest::cached;
                                currentRequest->data.set( fileData, fileSize );
                                FREE_POINTER( fileData );
                                currentRequest = NULL;
                            }
                        }
                    }
                }

                if( currentRequest != NULL )
                {
                    // Check to see if the url needs to wait for the domain to be ready
                    bool wait = false;
                    for( int i=0; i<domainTimeOuts.length; ++i )
                    {
                        DomainTimeOut *domainTimeOut = domainTimeOuts.list[i];
                        if( CCText::Contains( currentRequest->url.buffer, domainTimeOut->name.buffer ) )
                        {
                            const float nextRequestTime = domainTimeOut->lastRequested + domainTimeOut->timeout;
                            if( gEngine->gameTime.lifetime < nextRequestTime )
                            {
                                wait = true;
                            }
                            break;
                        }
                    }
                    
                    if( wait == false )
                    {
                        currentRequest->timeRequested = gEngine->gameTime.lifetime;
                        deviceURLManager->processRequest( currentRequest );
                        
                        // Record the last request of this domain
                        for( int i=0; i<domainTimeOuts.length; ++i )
                        {
                            DomainTimeOut *domainTimeOut = domainTimeOuts.list[i];
                            if( CCText::Contains( currentRequest->url.buffer, domainTimeOut->name.buffer ) )
                            {
                                domainTimeOut->lastRequested = gEngine->gameTime.lifetime;
                                break;
                            }
                        }
                    }
                }
            }
		}
        
		CCEngineThreadUnlock();
	}
}


void CCURLManager::flushPendingRequests()
{
    deviceURLManager->clear();
    
    // Clean up our request object
    currentRequests.deleteObjects();
    requestQueue.deleteObjects();
}


void CCURLManager::requestURL(const char *url,
                              CCURLCallback *inCallback,
                              const int priority,
                              const char *cacheFile,
                              const bool checkCache,
                              const float timeout)
{
    ASSERT( priority >= 0 && priority <= 2 );
    if( priority > 0 )
    {
        highPriorityRequestsPending = true;
    }

    CCEngineThreadLock();
    
    CCURLRequest *urlRequest = NULL;
    for( int i=0; i<requestQueue.length; ++i )
    {
        CCURLRequest *request = requestQueue.list[i];
        ASSERT( request->url.length < 1000 );
        ASSERT( request->state == CCURLRequest::not_started );
        if( CCText::Equals( request->url.buffer, url ) )
        {
            // TODO: Ignore possible clashes with different cache filenames for now..
            if( checkCache == request->checkCache )
            {
                urlRequest = request;
                break;
            }
        }
    }
	
    if( urlRequest == NULL )
    {
        urlRequest = new CCURLRequest();
        urlRequest->url.set( url );
        urlRequest->priority = priority;
        urlRequest->checkCache = checkCache;
        urlRequest->timeout = gEngine->gameTime.lifetime + timeout;
        
        if( cacheFile != NULL )
        {
            urlRequest->cacheFile = cacheFile;
        }

        // If our priority is 0 push it to the back
        requestQueue.add( urlRequest );

        // If we have a higher priority reinsert it in the right spot
        if( priority > 0 )
        {
            for( int i=0; i<requestQueue.length; ++i )
            {
                CCURLRequest *request = requestQueue.list[i];
                if( request->priority < priority )
                {
#if 0 && defined DEBUGON
                    DEBUGLOG( "\n\nBefore \n" );
                    for( int j=0; j<requestQueue.length; ++j )
                    {
                        DEBUGLOG( "RequestQueue %i priority: %i \n%s \n", j, requestQueue.list[j]->priority, requestQueue.list[j]->url.buffer );
                    }
                    
#endif
                    
                    requestQueue.reinsert( urlRequest, i );
                    
#if 0 && defined DEBUGON
                    DEBUGLOG( "\nAfter \n" );
                    for( int j=0; j<requestQueue.length; ++j )
                    {
                        DEBUGLOG( "RequestQueue %i priority: %i \n%s \n", j, requestQueue.list[j]->priority, requestQueue.list[j]->url.buffer );
                    }
                    
#endif
                    break;
                }
            }
        }
    }

    // If we already have a request, check it's priority level
    else if( urlRequest->priority != priority )
    {
        urlRequest->priority = priority;
        urlRequest->timeout = gEngine->gameTime.lifetime + timeout;

        // If our new priority if 0, push it to the back of the queue
        if( priority == 0 )
        {
            requestQueue.remove( urlRequest );
            requestQueue.add( urlRequest );
    }

        // If our new priority is greater, make sure it's positioned appropriately
        else
        {
            const int urlIndex = requestQueue.find( urlRequest );
            const int previousIndex = urlIndex - 1;
            const int nextIndex = urlIndex + 1;
            bool correctSlot = true;

            // Ensure the previous request has a greater or equal priority
            if( previousIndex >= 0 )
            {
                CCURLRequest *request = requestQueue.list[previousIndex];
                if( request->priority < priority )
                {
                    correctSlot = false;
                }
            }

            // Ensure the next request has a less than or equal priority
            if( nextIndex < requestQueue.length )
            {
                CCURLRequest *request = requestQueue.list[nextIndex];
                if( request->priority > priority )
                {
                    correctSlot = false;
                }
            }

            // Otherwise reinsert it in the correct priority
            if( correctSlot == false )
            {
                requestQueue.remove( urlRequest );
                requestQueue.add( urlRequest );
                for( int i=0; i<requestQueue.length; ++i )
                {
                    CCURLRequest *request = requestQueue.list[i];
                    if( request->priority < priority )
                    {
                        requestQueue.reinsert( urlRequest, i );
                        break;
                    }
                }
            }
        }
    }
    
    if( inCallback != NULL )
    {
        inCallback->reply = urlRequest;
        urlRequest->onComplete.add( inCallback );
    }
    
    // Is this request already cached?
    if( urlRequest->checkCache )
    {
        if( gEngine->gameTime.lifetime >= urlRequest->timeout )
        {
            if( urlRequest->cacheChecked == false )
            {
                urlRequest->cacheChecked = true;
                
                // See if the data has been cached
                if( urlRequest->cacheFile.length > 0 )
                {
                    char *fileData = NULL;
                    struct stat fileInfo;
                    const int fileSize = CCFileManager::getCachedFile( urlRequest->cacheFile.buffer, &fileData, false, &fileInfo );
                    if( /*false*/ fileSize > 0 )
                    {
//                        time_t timeNow = time( NULL );
//#if defined Q_OS_WIN || defined ANDROID
//                        time_t timeSince = timeNow - fileInfo.st_mtime;
//#else
//                        time_t timeSince = timeNow - fileInfo.st_mtimespec.tv_sec;
//#endif
                    
                        urlRequest->state = CCURLRequest::cached;
                        urlRequest->data.set( fileData, fileSize );
                        FREE_POINTER( fileData );

                        const bool removed = requestQueue.remove( urlRequest );
                        ASSERT( removed );
                        currentRequests.add( urlRequest );
                        finishURL( urlRequest );
                    }
                }
            }
        }
    }

	CCEngineThreadUnlock();
}


void CCURLManager::finishURL(CCURLRequest *request)
{
    //LAMBDA_EMIT_ONCE( request->onComplete );
    for( int i=0; i<request->onComplete.length; ++i )
    {
        CCLambdaCallback *callback = request->onComplete.list[i];
        callback->run();
    }
    request->onComplete.deleteObjectsAndList();

    // Save out our result?
    if( request->cacheFile.length > 0 )
    {
        if( request->state == CCURLRequest::succeeded )
        {
            CCFileManager::saveCachedFile( request->cacheFile.buffer, request->data.buffer, request->data.length );
        }
    }

    // Clean up our request object
    const bool removed = currentRequests.remove( request );
    ASSERT( removed );
    delete request;
    
    // Reset our high priority marker
    if( highPriorityRequestsPending )
    {
        highPriorityRequestsPending = false;
        for( int i=0; i<requestQueue.length; ++i )
        {
            CCURLRequest *request = requestQueue.list[i];
            if( request->priority > 0 )
            {
                highPriorityRequestsPending = true;
                break;
            }
        }
        for( int i=0; i<currentRequests.length; ++i )
        {
            CCURLRequest *request = currentRequests.list[i];
            if( request->priority > 0 )
            {
                highPriorityRequestsPending = true;
                break;
            }
        }
    }
}


void CCURLManager::setDomainTimeOut(const char *domain, float timeout)
{
    for( int i=0; i<domainTimeOuts.length; ++i )
    {
        DomainTimeOut *domainTimeOut = domainTimeOuts.list[i];
        if( CCText::Equals( domain, domainTimeOut->name.buffer ) )
        {
            return;
        }
    }
    
    DomainTimeOut *domainTimeOut = new DomainTimeOut();
    domainTimeOut->name = domain;
    domainTimeOut->timeout = timeout;
    domainTimeOuts.add( domainTimeOut );
}


const bool CCURLManager::readyToRequest()
{
    if( requestQueue.length > 0 )
    {
        if( deviceURLManager->readyToRequest() )
        {
            return true;
        }
    }
    return false;
}
