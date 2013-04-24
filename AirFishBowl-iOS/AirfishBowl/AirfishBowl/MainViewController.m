//
//  MainViewController.m
//  AirfishBowl
//
//  Created by David on 11/04/2013.
//  Copyright (c) 2013 davidkirwan. All rights reserved.
//

#import "MainViewController.h"



@interface MainViewController ()

@end

@implementation MainViewController

@synthesize ipAddr;
@synthesize ipLabel;
@synthesize forwardButton;
@synthesize leftButton;
@synthesize rightButton;
@synthesize stopButton;
@synthesize pitchDownButton;
@synthesize pitchUpButton;
@synthesize statusImage;


- (void)viewDidLoad
{
    forwardButton.layer.cornerRadius = 10;
    forwardButton.clipsToBounds = YES;
    
    stopButton.layer.cornerRadius = 10;
    stopButton.clipsToBounds = YES;
    
    leftButton.layer.cornerRadius = 10;
    leftButton.clipsToBounds = YES;
    
    rightButton.layer.cornerRadius = 10;
    rightButton.clipsToBounds = YES;
    
    pitchUpButton.layer.cornerRadius = 5;
    pitchUpButton.clipsToBounds = YES;
    
    pitchDownButton.layer.cornerRadius = 5;
    pitchDownButton.clipsToBounds = YES;
    
    ipAddr = [(AppDelegate*)[[UIApplication sharedApplication] delegate] defaultIP];
    [ipLabel setText:ipAddr];
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Flipside View


-(IBAction)forward:(id)sender
{
    NSLog(@"forward method called");
    NSLog(@"http://%@/leds.cgi?led=0", ipAddr);
    
    [forwardButton setHighlighted:YES];
    
    NSMutableString *theURL = [[NSMutableString alloc] initWithFormat:@"http://%@/leds.cgi?led=0", ipAddr];
    
    NSURL *url = [NSURL URLWithString:theURL];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    GTMHTTPFetcher *myFetcher = [GTMHTTPFetcher fetcherWithRequest:request];
    [myFetcher beginFetchWithDelegate:self
                    didFinishSelector:@selector(myFetcher:finishedWithData:error:)];
}


-(IBAction)stop:(id)sender
{
    NSLog(@"stop method called");
    NSLog(@"%@/leds.cgi?led=1", ipAddr);
    
    [forwardButton setHighlighted:NO];
    [leftButton setHighlighted:NO];
    [rightButton setHighlighted:NO];
    
    NSMutableString *theURL = [[NSMutableString alloc] initWithFormat:@"http://%@/leds.cgi?led=1", ipAddr];
    
    NSURL *url = [NSURL URLWithString:theURL];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    GTMHTTPFetcher *myFetcher = [GTMHTTPFetcher fetcherWithRequest:request];
    [myFetcher beginFetchWithDelegate:self
                    didFinishSelector:@selector(myFetcher:finishedWithData:error:)];
}


-(IBAction)left:(id)sender
{
    NSLog(@"left method called");
    NSLog(@"%@/leds.cgi?led=3", ipAddr);
    
    [leftButton setHighlighted:YES];
    
    NSMutableString *theURL = [[NSMutableString alloc] initWithFormat:@"http://%@/leds.cgi?led=3", ipAddr];
    
    NSURL *url = [NSURL URLWithString:theURL];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    GTMHTTPFetcher *myFetcher = [GTMHTTPFetcher fetcherWithRequest:request];
    [myFetcher beginFetchWithDelegate:self
                    didFinishSelector:@selector(myFetcher:finishedWithData:error:)];
}


-(IBAction)right:(id)sender
{
    NSLog(@"right method called");
    NSLog(@"%@/leds.cgi?led=2", ipAddr);
 
    [rightButton setHighlighted:YES];
    
    NSMutableString *theURL = [[NSMutableString alloc] initWithFormat:@"http://%@/leds.cgi?led=2", ipAddr];
    
    NSURL *url = [NSURL URLWithString:theURL];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    GTMHTTPFetcher *myFetcher = [GTMHTTPFetcher fetcherWithRequest:request];
    [myFetcher beginFetchWithDelegate:self
                    didFinishSelector:@selector(myFetcher:finishedWithData:error:)];
}


-(IBAction)pitchUp:(id)sender
{
    NSLog(@"pitchUp method called");
    NSLog(@"%@/leds.cgi?led=4", ipAddr);
    
    NSMutableString *theURL = [[NSMutableString alloc] initWithFormat:@"http://%@/leds.cgi?led=4", ipAddr];
    
    NSURL *url = [NSURL URLWithString:theURL];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    GTMHTTPFetcher *myFetcher = [GTMHTTPFetcher fetcherWithRequest:request];
    [myFetcher beginFetchWithDelegate:self
                    didFinishSelector:@selector(myFetcher:finishedWithData:error:)];
}


-(IBAction)pitchDown:(id)sender
{
    NSLog(@"pitchDown method called");
    NSLog(@"%@/leds.cgi?led=5", ipAddr);
    
    NSMutableString *theURL = [[NSMutableString alloc] initWithFormat:@"http://%@/leds.cgi?led=5", ipAddr];
    
    NSURL *url = [NSURL URLWithString:theURL];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    GTMHTTPFetcher *myFetcher = [GTMHTTPFetcher fetcherWithRequest:request];
    [myFetcher beginFetchWithDelegate:self
                    didFinishSelector:@selector(myFetcher:finishedWithData:error:)];
}


-(void)myFetcher:(GTMHTTPFetcher *)fetcher
finishedWithData:(NSData *)retrievedData
           error:(NSError *)error
{
    if(error != nil)
    {
        int status = [error code];
        NSLog(@"Error code received: %d", status);
        
        UIImage *image = [UIImage imageNamed:@"red.png"];
        [statusImage setImage:image];
    }
    else
    {
        UIImage *image = [UIImage imageNamed:@"green.png"];
        [statusImage setImage:image];
        
        // Get the response data in string form for outputting in the log
        NSMutableString *str = [[NSMutableString alloc] initWithData:retrievedData encoding:NSUTF8StringEncoding];
        
        NSLog(@"%@", str);
    }
}


- (void)flipsideViewControllerDidSave:(FlipsideViewController *)controller :(NSString *)theIp
{
    NSLog(@"The original contents of ipAddr: %@", ipAddr);
    [(AppDelegate*)[[UIApplication sharedApplication] delegate] setDefaultIP:theIp];
    ipAddr = theIp;
    NSLog(@"The new contents of ipAddr: %@", ipAddr);
    
    [ipLabel setText:ipAddr];
    
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)flipsideViewControllerDidCancel:(FlipsideViewController *)controller
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"showSettings"]) {
        [[segue destinationViewController] setDelegate:self];
    }
}

@end
