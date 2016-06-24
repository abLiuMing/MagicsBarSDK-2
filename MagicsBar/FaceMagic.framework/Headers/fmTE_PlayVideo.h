//
//  fmTE_PlayVideo.h
//  fmFaceTracker
//
//  Created by wysaid on 16/1/18.
//  Copyright © 2016年 wysaid. All rights reserved.
//

#ifndef _FM_TRACKINGEFECT_PLAYVIDEO_H_
#define _FM_TRACKINGEFECT_PLAYVIDEO_H_

#include "fmTrackingEffectHelper.h"
#import "fmVideoPlayer.h"

namespace FM
{
    class FMTE_PlayVideo : public virtual FMTrackingEffectHelper
    {
    public:
        FMTE_PlayVideo();
        ~FMTE_PlayVideo();
        
        void fire();
        void play(FMImageHandler* handler);
        bool canFire();
        bool shouldTracking();
        bool isPlaying();
        
        void setVideos(const std::vector<NSURL*>& videos);
        void setViewport(const Vec4f& v) { m_videoViewPort = v; }
        
        bool isValid() { return _videoPlayer != nil; }
        
        void setFilter(FMImageFilterInterfaceAbstract* filter) { m_filter = filter; }
        
    protected:
        void clearRes();
        
    protected:
        FMVideoPlayer* _videoPlayer; //oc 对象
        Vec4f m_videoViewPort; //范围 [0, 1]
        FMImageFilterInterfaceAbstract* m_filter;
    };
}


#endif