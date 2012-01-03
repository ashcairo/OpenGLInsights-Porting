/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceURLManager.h
 * Description : iOS specific url manager.
 *
 * Created     : 04/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

@class CCDeviceURLManagerOC;

class CCDeviceURLManager
{
public:
    CCDeviceURLManager();
    ~CCDeviceURLManager();
    
    void processRequest(CCURLRequest *inRequest);
    void clear();
    const bool readyToRequest();
    
protected:
    CCDeviceURLManagerOC *internal;
};


struct CCURLRequestPacket
{
    CCURLRequestPacket()
    {
        request = NULL;
        connection = NULL;
        data = NULL;
    }
    ~CCURLRequestPacket()
    {
        if( data != NULL )
        {
            [data release];
        }
    }
    CCURLRequest *request;
    NSURLConnection *connection;
	NSMutableData *data;
};

@interface CCDeviceURLManagerOC : NSObject 
{	
@private
    CCList<CCURLRequestPacket> currentRequests;
}

-(void)processRequest:(CCURLRequest*)inRequest;

-(void)connection:(NSURLConnection*)connection didReceiveResponse:(NSURLResponse*)response;
-(void)connection:(NSURLConnection*)connection didReceiveData:(NSData*)data;
-(void)connection:(NSURLConnection*)connection didFailWithError:(NSError*)error;
-(void)connectionDidFinishLoading:(NSURLConnection*)connection;

-(void)clear;

@end


void SetLastRequestTime(NSUserDefaults *archive, NSString *string);
const float GetLastRequestTime(NSUserDefaults *archive, NSString *string);