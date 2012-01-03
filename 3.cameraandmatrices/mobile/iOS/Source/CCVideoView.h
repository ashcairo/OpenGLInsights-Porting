/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCVideoView.h
 * Description : Video player view.
 *
 * Created     : 03/09/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#import <MediaPlayer/MediaPlayer.h>

@interface CCVideoView : UIView
{	
@public
    MPMoviePlayerController *player;
    
    bool playing;
}

-(void)playVideo:(const char*)filePath;

-(void)play;
-(void)pause;
-(void)stop;
-(void)remove;

@end
