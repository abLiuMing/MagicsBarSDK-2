/*
 * fmVideoPlayer.h
 *
 *  Created on: 2015-10-14
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#import <Foundation/Foundation.h>
#import "fmFrameRenderer.h"

typedef enum FMVideoPlayerLoadingStatus
{
    FMVideoPlayerLoadingStatus_Loading,
    FMVideoPlayerLoadingStatus_LoadOK,
    FMVideoPlayerLoadingStatus_LoadFailed,
}FMVideoPlayerLoadingStatus;

@class FMVideoPlayer;

@protocol FMVideoPlayerDelegate <NSObject>

// 视频播放完毕
@required
- (void)videoPlayingComplete:(FMVideoPlayer*)player playItem:(AVPlayerItem*)item;

// 视频分辨率改变 (在 startWithURL 调用后触发)
@optional
- (void)videoResolutionChanged: (CGSize)size;

// 播放时间更新
@optional
- (void)playTimeUpdated:(CMTime)currentTime;

// 设定播放时间更新频率 (当未实现 playTimeUpdateInterval 方法时， 默认为一秒 CMTimeMakeWithSeconds(1, 1) )
@optional
- (CMTime)playTimeUpdateInterval;

// 播放器状态改变
@optional
- (void)playerStatusChanged:(AVPlayerItemStatus)status;

@end

@interface FMVideoPlayer : FMFrameRenderer

@property AVPlayer* avPlayer;
@property AVPlayerItemVideoOutput* videoOutput;
@property CADisplayLink* displayLink;

// 视频分辨率
@property(nonatomic) CGSize videoResolution;

@property(nonatomic, weak) id<FMFrameUpdateDelegate> updateDelegate;
@property(nonatomic, weak) id<FMVideoPlayerDelegate> playerDelegate;

@property(nonatomic, assign) FMVideoPlayerLoadingStatus loadingStatus;

#pragma mark - 初始化相关

- (id)initWithContext:(FMSharedGLContext *)sharedContext;

#pragma mark - 视频 播放/设置 相关

- (void)startWithURL:(NSURL*) url;
- (void)startWithURL:(NSURL*) url completionHandler:(void (^)(NSError*))block;

- (BOOL)isPlaying;

- (void)restart;
- (void)pause;
- (void)resume;

#pragma mark - --其他--

// 视频处理方法
- (void)processVideoFrame:(CVPixelBufferRef)pixBuffer;

@end
