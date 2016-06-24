//
//  fmTE_PlayVideoTransparent.h
//  fmFaceTracker
//
//  Created by wysaid on 16/4/11.
//  Copyright © 2016年 wysaid. All rights reserved.
//

#ifndef _FM_TRACKINGEFECT_PLAYVIDEOTRANSPARENT_H_
#define _FM_TRACKINGEFECT_PLAYVIDEOTRANSPARENT_H_

#import "fmTE_PlayVideo.h"

namespace FM
{
    class FMTE_TextureDrawerTransparent;
    
    class FMTE_PlayVideoTransparent : public FMTE_PlayVideo
    {
    public:
        FMTE_PlayVideoTransparent();
        ~FMTE_PlayVideoTransparent();
        
        void play(FMImageHandler* handler);
        
        bool isValid() { return _videoPlayer != nil && m_transparentDrawer != nullptr; }
        
    protected:
        FMTE_TextureDrawerTransparent* m_transparentDrawer;
    };
}


#endif