//
//  MagicsStoreDetailCVC.h
//  MagicsBar
//
//  Created by 刘铭 on 16/3/10.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MagicsStoreDetailCVC : UICollectionViewController

@property (nonatomic,assign) BOOL commonType;
@property (nonatomic,strong) NSDictionary* dataDic;
@property (nonatomic,assign) NSInteger index;
@property (nonatomic,  copy) void (^complete)(UIImage* image,NSString* zipPath);//NSDictionary* data,
@end
