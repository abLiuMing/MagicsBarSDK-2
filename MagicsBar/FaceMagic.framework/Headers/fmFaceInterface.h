//
//  fmFaceInterface.h
//  fmFaceTracker
//
//  Created by wysaid on 16/1/14.
//  Copyright (c) 2015年 wysaid. All rights reserved.
//

#ifndef _fmFaceInterface_h_
#define _fmFaceInterface_h_

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <UIKit/UIKit.h>
#include "fmFaceCommon.h"

@interface FMFaceInterface : NSObject

@property (nonatomic) BOOL hasFace;
@property (nonatomic) BOOL isMouthOpen;

- (void)findFaceByCVImageBuffer :(CVImageBufferRef)cvImageBufferRef;

- (UIImage*)findFaceByUIImage :(UIImage*)uiimage;

- (void*) getTracker;

- (CGPoint)getMouthPos;

@end


#endif
