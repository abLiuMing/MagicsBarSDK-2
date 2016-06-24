/*
* fmGlobal.h
*
*  Created on: 2014-9-9
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#ifndef _FMGLOBAL_H_
#define _FMGLOBAL_H_

#ifdef GLEW_USED
#include "fmPlatform_GLEW.h"
#elif defined(ANDROID_NDK)
#include "fmPlatform_ANDROID.h"
#elif defined(IOS_SDK)
#include "fmPlatform_IOS.h"
#elif defined(LIBFM4QT_LIB)
#include "fmPlatform_QT.h"
#endif


#ifdef __cplusplus

namespace FM
{
	//辅助类，全局可用。
	class FMGlobalConfig
	{
	public:
		static int viewWidth, viewHeight;

		static GLuint sVertexBufferCommon;
		static float sVertexDataCommon[8];

		enum InitArguments
		{
			FM_INIT_LEAST = 0,
			FM_INIT_COMMONVERTEXBUFFER = 0x1,
			FM_INIT_SPRITEBUILTIN = 0x3,
			FM_INIT_DEFAULT = 0xffffffff,
		};

		static InitArguments sInitArugment;
	};

	//FM 全局初始化函数。
	bool fmInitialize(int w = FMGlobalConfig::viewWidth, int h = FMGlobalConfig::viewHeight, FMGlobalConfig::InitArguments arg = FMGlobalConfig::FM_INIT_DEFAULT);

	inline bool fmInitialize(int w, int h, GLenum arg)
	{
		return fmInitialize(w, h, FMGlobalConfig::InitArguments(arg));
	}

	void fmInitFilterStatus();

	//FM 全局清除函数
	void fmCleanup();

	//设置画面显示尺寸
	void fmSetGlobalViewSize(int width, int height);

}

#endif

#ifdef __cplusplus
extern "C" {
#endif
    
    void fmPrintGLInfo();
    const char* fmQueryGLExtensions();
    bool fmCheckGLExtension(const char* ext);

	GLuint fmGenCommonQuadArrayBuffer();
    
#ifdef __cplusplus
}
#endif

#endif