/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCViewController.h
 * Description : iOS view controller for handling orientation and views.
 *
 * Created     : 07/06/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

//#define USE_ADMOB

#ifdef USE_ADMOB
#import "GADBannerView.h"
#endif

@interface CCViewController : UIViewController
{
#ifdef USE_ADMOB
    GADBannerView *adContainer;
    bool adEnabled;
#endif
}

-(void)toggleAdverts:(const bool)toggle;

@end
