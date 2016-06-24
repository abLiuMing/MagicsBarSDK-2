//
//  MagicsWaitingVC.m
//  MagicsBar
//
//  Created by 刘铭 on 16/3/10.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import "MagicsWaitingVC.h"

@interface MagicsWaitingVC ()

@end

@implementation MagicsWaitingVC

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)dealloc
{
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self.view.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.5];//colorWithRed:0 green:0 blue:0 alpha:0.5];
    
    CGRect frame = [UIScreen mainScreen].bounds;
    self.view.frame = frame;
    
    CGPoint center;
    center.x = frame.size.width / 2;
    center.y = frame.size.height / 2;
    self.progressBackgroundView.center = center;
    self.progressBackgroundView.backgroundColor = [UIColor clearColor];
    
    _progressBackgroundView.layer.cornerRadius = 8.0;
    //    _progressImageView.animationDuration = 3 / 5;
    //    _progressImageView.animationImages = @[[UIImage imageNamed:@"progress1"],
    //                                           [UIImage imageNamed:@"progress2"],
    //                                           [UIImage imageNamed:@"progress3"],
    //                                           [UIImage imageNamed:@"progress4"],
    //                                           [UIImage imageNamed:@"progress5"],
    //                                           [UIImage imageNamed:@"progress6"],
    //                                           ];
    //
    //    [_progressImageView startAnimating];
    
    self.activityIndicator = [[UIActivityIndicatorView alloc ]initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    self.activityIndicator.center = self.view.center;
    //        self.activityIndicator.
    [self.activityIndicator startAnimating];
    [self.view addSubview: self.activityIndicator];
    
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

- (void)startAnimating
{
    //    [_progressImageView startAnimating];
    [self.activityIndicator startAnimating];
}

- (void)stopAnimating
{
    //    [_progressImageView stopAnimating];
    [self.activityIndicator stopAnimating];
}

- (void)stopAndRemoveAnimating
{
    //    [_progressImageView stopAnimating];
    [self.activityIndicator stopAnimating];
    [self.view removeFromSuperview];
}

+ (MagicsWaitingVC *)startWaiting
{
    MagicsWaitingVC *indictorViewController = [[MagicsWaitingVC alloc] initWithNibName:@"WaitingIndictorViewController" bundle:nil];
    [indictorViewController startAnimating];
    [[UIApplication sharedApplication].keyWindow addSubview:indictorViewController.view];
    return indictorViewController;
}
@end


