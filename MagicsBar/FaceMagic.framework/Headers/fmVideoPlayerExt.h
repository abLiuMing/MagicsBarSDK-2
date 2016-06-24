//
//  FMVideoPlayerExt.h
//  fmStatic
//
//  Created by WangYang on 16/4/6.
//  Copyright © 2016年 wysaid. All rights reserved.
//

#import "fmVideoPlayer.h"

@interface FMVideoPlayerExt : FMVideoPlayer

@property(nonatomic, weak, setter=setProcessingDelegate:) id<FMFrameProcessingDelegate> processingDelegate;

@end
