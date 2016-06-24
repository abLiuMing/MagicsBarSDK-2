//
//  fmEmptyEffectViewHandler.h
//  fmFaceTracker
//
//  Created by wysaid on 16/4/10.
//  Copyright © 2016年 wysaid. All rights reserved.
//

#ifndef fmEmptyEffectViewHandler_h
#define fmEmptyEffectViewHandler_h

#import "fmEmptyEffect.h"

@interface FMEmptyEffectViewHandler : NSObject<GLKViewDelegate, FMFrameUpdateDelegate>

@property(nonatomic, weak) GLKView* glkView;

@property(nonatomic) FMEmptyEffect* emptyEffect;

- (id)initWithGLKView:(GLKView*)glkView width:(float)width height:(float)height;

- (void)clear;

//- (void)updateFrame;

@end

#endif /* fmEmptyEffectViewHandler_h */
