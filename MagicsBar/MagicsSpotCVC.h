//
//  MagicsSpotCVC.h
//  MagicsBar
//
//  Created by 刘铭 on 16/3/10.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol MagicsSpotCVCDelegate <NSObject>
- (void)scroll:(NSInteger)indexPath;
@end

@interface MagicsSpotCVC : UICollectionViewController

@property (nonatomic,assign) BOOL remoteContainerSelectedAction;
@property (nonatomic,assign) id <MagicsSpotCVCDelegate> delegate;
@property (nonatomic,  copy) void (^complete)(NSDictionary* data);
@property (nonatomic,strong) NSDictionary* dataDic;
@property (nonatomic,  copy) NSString* hostUrl;
@end
