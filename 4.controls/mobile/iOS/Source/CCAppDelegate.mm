/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCAppDelegate.mm
 *-----------------------------------------------------------
 */

#import "CCDefines.h"
#import "CCAppDelegate.h"
#import "CCViewManager.h"


@implementation CCAppDelegate


-(id)init
{
	self = [super init];
	if( self != NULL )
	{
        self->viewManager = new CCViewManager();
	}
	
	return self;
}


-(void)applicationDidFinishLaunching:(UIApplication*)application
{
    viewManager->startup();
}


-(void)dealloc 
{	
    delete viewManager;
	[super dealloc];
}


-(void)applicationWillTerminate:(UIApplication*)application
{
    viewManager->shutdown();
}


-(void)applicationWillResignActive:(UIApplication*)application
{
    viewManager->pause();
}


-(void)applicationDidBecomeActive:(UIApplication*)application
{
    viewManager->resume();
}


-(void)applicationDidReceiveMemoryWarning:(UIApplication*)application
{
	NSLog( @"applicationDidReceiveMemoryWarning" );
}  

@end

