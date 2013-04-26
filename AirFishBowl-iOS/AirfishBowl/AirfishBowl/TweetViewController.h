//
//  TweetViewController.h
//  AirfishBowl
//
//  Created by David on 20/04/2013.
//  Copyright (c) 2013 davidkirwan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Accounts/Accounts.h>
#import <Social/Social.h>
#import <Twitter/Twitter.h>

@class TweetViewController;

@protocol TweetViewControllerDelegate
- (void)tweetViewControllerDone:(TweetViewController *)controller;
@end

@interface TweetViewController : UIViewController

@property (nonatomic) ACAccountStore *accountStore;
@property (weak, nonatomic) id <TweetViewControllerDelegate> delegate;

- (IBAction)done:(id)sender;
- (IBAction)tweetIt:(id)sender;
- (BOOL)userHasAccessToTwitter;

@end
