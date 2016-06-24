//
//  fmTE_TexUtils.h
//  fmFaceTracker
//
//  Created by wysaid on 16/1/27.
//  Copyright © 2016年 wysaid. All rights reserved.
//

#ifndef _fmTE_TexUtils_h_
#define _fmTE_TexUtils_h_

#include "fmFaceCommon.h"
#include "fmSprite2d.h"
#include "fmTextureUtils.h"
#include <list>

namespace FM
{
    class MaskFrameDrawer : public TextureDrawer
    {
    public:
        FM_TEXTURE_DRAWER_STATIC_FUNC(MaskFrameDrawer);
        
        void setStepValue(float value);
        
    protected:
        FMConstString getFragmentShaderString();
    };
    
    class MaskSprite2d : public Sprite2d
    {
        MaskSprite2d(GLuint textureID, int width, int height) : Sprite2d(textureID, width, height, false)
        {
            
        }
        
        bool initSprite();
        
    public:
        
        ~MaskSprite2d();
        
        static MaskSprite2d* create(GLuint texID, int width, int height)
        {
            MaskSprite2d* sprite = new MaskSprite2d(texID, width, height);
            if(!sprite->initSprite())
            {
                delete sprite;
                sprite = nullptr;
            }
            else
            {
                sprite->setStepValue(0.3f);
                sprite->setSpriteFlip(false, true);
                sprite->setHotspot(0.0f, -1.2f);
            }
            
            return sprite;
        }
        
        void setStepValue(float value)
        {
            m_program.bind();
            m_program.sendUniformf(paramStepValue, value);
        }
        
    protected:
        static FMConstString paramStepValue;
    };
    
    class Mix2VideoSprite2d : public Sprite2d
    {
        Mix2VideoSprite2d(GLuint colorTex, GLuint alphaTex, int width, int height) : Sprite2d(colorTex, width, height, false), m_alphaTexture(alphaTex)
        {
            
        }
        
        bool initSprite();
        
    public:
        
        ~Mix2VideoSprite2d();
        
        static Mix2VideoSprite2d* create(GLuint colorTex, GLuint alphaTex, int width, int height)
        {
            Mix2VideoSprite2d* sprite = new Mix2VideoSprite2d(colorTex, alphaTex, width, height);
            if(!sprite->initSprite())
            {
                delete sprite;
                sprite = nullptr;
            }
//            else
//            {
//                sprite->setSpriteFlip(false, true);
//            }

            return sprite;
        }
        
    protected:
        GLuint m_alphaTexture;
        
        void _drawFunc();
    };

    class Sprite2dSequenceExt : public Sprite2dSequence
    {
    public:
        Sprite2dSequenceExt(int width, int height) : Sprite2dSequence(width, height), m_texturesToLoad(nullptr) {}
        ~Sprite2dSequenceExt();

        void setFramesDelayLoading(std::list<NSString*>* v) { m_texturesToLoad = v; }

        void updateFrame(double dt);

    protected:
        void _loadRemainImages(int cnt);

    protected:
        std::list<NSString*>* m_texturesToLoad;
    };
    
}

#endif