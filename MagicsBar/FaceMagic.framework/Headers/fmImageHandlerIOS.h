/*
 * fmImageHandlerIOS.h
 *
 *  Created on: 2015-8-23
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef __fm__fmImageHandlerIOS__
#define __fm__fmImageHandlerIOS__

#import <UIKit/UIKit.h>
#include "fmImageHandler.h"

namespace FM
{
    class FMImageHandlerIOS : public FM::FMImageHandler
    {
    public:
        
        FMImageHandlerIOS();
        ~FMImageHandlerIOS();
        
        bool initWithUIImage(UIImage* uiimage, bool useImageBuffer = true, bool enableRevision = false);
        
        UIImage* getResultUIImage();
        
        void processingFilters();
        
        void swapBufferFBO();
        
        void enableImageBuffer(bool useBuffer);
        bool isImageBufferEnabled() { return m_imageBuffer != nullptr;}
        
    protected:
        
        unsigned char* m_imageBuffer;
        int m_imageBufferLen;
        CGColorSpaceRef m_colorSpace;
        CGFloat m_imageScale;
    };

}


#endif /* defined(__fm__fmImageHandlerIOS__) */
