//
//  fmEmptyEffect.h
//  fmFaceTracker
//
//  Created by wysaid on 16/4/10.
//  Copyright © 2016年 wysaid. All rights reserved.
//

#ifndef fmEmptyEffect_h
#define fmEmptyEffect_h

#import <GLKit/GLKit.h>
#import "fmFaceCommon.h"
#import "fmSharedGLContext.h"
#include "fmFrameRenderer.h"

@interface FMEmptyEffect : NSObject

@property(nonatomic, weak, setter=setProcessingDelegate:) id<FMFrameProcessingDelegate> processingDelegate;
@property(nonatomic, weak) id<FMFrameUpdateDelegate> updateDelegate;

@property(nonatomic) FMSharedGLContext* sharedContext;
@property(nonatomic, readonly) NSLock* renderLock;


- (void)setProcessingDelegate: (id<FMFrameProcessingDelegate>)delegate;

- (id<FMFrameProcessingDelegate>)processingDelegate;

- (id)initWithSharedContext:(FMSharedGLContext*)context width:(float)width height:(float)height;

- (void)drawResult;

- (void)updateFrame;

- (void)clear;

@end


#endif /* fmEmptyEffect_h */
