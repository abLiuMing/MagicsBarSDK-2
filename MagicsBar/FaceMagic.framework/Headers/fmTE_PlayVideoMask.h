//
//  fmTE_PlayVideoMask.h
//  fmFaceTracker
//
//  Created by wysaid on 16/1/18.
//  Copyright © 2016年 wysaid. All rights reserved.
//

#ifndef _FM_TRACKINGEFECT_PLAYVIDEOMASK_H_
#define _FM_TRACKINGEFECT_PLAYVIDEOMASK_H_

#include "fmTrackingEffectHelper.h"
#import "fmVideoPlayer.h"
#include "fmImageFilter.h"

#include "fmTE_TexUtils.h"

namespace FM
{
    class FMTE_PlayVideoMask : public virtual FMTrackingEffectHelper
    {
    public:
        FMTE_PlayVideoMask();
        ~FMTE_PlayVideoMask();
        
        void fire();
        void play(FMImageHandler* handler);
        bool canFire();
        bool shouldTracking();
        bool isPlaying();
        void setStepValue(float value);
        void setVideos(const std::vector<NSURL*>& videos);
        
        void setViewport(const Vec4f& v) { m_videoViewPort = v; }
        
        bool isValid() { return _videoPlayer != nil && m_frameDrawer != nullptr; }
        
        void setFilter(FMImageFilterInterfaceAbstract* filter) { m_filter = filter; }
        
    protected:
        void clearRes();

    protected:
        FMVideoPlayer* _videoPlayer; //oc 对象
        MaskFrameDrawer* m_frameDrawer;
        Vec4f m_videoViewPort; //范围 [0, 1]
        FMImageFilterInterfaceAbstract* m_filter;
    };
}


#endif