//
//  FlipsideViewController.m
//  AirfishBowl
//
//  Created by David on 11/04/2013.
//  Copyright (c) 2013 davidkirwan. All rights reserved.
//

#import "FlipsideViewController.h"

@interface FlipsideViewController ()

@end

@implementation FlipsideViewController

@synthesize ipAddr;
@synthesize musicSwitch;

- (void)viewDidLoad
{
    [ipAddr setText:[(AppDelegate*)[[UIApplication sharedApplication] delegate] defaultIP]];
    
    if([(AppDelegate*)[[UIApplication sharedApplication] delegate] isPlaying])
    {
        [musicSwitch setOn:YES];
    }
    else
    {
        [musicSwitch setOn:NO];
    }
    
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Actions

- (IBAction)save:(id)sender
{
    if([(AppDelegate*)[[UIApplication sharedApplication] delegate] isPlaying] &&
       [musicSwitch isOn])
    {
        
    }
    else if(![(AppDelegate*)[[UIApplication sharedApplication] delegate] isPlaying] &&
            ![musicSwitch isOn])
    {
        
    }
    else
    {
        [(AppDelegate*)[[UIApplication sharedApplication] delegate] toggleMusic:self];
    }
    [self.delegate flipsideViewControllerDidSave:self :ipAddr.text];
}

- (IBAction)cancel:(id)sender
{        
    [self.delegate flipsideViewControllerDidCancel:self];
}

- (void)tweetViewControllerDone:(TweetViewController *)controller
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"showTweet"]) {
        [[segue destinationViewController] setDelegate:self];
    }
}

@end
