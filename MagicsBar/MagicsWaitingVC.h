//
//  MagicsWaitingVC.h
//  MagicsBar
//
//  Created by 刘铭 on 16/3/10.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MagicsWaitingVC : UIViewController {
@private
    
}
@property(strong, nonatomic) IBOutlet UIImageView *progressImageView;
@property(strong, nonatomic) IBOutlet UIView *progressBackgroundView;
@property(strong, nonatomic) UIActivityIndicatorView* activityIndicator;

+ (MagicsWaitingVC *)startWaiting;
- (void)startAnimating;
- (void)stopAnimating;
- (void)stopAndRemoveAnimating;
@end
