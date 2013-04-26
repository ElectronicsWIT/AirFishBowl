//
//  AppDelegate.m
//  AirfishBowl
//
//  Created by David on 11/04/2013.
//  Copyright (c) 2013 davidkirwan. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

@synthesize isPlaying;
@synthesize audioPlayer;
@synthesize defaultIP;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    defaultIP = @"192.168.1.115";
    
    @try {
        NSURL *url = [NSURL fileURLWithPath:[NSString stringWithFormat:@"%@/Jackson_F_Smith_-_01_-_Cantina_Rag.mp3", [[NSBundle mainBundle] resourcePath]]];
        NSError *error;
        audioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
        audioPlayer.numberOfLoops = -1;
        
        // Music does not play at startup, you must turn it on
        //[audioPlayer play];
        //isPlaying = YES;
    }
    @catch (NSException *exception) {
        NSLog(@"Exception: %@", exception);
    }
    @finally {
        //
    }
    
    return YES;
}


-(void)setTheIP:(NSString *)newIP
{
    defaultIP = newIP;
}


-(IBAction)toggleMusic:(id)sender
{
    if(isPlaying)
    {
        [audioPlayer stop];
        isPlaying = NO;
    }
    else
    {
        [audioPlayer play];
        isPlaying = YES;
    }
}

				
- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
