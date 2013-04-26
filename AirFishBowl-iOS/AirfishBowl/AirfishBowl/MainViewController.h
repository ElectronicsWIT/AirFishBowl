//
//  MainViewController.h
//  AirfishBowl
//
//  Created by David on 11/04/2013.
//  Copyright (c) 2013 davidkirwan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "FlipsideViewController.h"
#import "GTMHTTPFetcher.h"


@interface MainViewController : UIViewController <FlipsideViewControllerDelegate>

@property NSString *ipAddr;
@property (strong, nonatomic) IBOutlet UIButton *forwardButton;
@property (strong, nonatomic) IBOutlet UIButton *leftButton;
@property (strong, nonatomic) IBOutlet UIButton *rightButton;
@property (strong, nonatomic) IBOutlet UILabel *ipLabel;
@property (strong, nonatomic) IBOutlet UIButton *stopButton;
@property (strong, nonatomic) IBOutlet UIButton *pitchDownButton;
@property (strong, nonatomic) IBOutlet UIButton *pitchUpButton;
@property (strong, nonatomic) IBOutlet UIImageView *statusImage;


-(IBAction)forward:(id)sender;
-(IBAction)left:(id)sender;
-(IBAction)right:(id)sender;
-(IBAction)stop:(id)sender;
-(IBAction)pitchUp:(id)sender;
-(IBAction)pitchDown:(id)sender;

@end
