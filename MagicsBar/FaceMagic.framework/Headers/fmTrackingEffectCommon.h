//
//  fmTrackingEffectCommon.h
//  fmFaceTracker
//
//  Created by wysaid on 16/1/18.
//  Copyright © 2016年 wysaid. All rights reserved.
//

//对外接口头文件, 直接引用不产生 c++ 依赖

#ifndef _FM_TRACKINGEFFECTCOMMON_H_
#define _FM_TRACKINGEFFECTCOMMON_H_

#import <Foundation/Foundation.h>
#import "fmCameraFrameRecorder.h"
#import "fmFaceCommon.h"

@interface FMTrackingEffectCommon : NSObject<FMFrameProcessingDelegate>

@property (nonatomic, weak)FMSharedGLContext* weakSharedContext;

@property (nonatomic, assign)BOOL requestRGBA;

@property (nonatomic) BOOL hasFace;
@property (nonatomic) BOOL isMouthOpen;

- (void)clear;

- (BOOL)processingHandleBuffer :(CVImageBufferRef)imageBuffer;

//全局初始化特效
+ (BOOL)initTrackingEffects: (int)width height:(int)height;
//全局清理特效
+ (void)clearTrackingEffects;

//通过指定目录资源创建特效
+ (instancetype)createTrackingEffectByPath:(NSString*)path procAsync:(BOOL)procAsyc;

//通过指定bundle创建特效
+ (instancetype)createTrackingEffectByBundle:(NSBundle*)bundle procAsync:(BOOL)procAsyc;

//通过指定目录资源或者bundle创建特效 (其中一个为 NULL)
+ (instancetype)createTrackingEffect:(NSString*)path bundle:(NSBundle*)bundle procAsync:(BOOL)procAsync;

@end


#endif