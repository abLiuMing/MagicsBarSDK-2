//
//  MagicsStoreViewController.h
//  MagicsBar
//
//  Created by 刘铭 on 16/3/10.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol MagicsStoreViewDelegate <NSObject>
- (void)selectedSopt:(UIImage*)soptImage;
@end

@interface MagicsStoreViewController : UIViewController

@property (nonatomic,assign) id <MagicsStoreViewDelegate> delegate;

@property (nonatomic,  copy) void (^complete)(UIImage* image,NSString* zipPath);
@end
