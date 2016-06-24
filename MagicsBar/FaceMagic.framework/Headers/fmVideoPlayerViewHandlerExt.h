//
//  fmVideoPlayerViewHandlerExt.h
//  fmStatic
//
//  Created by WangYang on 16/4/5.
//  Copyright © 2016年 wysaid. All rights reserved.
//

#import "fmVideoPlayerViewHandler.h"

@interface FMVideoPlayerViewHandlerExt : FMVideoPlayerViewHandler

- (void)setProcessingDelegate: (id<FMFrameProcessingDelegate>)delegate;

- (id<FMFrameProcessingDelegate>)processingDelegate;

@end
