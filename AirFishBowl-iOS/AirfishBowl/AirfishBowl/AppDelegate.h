//
//  AppDelegate.h
//  AirfishBowl
//
//  Created by David on 11/04/2013.
//  Copyright (c) 2013 davidkirwan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>



@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property BOOL isPlaying;
@property NSString *defaultIP;
@property AVAudioPlayer *audioPlayer;

@property (strong, nonatomic) UIWindow *window;

-(IBAction)toggleMusic:(id)sender;
-(void)setTheIP:(NSString *)newIP;

@end
