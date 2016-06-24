//
//  MagicsToolbarVC.h
//  MagicsBar
//
//  Created by 刘铭 on 16/4/14.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "fmEmptyEffectViewHandler.h"

@protocol MagicToolbarDelegate<NSObject>
/**
 *  @brief 选中某个效果 返回效果filePath
 *
 *  @param filePath 返回值
 */
@optional
- (void)didSelectEffect:(NSString *)filePath;

@end

@class FMCameraViewHandler;
@class FMVideoPlayerViewHandlerExt;
@class FMEmptyEffectViewHandler;

@interface MagicToolbarVC : UIViewController

@property (nonatomic, weak) id<MagicToolbarDelegate> delegate;

@property (nonatomic, strong) FMCameraViewHandler *cameraViewHandler;
/**
 *  @brief 合成调用
 */
@property (nonatomic, strong) FMVideoPlayerViewHandlerExt *videoPlayerHandlerExt;

/**
 *  @brief 非合成参数
 */
@property (nonatomic, strong) FMEmptyEffectViewHandler *emptyEffectViewHandler;

/**
 *  @brief 父VC
 */
@property (nonatomic, weak) UIViewController *superVC;

/**
 *  @brief 获取toolbar
 *
 *  @return Returns the shared Toolbar
 */
+ (instancetype)sharedToolbar;

/**
 *  @brief 检测网络
 */
+ (void)startNotifier;

/**
 *  @brief 初始化MagicsToolbarVC
 *
 *  @param view 父View
 *  @param rect MagicsToolbarVC的frame
 */
- (void)parentView:(UIView *)view frame:(CGRect)rect;


/**
 *  @brief 是否隐藏工具条
 *
 *  @param hidden YES or NO
 */
- (void)hideToolbar:(BOOL)hidden;


/**
 *  @brief 清空视频或相机特效
 */
- (void)clear;

/**
 *  @brief 获取特效缓存
 *
 *  @return 缓存大小 M单位
 */
- (NSString *)getCache;

/**
 *  @brief 清除缓存
 *
 *  @return YES = Succeed NO = Fail
 */
- (BOOL)clearCache;

/**
 *  @brief 叠加效果
 *
 *  @param path 效果路径
 */
- (void)addEffectWithPath:(NSString *)path;


@end
