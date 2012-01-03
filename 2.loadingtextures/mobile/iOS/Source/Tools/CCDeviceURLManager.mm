/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceURLManager.mm
 *-----------------------------------------------------------
 */

#import "CCDefines.h"
#import "CCDeviceURLManager.h"


CCDeviceURLManager::CCDeviceURLManager()
{
    internal = [[CCDeviceURLManagerOC alloc] init];
}


CCDeviceURLManager::~CCDeviceURLManager()
{
    [internal release];
}


void CCDeviceURLManager::processRequest(CCURLRequest *inRequest)
{
    [internal processRequest:inRequest];
}


void CCDeviceURLManager::clear()
{
    [internal clear];
}


const bool CCDeviceURLManager::readyToRequest()
{
    return true;
}


@implementation CCDeviceURLManagerOC

-(id)init
{
	self = [super init];
	if( self != NULL )
    {
    }
    return self;
}


-(void)dealloc
{
	currentRequests.deleteObjectsAndList();
	[super dealloc];
}


-(void)processRequest:(CCURLRequest*)inRequest
{
    CCURLRequestPacket *urlRequestPacket = new CCURLRequestPacket();
    currentRequests.add( urlRequestPacket );
    urlRequestPacket->request = inRequest;
    urlRequestPacket->request->state = CCURLRequest::in_flight;
	
	// Filter out spaces
	NSString *url = [[NSString alloc] initWithFormat:@"%s", urlRequestPacket->request->url.buffer];
	[url stringByReplacingOccurrencesOfString:@" " withString:@"%20"];
	
#if DEBUGON
    //NSLog( @"NSMutableURLRequest: %@ \n", url );
#endif
    
	// create the request
	NSURLRequest *theRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:url]
												cachePolicy:NSURLRequestUseProtocolCachePolicy
											timeoutInterval:15.0];
	[url release];
	
	// create the connection with the request
	// and start loading the data
	urlRequestPacket->connection = [[NSURLConnection alloc] initWithRequest:theRequest delegate:self];
	if( urlRequestPacket->connection )
	{
		// Create the NSMutableData that will hold
		// the received data
		// receivedData is declared as a method instance elsewhere
		urlRequestPacket->data = [[NSMutableData data] retain];	
	}
	else 
	{
		// inform the user that the download could not be made
        ASSERT( false );
	}
}


-(CCURLRequestPacket*)findRequest:(NSURLConnection*)connection
{
    for( int i=0; i<currentRequests.length; ++i )
    {
        if( currentRequests.list[i]->connection == connection )
        {
            return currentRequests.list[i];
        }
    }
    return NULL;
}


-(void)connection:(NSURLConnection*)connection didReceiveResponse:(NSURLResponse*)response
{
    CCURLRequestPacket *packet = [self findRequest:connection];
    if( packet == NULL )
    {
        return;
    }
    
    // this method is called when the server has determined that it
    // has enough information to create the NSURLResponse
	
    // it can be called multiple times, for example in the case of a
    // redirect, so each time we reset the data.
    // receivingData is declared as a method instance elsewhere
    [packet->data setLength:0];	
    
    NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse*)response;
	if( [response respondsToSelector:@selector(allHeaderFields)] )
    {
        NSURL *url = [response URL];
        if( url != NULL )
        {
            NSString *urlString = [url absoluteString];
            if( urlString != NULL )
            {
                CCText *headerName = new CCText( "Location" );
                CCText *headerData = new CCText( [urlString UTF8String] );
                packet->request->header.names.add( headerName );
                packet->request->header.values.add( headerData );
            }
        }
        
		NSDictionary *dictionary = [httpResponse allHeaderFields];
        for( NSString *key in [dictionary allKeys] )
        {
            NSString *value = [dictionary objectForKey:key];
            CCText *headerName = new CCText( [key UTF8String] );
            CCText *headerData = new CCText( [value UTF8String] );
            packet->request->header.names.add( headerName );
            packet->request->header.values.add( headerData );
        }
	}
}


-(void)connection:(NSURLConnection*)connection didReceiveData:(NSData*)data
{
    CCURLRequestPacket *packet = [self findRequest:connection];
    if( packet == NULL )
    {
        return;
    }
    
    // append the new data to the receivingData	
    // receivingData is declared as a method instance elsewhere
    [packet->data appendData:data];
}


-(void)connection:(NSURLConnection*)connection didFailWithError:(NSError*)error
{
    CCURLRequestPacket *packet = [self findRequest:connection];
    if( packet == NULL )
    {
        return;
    }
    
#if DEBUGON
    NSLog( @"Download failed! Error - %@ %@ \n",
           [error localizedDescription],
           [[error userInfo] objectForKey:NSURLErrorFailingURLStringErrorKey] );
#endif
    
	// Copy over the recieved data
	CCEngineThreadLock();
	packet->request->state = CCURLRequest::failed;
    packet->request->data.set( (char*)[packet->data bytes], [packet->data length] );
	CCEngineThreadUnlock();
    
    // Clear
    currentRequests.remove( packet );
    delete packet;
	
    // inform the user
    DEBUGLOG( "Connection failed! Error - %s %s \n",
              [[error localizedDescription] UTF8String],
              [[[error userInfo] objectForKey:NSErrorFailingURLStringKey] UTF8String] );
	
	// release the connection
    [connection release];
}


-(void)connectionDidFinishLoading:(NSURLConnection*)connection
{
    CCURLRequestPacket *packet = [self findRequest:connection];
    if( packet == NULL )
    {
        return;
    }
    
    // do something with the data
    // receivingData is declared as a method instance elsewhere
    //DEBUGLOG( "Succeeded! Received %d bytes of data\n", [receivedData length] );
	
	// Copy over the recieved data
    if( packet->request->state == CCURLRequest::in_flight )
    {
        CCEngineThreadLock();
        packet->request->state = CCURLRequest::succeeded;
        uint length = [packet->data length];
        packet->request->data.set( (char*)[packet->data bytes], length );
        CCEngineThreadUnlock();
        
        currentRequests.remove( packet );
        delete packet;
    }
    else
    {
        ASSERT( false );
    }
	
    // release the connection
    [connection release];
}


-(void)clear
{
    currentRequests.deleteObjects();
}

@end


void SetLastRequestTime(NSUserDefaults *archive, NSString *string)
{
	NSString *timeString = [[NSString alloc] initWithFormat:@"%@Time", string];
	
	NSDate *now = [NSDate date];
	const float timeSince = (float)[now timeIntervalSinceReferenceDate];
	[archive setFloat:timeSince forKey:timeString];
	
	[timeString release];
}


const float GetLastRequestTime(NSUserDefaults *archive, NSString *string)
{
	NSString *timeString = [[NSString alloc] initWithFormat:@"%@Time", string];
	
	NSDate *now = [NSDate date];
	const float timeSince = (float)[now timeIntervalSinceReferenceDate];
	
	float lastTime = [archive floatForKey:timeString];
	const float difference = timeSince - lastTime;
	
	[timeString release];
	
	return difference;
}