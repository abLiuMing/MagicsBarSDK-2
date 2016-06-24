/*
 * fmVideoPlayerViewHandler.h
 *
 *  Created on: 2015-10-13
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */


// 对外接口， 添加较多中文注释

#import <GLKit/GLKit.h>
#import "fmVideoPlayer.h"

typedef enum FMVideoPlayerViewDisplayMode
{
    FMVideoPlayerViewDisplayModeScaleToFill,
    FMVideoPlayerViewDisplayModeAspectFill,
    FMVideoPlayerViewDisplayModeAspectFit,
    FMVideoPlayerViewDisplayModeDefault = FMVideoPlayerViewDisplayModeScaleToFill
}FMVideoPlayerViewDisplayMode;

@interface FMVideoPlayerViewHandler : NSObject<GLKViewDelegate, FMFrameUpdateDelegate>

@property(nonatomic) FMVideoPlayer* videoPlayer;

@property(nonatomic) float maskAspectRatio; //使用的 mask 宽高比

@property(nonatomic) BOOL shouldResetViewport;
@property(nonatomic) BOOL shouldUpdateViewport;

- (id)initWithGLKView:(GLKView*)glkView;

#pragma mark - 播放相关接口

//此接口将阻塞当前线程直到开始播放为止
- (void)startWithURL:(NSURL*) url;

//此接口当block为nil时将自动播放， 当block存在时需要手动开始播放(可写在block内)
- (void)startWithURL:(NSURL*) url completionHandler:(void (^)(NSError*))block;

- (BOOL)isPlaying;

- (void)restart;
- (void)pause;
- (void)resume;

- (CMTime)videoDuration;    //正在播放的视频总时长
- (CMTime)currentTime;      //视频当前播放时间
- (AVPlayerItemStatus)status; //播放器当前状态

#pragma mark - mask相关接口

// mask设定中， 第一个参数填 nil 或者0 表示不使用mask
- (void)setMaskUIImage :(UIImage*)image;
- (void)setMaskTexture :(GLuint)maskTexture textureAspectRatio:(float)aspectRatio;
- (BOOL)isUsingMask;

#pragma mark - 显示相关的接口

- (void)setViewDisplayMode :(FMVideoPlayerViewDisplayMode)mode;

#pragma mark - 其他接口

- (void)clear;

@end
