/*﻿
* fmImageFilter.h
*
*  Created on: 2013-12-13
*      Author: Wang Yang
*/

#ifndef _FMIMAGEFILTER_H_
#define _FMIMAGEFILTER_H_

#include "fmGLFunctions.h"
#include "fmShaderFunctions.h"

#ifndef FM_CURVE_PRECISION
#define FM_CURVE_PRECISION 256
#endif

namespace FM
{

	extern FMConstString g_vshDefault;
	extern FMConstString g_vshDefaultWithoutTexCoord;
	extern FMConstString g_vshDrawToScreen;
	extern FMConstString g_vshDrawToScreenRot90;
	extern FMConstString g_fshDefault;
	extern FMConstString g_fshFastAdjust;
	extern FMConstString g_fshFastAdjustRGB;
	extern FMConstString g_fshCurveMapNoIntensity;

	extern FMConstString g_vshTest;
	extern FMConstString g_fshTest;

	extern FMConstString g_paramFastAdjustArrayName;
	extern FMConstString g_paramFastAdjustRGBArrayName;
	extern FMConstString g_paramCurveMapTextureName;


	class FMImageHandlerInterface;

	class FMImageFilterInterface;

	class FMImageFilterInterfaceAbstract
	{
	public:
		FMImageFilterInterfaceAbstract() { FM_ENABLE_GLOBAL_GLCONTEXT(); }
		virtual ~FMImageFilterInterfaceAbstract() {}
		virtual void render2Texture(FMImageHandlerInterface* handler, GLuint srcTexture, GLuint vertexBufferID) = 0;

		virtual void setIntensity(float value) {}

		//mutiple effects专有， 若返回为 true， handler在添加filter时会进行拆解。
		virtual bool isWrapper() { return false; }
		virtual std::vector<FMImageFilterInterface*> getFilters(bool bMove = true) { return std::vector<FMImageFilterInterface*>(); }
		
	};

	class FMImageFilterInterface : public FMImageFilterInterfaceAbstract
	{
	public:
		FMImageFilterInterface();
		virtual ~FMImageFilterInterface();

		virtual void render2Texture(FMImageHandlerInterface* handler, GLuint srcTexture, GLuint vertexBufferID);

		//////////////////////////////////////////////////////////////////////////
		bool initShadersFromString(const char* vsh, const char* fsh);
		//bool initShadersFromFile(const char* vshFileName, const char* fshFileName);

		void setAdditionalUniformParameter(UniformParameters* param);
		UniformParameters* getUniformParam() { return m_uniformParam; }

		virtual bool init() { return false; }

		ProgramObject& getProgram() { return m_program; }

		static FMConstString paramInputImageName;
		static FMConstString paramPositionIndexName;

	//protected:
		//////////////////////////////////////////////////////////////////////////
		//virtual bool initVertexShaderFromString(const char* vsh);
		//	virtual bool initVertexShaderFromFile(const char* vshFileName);

		//virtual bool initFragmentShaderFromString(const char* fsh);	
		//	virtual bool initFragmentShaderFromFile(const char* fshFileName);

		//virtual bool finishLoadShaders(); //如果单独调用上方函数初始化，请在结束后调用本函数。	

	protected:
		ProgramObject m_program;

		//See the description of "UniformParameters" to know "How to use it".
		UniformParameters* m_uniformParam;
	};

	class FMFastAdjustFilter : public FMImageFilterInterface
	{
	public:

		typedef struct CurveData
		{
			float data[3];

			float& operator[](int index)
			{
				return data[index];
			}

			const float& operator[](int index) const
			{
				return data[index];
			}
		}CurveData;

		bool init();

	protected:
		static FMConstString paramArray;
		void assignCurveArrays();
		void initCurveArrays();

	protected:
		std::vector<CurveData> m_curve;
	};

	class FMFastAdjustRGBFilter : public FMImageFilterInterface
	{
	public:

		bool init();

	protected:
		static FMConstString paramArrayRGB;
		void assignCurveArray();
		void initCurveArray();

	protected:
		std::vector<float> m_curveRGB;
	};

}

#endif
