//
//  FMTE_PlayImageAnims.h
//  qdFaceSDK
//
//  Created by wysaid on 16/4/17.
//  Copyright © 2016年 wysaid. All rights reserved.
//


#ifndef FMTE_PlayImageAnim_h
#define FMTE_PlayImageAnim_h

#include "fmTrackingEffectHelper.h"
#include "fmTE_TexUtils.h"
#import  <Foundation/Foundation.h>

namespace FM
{
    class Sprite2dSequence;
    
    class FMTE_PlayImageAnim : public FMTrackingEffectHelper
    {
    public:
        struct Element {
            Element() : spriteSequence(nullptr),target(-1),rotation(1),scalingX(1.0f),scalingY(1.0f){}
            ~Element(){delete spriteSequence;}
            Sprite2dSequenceExt* spriteSequence;
            int target;
            bool rotation;
            double scalingX,scalingY;
        };
        
        FMTE_PlayImageAnim();
        ~FMTE_PlayImageAnim();
        
        void update(double during);
        void play(FMImageHandler* handler);
        bool canFire();
        bool shouldTracking(){return true;};
        bool isPlaying();
        void putSprite();
        
        void setElements(const std::vector<NSDictionary*>& elements, NSString* path);
        
        bool isValid() { return !m_elements.empty(); }
        
    protected:
        std::vector<Element *>m_elements;
    };
}


#endif /* FMTE_PlayImageAnim_h */