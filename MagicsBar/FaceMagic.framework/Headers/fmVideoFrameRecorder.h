/*
 * fmVideoFrameRecorder.h
 *
 *  Created on: 2015-10-10
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#import <Foundation/Foundation.h>
#import "fmFrameRecorder.h"
#import "fmCommonDefine.h"

// 'fmFrameRecorder' is used for offscreen processing, no sounds play back. (Sounds would be saved in the result)

@class FMVideoFrameRecorder;

@protocol FMVideoFrameRecorderDelegate <NSObject>

@required
- (void)videoReadingComplete:(FMVideoFrameRecorder*)videoFrameRecorder;

@optional
- (void)videoResolutionChanged: (CGSize)size;

@end

@interface FMVideoFrameRecorder : FMFrameRecorder

@property(nonatomic, readonly) AVAssetReader* videoAssetReader;
@property(atomic) BOOL videoLoopRunning;
@property(atomic) BOOL audioLoopRunning;
@property(nonatomic) BOOL mute;

//YES 表示使用实际播放速度进行播放
//NO 表示使用最大速度进行播放(主要用于后台处理)
@property(nonatomic) BOOL playAtActualSpeed;

// 播放进度, 范围 [0, 1]
@property(nonatomic) float progress;

// 总时长
@property(nonatomic) CMTime duration;

@property(nonatomic) CGSize videoResolution;

@property(nonatomic, weak) id<FMVideoFrameRecorderDelegate> videoFrameRecorderDelegate;

- (id)initWithContext:(FMSharedGLContext *)sharedContext;

- (void)setupWithAsset:(AVAsset*)asset completionHandler:(void (^)(BOOL))block;
- (void)setupWithURL:(NSURL*)url completionHandler:(void (^)(BOOL))block;

- (void)start; //启动 (注意， 本类不提供暂停继续快进等功能， 一旦停止， 只能重新启动)
- (void)end;   //停止, 直接调用 end 结束时，(videoFrameRecorderDelegate videoReadingComplete:) 不会被调用


- (void)startRecording :(NSURL*)videoURL; // 启动录制， 并使用视频完整分辨率保存 (必须在 start之后调用)

@end




