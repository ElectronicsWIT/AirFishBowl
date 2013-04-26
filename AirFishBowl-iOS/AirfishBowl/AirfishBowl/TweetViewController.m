//
//  TweetViewController.m
//  AirfishBowl
//
//  Created by David on 20/04/2013.
//  Copyright (c) 2013 davidkirwan. All rights reserved.
//

#import "TweetViewController.h"


@interface TweetViewController ()

@end

@implementation TweetViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)userHasAccessToTwitter
{
    return [SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter];
}

- (IBAction)done:(id)sender
{
    [self.delegate tweetViewControllerDone:self];
}

- (IBAction)tweetIt:(id)sender
{
    NSLog(@"tweetIt button pressed");
    
        
    if([self userHasAccessToTwitter])
    {
        NSLog(@"The phone has a twitter profile available!");
        
        self.accountStore = [[ACAccountStore alloc] init];
        
        ACAccountType *twitterAccountType = [self.accountStore
                                          accountTypeWithAccountTypeIdentifier:
                                          ACAccountTypeIdentifierTwitter];
        
        [self.accountStore
         requestAccessToAccountsWithType:twitterAccountType
         withCompletionHandler:^(BOOL granted, NSError *error)
         {
             if(granted)
             {
                 NSLog(@"We were allowed to access the twitter accounts");
                 
                 NSDictionary *params = [[NSDictionary alloc] initWithObjectsAndKeys:@"#helium #fish #blimp www.airfishbowl.com #airfishbowl", @"status", nil];
                 
                 SLRequest *slRequest = [SLRequest requestForServiceType:SLServiceTypeTwitter
                                                           requestMethod:SLRequestMethodPOST
                                                                     URL:[NSURL URLWithString:@"https://api.twitter.com/1.1/statuses/update.json"]
                                                              parameters:params];
                 
                 NSArray *twitterAccounts = [self.accountStore accountsWithAccountType:twitterAccountType];
                 
                 [slRequest setAccount:[twitterAccounts objectAtIndex:0]];
                 
                 
                 [slRequest performRequestWithHandler:^(NSData *responseData,
                                                        NSHTTPURLResponse *urlResponse,
                                                        NSError *error)
                  {
                      NSLog(@"The response: %@", [[NSString alloc] initWithData:responseData
                                                                       encoding:NSUTF8StringEncoding]);
                  }];

             }
             else
             {
                 NSLog(@"We were not allowed to access the twitter accounts");
             }
             
         }];
    }
    else
    {
        NSLog(@"The phone does not have a twitter profile available!");
    }
}

@end
