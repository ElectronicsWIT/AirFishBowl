//
//  FlipsideViewController.h
//  AirfishBowl
//
//  Created by David on 11/04/2013.
//  Copyright (c) 2013 davidkirwan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import "TweetViewController.h"
 
@class FlipsideViewController;

@protocol FlipsideViewControllerDelegate
- (void)flipsideViewControllerDidSave:(FlipsideViewController *)controller :(NSString *)ipAddr;
- (void)flipsideViewControllerDidCancel:(FlipsideViewController *)controller;
@end



@interface FlipsideViewController : UIViewController <TweetViewControllerDelegate>

@property (strong, nonatomic) IBOutlet UITextField *ipAddr;
@property (strong, nonatomic) IBOutlet UISwitch *musicSwitch;
@property (weak, nonatomic) id <FlipsideViewControllerDelegate> delegate;

- (IBAction)save:(id)sender;
- (IBAction)cancel:(id)sender;

@end
