/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCVideoView.mm
 *-----------------------------------------------------------
 */

#import "CCVideoView.h"

#import "CCBaseTypes.h"


@implementation CCVideoView

-(id)initWithFrame:(CGRect)frame
{	
	self = [super initWithFrame:frame];
    if( self != NULL )
	{
	}
	
    return self;
}

-(void)dealloc
{
    [super dealloc];
}


-(void)playVideo:(const char*)file
{
    NSString *filePathString = [[NSString alloc] initWithFormat:@"%s", file];
    NSURL *filePathURL = [NSURL fileURLWithPath:filePathString];
    
    if( player == NULL )
    {
        player = [[MPMoviePlayerController alloc] initWithContentURL:filePathURL];
        ASSERT( player != NULL );
        
        // Register the current object as an observer for the movie notifications.
        {
            [[NSNotificationCenter defaultCenter] addObserver:self 
                                                     selector:@selector( loadStateDidChange: ) 
                                                         name:MPMoviePlayerLoadStateDidChangeNotification 
                                                       object:player];
            
            [[NSNotificationCenter defaultCenter] addObserver:self 
                                                     selector:@selector( moviePlayBackDidFinish: ) 
                                                         name:MPMoviePlayerPlaybackDidFinishNotification 
                                                       object:player];
            
            [[NSNotificationCenter defaultCenter] addObserver:self 
                                                     selector:@selector( mediaIsPreparedToPlayDidChange: ) 
                                                         name:MPMediaPlaybackIsPreparedToPlayDidChangeNotification 
                                                       object:player];
            
            [[NSNotificationCenter defaultCenter] addObserver:self 
                                                     selector:@selector( moviePlayBackStateDidChange: ) 
                                                         name:MPMoviePlayerPlaybackStateDidChangeNotification 
                                                       object:player];   
        }
        
        // Preferences
        {
            player.repeatMode = MPMovieRepeatModeOne;    
            player.controlStyle = MPMovieControlStyleNone;
        }
        [[player view] setFrame:self.frame];
        
        // To present a movie in your application, incorporate the view contained 
        // in a movie player’s view property into your application’s view hierarchy. 
        // Be sure to size the frame correctly.
        [self addSubview: [player view]];        
    }
    else
    {
        [player setContentURL:filePathURL];
    }
    
    [filePathString release];
    
    playing = true;
}


-(void)play
{
    [player play];
    playing = true;
}


-(void)pause
{
    [player pause];
    playing = false;
}


-(void)stop
{
    [player stop];
    playing = false;
}


-(void)remove
{
    if( player != NULL )
    {
        [[NSNotificationCenter defaultCenter] removeObserver:self];
        [player.view removeFromSuperview];
    }
    [self removeFromSuperview];
}


// Handle movie load state changes.
-(void)loadStateDidChange:(NSNotification*)notification 
{
}

// Notification called when the movie finished playing.
-(void)moviePlayBackDidFinish:(NSNotification*)notification
{
}

// Called when the movie playback state has changed.
-(void)moviePlayBackStateDidChange:(NSNotification*)notification
{
}

// Notifies observers of a change in the prepared-to-play state of an object conforming to the MPMediaPlayback protocol.
-(void)mediaIsPreparedToPlayDidChange:(NSNotification*)notification
{
}

@end
