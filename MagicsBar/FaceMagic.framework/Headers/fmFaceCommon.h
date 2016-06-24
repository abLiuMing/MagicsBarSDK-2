//
//  fmFaceCommon.h
//  fmFaceTracker
//
//  Created by wysaid on 16/1/17.
//  Copyright © 2016年 wysaid. All rights reserved.
//

#ifndef fmFaceCommon_h
#define fmFaceCommon_h

#import "libFaceMagic-Prefix.pch"

#ifdef __cplusplus
extern "C"
{
#endif
    
#define FM_FACE_EFFECT_VERSION 20160126

typedef enum FMFaceFeature {
    FM_FACE_LEFT_EYE_TOP,
    FM_FACE_RIGHT_EYE_TOP,
    FM_FACE_LEFT_EYEBROW,
    FM_FACE_RIGHT_EYEBROW,
    FM_FACE_LEFT_EYE,
    FM_FACE_RIGHT_EYE,
    FM_FACE_LEFT_JAW,
    FM_FACE_RIGHT_JAW,
    FM_FACE_JAW,
    FM_FACE_OUTER_MOUTH,
    FM_FACE_INNER_MOUTH,
    FM_FACE_NOSE_BRIDGE,
    FM_FACE_NOSE_BASE,
    FM_FACE_OUTLINE,
    FM_FACE_ALL_FEATURES,
    FM_FACE_TOTAL_FEATURE_NUM
}FMFaceFeature;

typedef enum FMTrackingEffectMode
{
    FM_TE_INVALID, //非法特效标记
    
    // 检测到张嘴动作时播放视频
    FM_TE_OPENMOUTH_PLAYVIDEO_ADD,
    
    FM_TE_OPENMOUTH_PLAYVIDEO_MASK,
    
    // 头顶跟踪显示一张图片, 同时检测到张嘴动作时播放视频
    FM_TE_OPENMOUTH_PLAYVIDEO_ADD_x_IMAGE_ABOVE_HEAD,
    
    // 头顶跟踪显示一个视频(视频播放完毕之后停留在最后一帧, 同时检测到张嘴动作时播放另一个视频
    FM_TE_OPENMOUTH_PLAYVIDEO_ADD_x_VIDEO_ABOVE_HEAD,

    FM_TE_OPENMOUTH_PLAYVIDEO_MASK_x_IMAGE_ABOVE_HEAD,
    
    FM_TE_OPENMOUTH_PLAYVIDEO_MASK_x_VIDEO_ABOVE_HEAD,
    
    FM_TE_OPENMOUTH_TRACKINGPLAYVIDEO,
    
    FM_TE_PLAY2MIXEDVIDEO,
    
    FM_TRACKING_EFFECT_TOTAL_NUM
}FMTrackingEffectMode;

typedef enum FMTrackingEffectPlayMode
{
    FM_TE_PLAY_REPEAT, //default
    FM_TE_PLAY_ONCE_STILL,
    FM_TE_PLAY_ONCE_DISAPPEAR,
    FM_TE_PLAY_LOOP
    
}FMTrackingEffectPlayMode;
    
typedef enum FMTrackingEffectFireEvent
{
    FM_TE_FIRE_EVENT_NONE, //default
    FM_TE_FIRE_EVENT_ANY, 
    FM_TE_FIRE_EVENT_FACE_APPEAR,
    FM_TE_FIRE_EVENT_MOUTH_OPEN,
    FM_TE_FIRE_EVENT_EYEBROW_RAISE,
    
    FM_TE_FIRE_EVENT_NUM
}FMTrackingEffectFireEvent;
    
#ifdef __cplusplus
}
#endif

#endif /* fmFaceCommon_h */
