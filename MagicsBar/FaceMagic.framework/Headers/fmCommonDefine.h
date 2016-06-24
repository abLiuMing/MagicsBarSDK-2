/*
 * fmCommonDefine.h
 *
 *  Created on: 2013-12-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _FMCOMMONDEFINE_H_
#define _FMCOMMONDEFINE_H_

#include "fmGlobal.h"

#ifdef FM_LOG_ERROR
#define fmCheckGLError(name) _fmCheckGLError(name, __FILE__, __LINE__);
#else
#define fmCheckGLError(name)
#endif
#ifndef _FM_GET_MACRO_STRING_HELP
#define _FM_GET_MACRO_STRING_HELP(x) #x
#endif
#ifndef FM_GET_MACRO_STRING
#define FM_GET_MACRO_STRING(x) _FM_GET_MACRO_STRING_HELP(x)
#endif
#define FM_FLOATCOMP0(x)	(x < 0.001f && x > -0.001f)

#define FM_UNIFORM_MAX_LEN 32

#define FM_DELETE(p) do { delete p; p = NULL; } while(0)
#define FM_DELETE_ARR(p) do { delete[] p; p = NULL; } while(0)

/*
 为节省texture资源，对OpenGL 所有texture的使用约束如下:
 0号和1号纹理在各种初始化中可能会被多次用到，如果需要绑定固定的纹理，
 请在使用纹理时，从 TEXTURE_START 开始。
 不排除这种需要会增加，所以，
 请使用下面的宏进行加法运算, 以代替手写的 GL_TEXTURE*
 
 */

#define FM_TEXTURE_INPUT_IMAGE_INDEX 0
#define FM_TEXTURE_INPUT_IMAGE GL_TEXTURE0

#define FM_TEXTURE_OUTPUT_IMAGE_INDEX 1
#define FM_TEXTURE_OUTPUT_IMAGE GL_TEXTURE1

#define FM_TEXTURE_START_INDEX 2
#define FM_TEXTURE_START GL_TEXTURE2

//Mark if the texture is premultiplied with the alpha channel.
#ifndef FM_TEXTURE_PREMULTIPLIED
#define FM_TEXTURE_PREMULTIPLIED 0
#endif

#ifdef _FM_SHADER_VERSION_

#define FM_GLES_ATTACH_STRING_L "#version " FM_GET_MACRO_STRING(_FM_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision lowp float;\n#endif\n"
#define FM_GLES_ATTACH_STRING_M "#version " FM_GET_MACRO_STRING(_FM_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision mediump float;\n#endif\n"
#define FM_GLES_ATTACH_STRING_H "#version " FM_GET_MACRO_STRING(_FM_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision highp float;\n#endif\n"

#else

#define FM_GLES_ATTACH_STRING_L "#ifdef GL_ES\nprecision lowp float;\n#endif\n"
#define FM_GLES_ATTACH_STRING_M "#ifdef GL_ES\nprecision mediump float;\n#endif\n"
#define FM_GLES_ATTACH_STRING_H "#ifdef GL_ES\nprecision highp float;\n#endif\n"
#endif

//Do not add any precision conf within SHADER_STRING_PRECISION_* macro!
#if defined(_MSC_VER) && _MSC_VER < 1600
//If the m$ compiler is under 10.0, there mustn't be any ',' outside the "()" in the shader string !!
//For exmaple: vec2(0.0, 0.0) --> YES.
//             float a, b;  --> No!!!, you must do like this: float a; float b;
#define FM_SHADER_STRING_PRECISION_L(string) FM_GLES_ATTACH_STRING_L  #string
#define FM_SHADER_STRING_PRECISION_M(string) FM_GLES_ATTACH_STRING_M  #string
#define FM_SHADER_STRING_PRECISION_H(string) FM_GLES_ATTACH_STRING_H  #string
#ifndef FM_SHADER_STRING
#define FM_SHADER_STRING(string) #string
#endif
#else
#define FM_SHADER_STRING_PRECISION_L(...) FM_GLES_ATTACH_STRING_L  #__VA_ARGS__
#define FM_SHADER_STRING_PRECISION_M(...) FM_GLES_ATTACH_STRING_M  #__VA_ARGS__
#define FM_SHADER_STRING_PRECISION_H(...) FM_GLES_ATTACH_STRING_H  #__VA_ARGS__
#ifndef FM_SHADER_STRING
#define FM_SHADER_STRING(...) #__VA_ARGS__
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef const char* const FMConstString;
    
    typedef enum FMBufferFormat
    {
        FM_FORMAT_RGB_INT8,
        FM_FORMAT_RGB_INT16,
        FM_FORMAT_RGB_FLOAT32,
        FM_FORMAT_RGBA_INT8,
        FM_FORMAT_RGBA_INT16,
        FM_FORMAT_RGBA_FLOAT32,
#ifdef GL_BGR
        FM_FORMAT_BGR_INT8,
        FM_FORMAT_BGR_INT16,
        FM_FORMAT_BGR_FLOAT32,
#endif
#ifdef GL_BGRA
        FM_FORMAT_BGRA_INT8,
        FM_FORMAT_BGRA_INT16,
        FM_FORMAT_BGRA_FLOAT32,
#endif
#ifdef GL_LUMINANCE
        FM_FORMAT_LUMINANCE, // 8 bit
#endif
#ifdef GL_LUMINANCE_ALPHA
        FM_FORMAT_LUMINANCE_ALPHA, //8+8 bit
#endif
        
    }FMBufferFormat;
    
    typedef enum FMTextureBlendMode
    {
        FM_BLEND_MIX,            // 0 正常
        FM_BLEND_DISSOLVE,       // 1 溶解
        
        FM_BLEND_DARKEN,         // 2 变暗
        FM_BLEND_MULTIPLY,       // 3 正片叠底
        FM_BLEND_COLORBURN,      // 4 颜色加深
        FM_BLEND_LINEARBURN,     // 5 线性加深
        FM_BLEND_DARKER_COLOR,   // 6 深色
        
        FM_BLEND_LIGHTEN,        // 7 变亮
        FM_BLEND_SCREEN,         // 8 滤色
        FM_BLEND_COLORDODGE,     // 9 颜色减淡
        FM_BLEND_LINEARDODGE,    // 10 线性减淡
        FM_BLEND_LIGHTERCOLOR,  // 11 浅色
        
        FM_BLEND_OVERLAY,        // 12 叠加
        FM_BLEND_SOFTLIGHT,      // 13 柔光
        FM_BLEND_HARDLIGHT,      // 14 强光
        FM_BLEND_VIVIDLIGHT,     // 15 亮光
        FM_BLEND_LINEARLIGHT,    // 16 线性光
        FM_BLEND_PINLIGHT,       // 17 点光
        FM_BLEND_HARDMIX,        // 18 实色混合
        
        FM_BLEND_DIFFERENCE,     // 19 差值
        FM_BLEND_EXCLUDE,        // 20 排除
        FM_BLEND_SUBTRACT,       // 21 减去
        FM_BLEND_DIVIDE,         // 22 划分
        
        FM_BLEND_HUE,            // 23 色相
        FM_BLEND_SATURATION,     // 24 饱和度
        FM_BLEND_COLOR,          // 25 颜色
        FM_BLEND_LUMINOSITY,     // 26 明度
        
        /////////////    More blend mode below (You can't see them in Adobe Photoshop)    //////////////
        
        FM_BLEND_ADD,			  // 27
        FM_BLEND_ADDREV,         // 28
        FM_BLEND_COLORBW,		  // 29
        
        /////////////    More blend mode above     //////////////
        
        FM_BLEND_TYPE_MAX_NUM //Its value defines the max num of blend.
    }FMTextureBlendMode;
    
    typedef enum FMGlobalBlendMode
    {
        FMGLOBAL_BLEND_NONE,
        FMGLOBAL_BLEND_ALPHA,
        FMGLOBAL_BLEND_ALPHA_SEPERATE,
        FMGLOBAL_BLEND_ADD,
        FMGLOBAL_BLEND_ADD_SEPARATE,
        FMGLOBAL_BLEND_ADD_SEPARATE_EXT, //带EXT的忽略alpha是否预乘
        FMGLOBAL_BLEND_MULTIPLY,
        FMGLOBAL_BLEND_MULTIPLY_SEPERATE,
        FMGLOBAL_BLEND_SCREEN,
        FMGLOBAL_BLEND_SCREEN_EXT,
    }FMGlobalBlendMode;
    
    const char* fmGetVersion();
    void fmPrintGLString(const char*, GLenum);
    void _fmCheckGLError(const char* name, const char* file, int line); //请直接使用 fmCheckGLError
    
    ////////////////////////////////////

    void fmSetGlobalBlendMode(const FMGlobalBlendMode mode);
    void fmGetDataAndChannelByFormat(FMBufferFormat fmt, GLenum* dataFmt, GLenum* channelFmt, GLint* channel);
    
#ifdef __cplusplus
    const char* fmGetBlendModeName(const FMTextureBlendMode mode, bool withChinese = false);
    GLuint fmGenTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt, GLint channels = 4, GLint bindID = 0, GLenum texFilter = GL_LINEAR, GLenum texWrap = GL_CLAMP_TO_EDGE);
#else
    const char* fmGetBlendModeName(const FMTextureBlendMode mode, bool withChinese);
    GLuint fmGenTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt, GLint channels, GLint bindID , GLenum texFilter, GLenum texWrap);
#endif
    
#ifdef __cplusplus
}
#endif

#endif /* _FMCOMMONDEFINE_H_ */
