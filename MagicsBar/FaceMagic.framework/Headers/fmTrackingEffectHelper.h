//
//  fmTrackingEffectHelper.h
//  faceTracker
//
//  Created by wysaid on 16/1/18.
//  Copyright © 2016年 wysaid. All rights reserved.
//

// 对内辅助头文件， 直接引用将产生c++依赖

#ifndef FM_TRACKINGEFFECTHELPER_H_
#define FM_TRACKINGEFFECTHELPER_H_

#import <GLKit/GLKit.h>
#include "fmFaceCommon.h"
#include "fmFaceTracker.h"
#include "fmCommonDefine.h"
#include "fmImageHandler.h"
#include "fmSprite2d.h"

namespace FM
{
    class FMTrackingEffectHelper
    {
    public:
        
        enum SpriteDisplayMode
        {
            SPRITE_FULL_SCREEN,
            SPRITE_FIT_TOP,
            SPRITE_FIT_BOTTOM,
            SPRITE_TOP_ORIGIN,
            SPRITE_CENTER_ORIGIN,
            SPRITE_BOTTOM_ORIGIN,
        };
        
        static inline SpriteDisplayMode getSpriteDisplayModeByname(NSString* name)
        {
            SpriteDisplayMode mode = SPRITE_FULL_SCREEN;
            name = [name lowercaseString];
            
            if([name compare:@"fittop"] == NSOrderedSame)
            {
                mode = SPRITE_FIT_TOP;
            }
            else if([name compare:@"fitbottom"] == NSOrderedSame)
            {
                mode = SPRITE_FIT_BOTTOM;
            }
            else if([name compare:@"toporigin"] == NSOrderedSame)
            {
                mode = SPRITE_TOP_ORIGIN;
            }
            else if([name compare:@"centerorigin"] == NSOrderedSame)
            {
                mode = SPRITE_CENTER_ORIGIN;
            }
            else if([name compare:@"bottomorigin"] == NSOrderedSame)
            {
                mode = SPRITE_BOTTOM_ORIGIN;
            }
            
            return mode;
        }
        
        static inline void initSpriteByDisplayMode(SpriteInterface2d* sprite, float canvasWidth, float canvasHeight, float spriteWidth, float spriteHeight, SpriteDisplayMode mode)
        {
            float xScale, yScale;
            switch (mode)
            {
                default:
                case SPRITE_FULL_SCREEN:
                    xScale = canvasWidth / spriteWidth;
                    yScale = canvasHeight / spriteHeight;
                    sprite->scaleTo(xScale, yScale);
                    sprite->moveTo(canvasWidth / 2.0f, canvasHeight / 2.0f);
                    break;
                case SPRITE_FIT_TOP:
                    xScale = canvasWidth / spriteWidth;
                    sprite->scaleTo(xScale, xScale);
                    sprite->setHotspot(0.0f, -1.0f);
                    sprite->moveTo(canvasWidth / 2.0f, 0.0f);
                    break;
                case SPRITE_FIT_BOTTOM:
                    xScale = canvasWidth / spriteWidth;
                    sprite->scaleTo(xScale, xScale);
                    sprite->setHotspot(0.0f, 1.0f);
                    sprite->moveTo(canvasWidth / 2.0f, canvasHeight);
                    break;
                case SPRITE_TOP_ORIGIN:
                    sprite->setHotspot(0.0f, -1.0f);
                    sprite->moveTo(canvasWidth / 2.0f, 0.0f);
                    break;
                case SPRITE_BOTTOM_ORIGIN:
                    sprite->setHotspot(0.0f, 1.0f);
                    sprite->moveTo(canvasWidth / 2.0f, canvasHeight);
                    break;
                case SPRITE_CENTER_ORIGIN:
                    sprite->moveTo(canvasWidth / 2.0f, canvasHeight / 2.0f);
                    break;
            }
        }
        
        //////////////////////////////////////////
        
        FMTrackingEffectHelper();
        FMTrackingEffectHelper(int trackerFlag);
        virtual ~FMTrackingEffectHelper();
        
        virtual void fire(); //触发这个效果
        virtual void update(double during); //每一帧更新
        virtual void play(FMImageHandler* handler) = 0; //最终渲染效果
        virtual bool canFire(); //判断是否可以fire
        virtual bool shouldTracking(); //优化手段， 当不需要人脸检测时返回false
        virtual bool isPlaying(); //是否正在播放, 返回false不会调用play
        
        virtual void setVideos(const std::vector<NSURL*>& videos);
        virtual void setImages(const std::vector<UIImage*>& imgs);
        
        //使用文件名精确加载， 当文件数量较多时， 可以进行内部优化.
        virtual void setVideos(const std::vector<NSString*>& videos);
        virtual void setImages(const std::vector<NSString*>& imgs);
        
        virtual bool isValid() { return false; } // 判断最终特效是否初始化完毕(合法)
        
        inline FMFaceTracker& getFaceTracker() { return m_faceTracker; }
        
        inline FMTrackingEffectFireEvent getFireEvent() { return m_event; }
        inline void setFireEvent(FMTrackingEffectFireEvent e) { m_event = e; }
        
        inline FMTrackingEffectPlayMode getPlayMode() { return m_playMode; }
        inline void setPlayMode(FMTrackingEffectPlayMode mode) { m_playMode = mode; }
        
        static int sCanvasWidth, sCanvasHeight;
        
        inline void setFilter(FMImageFilterInterfaceAbstract* filter) { m_filter = filter; }
        
    protected:
        FMFaceTracker m_faceTracker;
        FMTrackingEffectFireEvent m_event;
        FMTrackingEffectPlayMode m_playMode;
        FMImageFilterInterfaceAbstract* m_filter;
    };
}

#endif