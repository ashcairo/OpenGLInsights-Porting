/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCViewController.mm
 *-----------------------------------------------------------
 */

#import "CCViewController.h"
#import "CCGLView.h"

#ifdef USE_ADMOB
NSString *gAdMobPublisherID = NULL;
#endif


@implementation CCViewController

// Implement loadView to create a view hierarchy programmatically, without using a nib.
-(void)loadView 
{
	// get the window frame here.
	self.view = gView;
}


-(void)viewDidUnload 
{
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = NULL;
#ifdef USE_ADMOB
    if( adContainer != NULL )
    {
        [adContainer release];
        adContainer = NULL;
    }
#endif
}


-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{	
	return false;
}


-(void)toggleAdverts:(const bool)toggle
{
    if( toggle )
    {
#ifdef USE_ADMOB
        if( adContainer == NULL )
        {
            if( gAdMobPublisherID != NULL )
            {
                adEnabled = true;
                
                const float screenWidth = self.view.frame.size.width;
                const float adWidth = GAD_SIZE_320x50.width;
                const float startX = ( screenWidth - adWidth ) * 0.5f;
                const float startY = self.view.frame.size.height - GAD_SIZE_320x50.height;
                
                // Create a view of the standard size at the bottom of the screen.
                adContainer = [[GADBannerView alloc] 
                               initWithFrame:CGRectMake( startX,
                                                         startY,
                                                         GAD_SIZE_320x50.width,
                                                         GAD_SIZE_320x50.height )];
                
                // Specify the ad's "unit identifier." This is your AdMob Publisher ID.
                adContainer.adUnitID = gAdMobPublisherID;
                
                // Let the runtime know which UIViewController to restore after taking
                // the user wherever the ad goes and add it to the view hierarchy.
                adContainer.rootViewController = self;
                [self.view addSubview:adContainer];
                
                // Initiate a generic request to load it with an ad.
                [adContainer loadRequest:[GADRequest request]];
            }
        }
        else if( adEnabled == false )
        {
            adEnabled = true;
            [UIView beginAnimations:nil context:NULL];
            [UIView setAnimationDuration:0.25];
            CGAffineTransform transform = CGAffineTransformMakeTranslation( 0.0f, 0.0f );
            adContainer.transform = transform;
            [UIView commitAnimations];
        }
#endif
    }
    else
    {
#ifdef USE_ADMOB
        if( adContainer != NULL )
        {
            if( adEnabled )
            {
                adEnabled = false;
                [UIView beginAnimations:nil context:NULL];
                [UIView setAnimationDuration:0.25];
                CGAffineTransform transform = CGAffineTransformMakeTranslation( 0.0f, GAD_SIZE_320x50.height );
                adContainer.transform = transform;
                [UIView commitAnimations];
            }
        }
#endif
    }
}

@end
