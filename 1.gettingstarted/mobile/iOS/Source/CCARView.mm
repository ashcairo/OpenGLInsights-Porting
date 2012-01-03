/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCARView.mm
 *-----------------------------------------------------------
 */

#import "CCARView.h"


@implementation CCARView

-(id)initWithFrame:(CGRect)frame
{	
	self = [super initWithFrame:frame];
    if( self != NULL )
	{
		// Create the AVCapture Session
        session = [[AVCaptureSession alloc] init];
        
        // create a preview layer to show the output from the camera
        AVCaptureVideoPreviewLayer *previewLayer = [AVCaptureVideoPreviewLayer layerWithSession:session];
        previewLayer.frame = self.frame;
        [self.layer addSublayer:previewLayer];
        
        // Get the default camera device
        AVCaptureDevice* camera = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
        
        // Create a AVCaptureInput with the camera device
        NSError *error = nil;
        AVCaptureInput* cameraInput = [[AVCaptureDeviceInput alloc] initWithDevice:camera error:&error];
        if( cameraInput == nil ) 
        {
            NSLog( @"Error to create camera capture:%@", error );
        }
        
        // Set the output
        AVCaptureVideoDataOutput* videoOutput = [[AVCaptureVideoDataOutput alloc] init];
        
        // create a queue to run the capture on
        dispatch_queue_t captureQueue=dispatch_queue_create( "catpureQueue", NULL );
        
        // setup our delegate
        [videoOutput setSampleBufferDelegate:self queue:captureQueue];
        
        // configure the pixel format
        videoOutput.videoSettings = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithUnsignedInt:kCVPixelFormatType_32BGRA], (id)kCVPixelBufferPixelFormatTypeKey,
									 nil];
        
        // and the size of the frames we want
        [session setSessionPreset:AVCaptureSessionPresetMedium];
        
        // Add the input and output
        [session addInput:cameraInput];
        [session addOutput:videoOutput];
        
        // Start the session
        [session startRunning];	 
	}
	
    return self;
}

-(void)dealloc
{
    [super dealloc];
}


-(void)remove
{
    [session stopRunning];
    [session release];
    session = nil;
    
    [self removeFromSuperview];
}

@end
