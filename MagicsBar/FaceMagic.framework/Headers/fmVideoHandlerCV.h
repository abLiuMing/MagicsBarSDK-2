/*
 * fmVideoHandlerCV.h
 *
 *  Created on: 2015-9-8
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _FMVIDEOHANLERCV_H_
#define _FMVIDEOHANLERCV_H_

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <UIKit/UIKit.h>
#import "fmImageHandler.h"
#import "fmTextureUtils.h"

namespace FM
{
    class FMVideoHandlerCV : public FMImageHandler
    {
    public:
        FMVideoHandlerCV();
        ~FMVideoHandlerCV();
        
        bool initHandler();
        
        bool updateFrameWithCVImageBuffer(CVImageBufferRef);
        
        void cleanupYUVTextures();
        
        void processingFilters();
        
        inline TextureDrawerYUV* getYUVDrawer() { return m_yuvDrawer; }
        
        inline void replaceYUVDrawer(TextureDrawerYUV* drawer, bool shouldDelete = true)
        {
            if(shouldDelete)
                delete m_yuvDrawer;
            m_yuvDrawer = drawer;
        }

        bool reverseTargetSize() { return m_reverseTargetSize; }

        void setReverseTargetSize(bool rev) { m_reverseTargetSize = rev; }
        
    protected:
        
        void swapBufferFBO();
        
    private:
        
        CVOpenGLESTextureCacheRef m_videoTextureCacheRef;
        CVOpenGLESTextureRef m_lumaTextureRef;
        CVOpenGLESTextureRef m_chromaTextureRef;
        
        TextureDrawerYUV* m_yuvDrawer;
        bool m_reverseTargetSize;
    };
}


#endif