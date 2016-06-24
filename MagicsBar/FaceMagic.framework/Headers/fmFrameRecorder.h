/*
 * fmFrameRecorder.h
 *
 *  Created on: 2015-12-1
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

// 主要作为辅助类使用

#ifndef _FM_FRAME_RECORDER_H_
#define _FM_FRAME_RECORDER_H_

#import <Foundation/Foundation.h>
#import "fmVideoWriter.h"
#import "fmFrameRenderer.h"

@interface FMFrameRecorder : FMFrameRenderer

@property(nonatomic, readonly) NSURL* outputVideoURL;
@property(nonatomic, readonly) NSURL* cacheVideoURL;

@property(nonatomic, weak) id<FMFrameUpdateDelegate> updateDelegate;
@property(nonatomic, readonly) FMVideoWriter* videoWriter;
@property(nonatomic, readonly) BOOL isRecording;
@property(nonatomic) BOOL shouldPassthroughAudio;

#pragma mark - 初始化相关

- (id)initWithContext :(FMSharedGLContext*)sharedContext;

// 手动调用释放
- (void)clear;


#pragma mark - 录像相关接口

- (void)startRecording :(NSURL*)videoURL size:(CGSize)videoSize;
- (void)startRecording :(NSURL*)videoURL size:(CGSize)videoSize outputSetting:(NSDictionary*)ouputSetting;
- (void)endRecording :(void (^)(void))completionHandler; //默认 压缩程度 中等
- (void)endRecording :(void (^)(void))completionHandler withCompressionLevel:(int)level; //level 取值范围为 [0, 3], 0为不压缩， 1 清晰度较高， 文件较大, 2 中等, 3 清晰度较低， 文件较小

- (void)endRecording:(void (^)(void))completionHandler withQuality:(NSString*)quality shouldOptimizeForNetworkUse:(BOOL)shouldOptimize; //quality为 AVAssetExportPreset*

- (void)cancelRecording;

@end


#endif