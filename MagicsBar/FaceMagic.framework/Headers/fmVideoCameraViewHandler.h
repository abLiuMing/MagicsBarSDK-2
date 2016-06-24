/*
 * fmVideoCameraViewHandler.h
 *
 *  Created on: 2015-9-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

// 对外接口， 添加较多中文注释

#import <GLKit/GLKit.h>
#import "fmCameraFrameRecorder.h"

@interface FMCameraViewHandler : NSObject<GLKViewDelegate, FMFrameUpdateDelegate>

@property(nonatomic, readonly) FMCameraFrameRecorder* cameraRecorder;

@property(nonatomic) BOOL shouldResetViewport;

- (id)initWithGLKView:(GLKView*)glkView;

#pragma mark - 相机设定相关接口

- (AVCaptureDevicePosition)cameraPosition;
- (BOOL)switchCamera :(BOOL)isFrontCameraMirrored;
- (AVCaptureFlashMode)getFlashMode;
- (BOOL)setCameraFlashMode :(AVCaptureFlashMode)flashMode;
- (AVCaptureTorchMode)getTorchMode;
- (BOOL)setTorchMode :(AVCaptureTorchMode)torchMode;

- (BOOL)setupCamera; //默认后置摄像头
- (BOOL)setupCamera :(NSString*)sessionPreset cameraPosition:(AVCaptureDevicePosition)cameraPosition isFrontCameraMirrored:(BOOL)isFrontCameraMirrored authorizationFailed:(void (^)(void))authorizationFailed;
- (BOOL)focusPoint: (CGPoint)point; //点按对焦, point 范围 [0, 1]， focus位置在显示区域的相对位置

- (FMCameraDevice*) cameraDevice;

#pragma mark - 拍照相关接口

- (void)setCameraSessionPreset:(NSString*)sessionPreset;

- (void)takeShot :(void (^)(UIImage*))block;

#pragma mark - 录像相关接口

- (void)startRecording :(NSURL*)videoURL size:(CGSize)videoSize;
- (void)startRecording :(NSURL*)videoURL size:(CGSize)videoSize cropArea:(CGRect)cropArea; //cropArea参见fmVideoWriter相关解释

- (void)endRecording :(void (^)(void))completionHandler;
- (void)endRecording :(void (^)(void))completionHandler withCompressionLevel:(int)level;//level 取值范围为 [0, 3], 0为不压缩， 1 清晰度较高，
- (void)endRecording:(void (^)(void))completionHandler withQuality:(NSString*)quality shouldOptimizeForNetworkUse:(BOOL)shouldOptimize; //quality为 AVAssetExportPreset*

- (void)cancelRecording;
- (BOOL)isRecording;

#pragma mark - mask相关接口

//mask设定中， 第一个参数填 nil 或者0 表示不使用mask
- (void)setMaskUIImage :(UIImage*)image;
- (void)setMaskTexture :(GLuint)maskTexture textureAspectRatio:(float)aspectRatio;
- (BOOL)isUsingMask;

- (BOOL)isGlobalFilterEnabled;

#pragma mark - 其他接口

//- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect;
- (void)clear;

- (void)fitViewSizeKeepRatio :(BOOL)shouldFit;

@end
