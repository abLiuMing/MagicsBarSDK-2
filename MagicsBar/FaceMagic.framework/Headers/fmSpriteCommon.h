/*
* fmSpriteCommon.h
*
*  Created on: 2014-9-25
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#if !defined(_FMSPRITECOMMON_H_) && !defined(_FM_ONLY_FILTERS_)
#define _FMSPRITECOMMON_H_

#include "fmMat.h"
#include "fmGLFunctions.h"
#include "fmShaderFunctions.h"

//简介： 本文件中包含一些sprite需要使用到的辅助类和函数

namespace FM
{
	void fmSpritesInitBuiltin();
	void fmSpritesCleanupBuiltin();

	class SpriteCommonSettings
	{
	public:
		SpriteCommonSettings();
		virtual ~SpriteCommonSettings();

		static FMSizei sCanvasSize; //Sprite2d 被绘制的画布大小（全局, 必须提前设置）

		inline static void sSetCanvasSize(int w, int h)
		{
			sCanvasSize.set(w, h);
			sOrthoProjectionMatrix = Mat4::makeOrtho(0.0f, (float)w, 0.0f, (float)h, -1e6f, 1e6f);
		}

		//仅对后面将要创建的 sprite 进行全局默认设置。 对于已经创建好的sprite，可以通过 set*Flip 函数进行单独处理
		static void sFlipCanvas(bool x, bool y); 
		static void sFlipSprite(bool x, bool y); 

        FM_LOG_CODE
        (
         static std::vector<SpriteCommonSettings*>& getDebugManager();
        )
        
	protected:

		static Mat4 sOrthoProjectionMatrix;
		static bool sCanvasFlipX, sCanvasFlipY;
		static bool sSpriteFlipX, sSpriteFlipY;

	};

	class GeometryLineStrip2d
	{
	public:
		GeometryLineStrip2d();
		~GeometryLineStrip2d();

		inline void setLineWidth(float lineWidth)
		{
			m_lineWidth = lineWidth;
		}

		inline void setColor(float r, float g, float b, float a = 1.0f)
		{
			m_color = Vec4f(r, g, b, a);
		}

		inline void setGradient(float gradient)
		{
			m_gradient = gradient;
		}

		inline void clearPoints() { m_points.clear(); }

		inline void pushPoint(const Vec2f& pnt) { m_points.push_back(pnt); }
		inline void pushPoint(float x, float y) { Vec2f pnt(x, y); m_points.push_back(pnt); }
		inline void pushPoints(std::vector<Vec2f> v);
		void flush();

		void render();


		static float sFlipX, sFlipY;
		static inline void sSetFlip(bool fx, bool fy)
		{
			sFlipX = fx ? -1.0f : 1.0f;
			sFlipY = fy ? -1.0f : 1.0f;
		}

		static void sClearProgram();

		static void inline sSetLineFlip(float fx, float fy)
		{
			s_program->bind();
			s_program->sendUniformf(paramLineFlipName, fx, fy);
		}

	protected:
		static FMConstString paramAttribPositionName;
		static FMConstString paramAttribLineDataName;
		static FMConstString paramLineWidthName;
		static FMConstString paramLineFlipName;
		static FMConstString paramColorName;
		static FMConstString paramGradientName;

		static GLint s_lineWidthLocation;
		static GLint s_lineFlipLocation;
		static GLint s_canvasSizeLocation;
		static GLint s_colorLocation;
		static GLint s_gradientLocation;

		static bool _initProgram();
		void _setUniforms();

	protected:
		static ProgramObject* s_program;
		static GLuint s_posAttribLocation, s_lineAttribLocation;
		GLuint m_posBuffer, m_lineBuffer;//, m_elementArrayBuffer;
        std::vector<Vec2f>::size_type m_posBufferLen, m_lineBufferLen;
		std::vector<Vec2f> m_points, m_vecPos;
		std::vector<Vec3f> m_vecLineData;
		Vec4f m_color;
		float m_lineWidth, m_gradient;
	};

	//暂时用不上
// 	class GeometryLineStripWithTexture2d : public GeometryLineStrip2d
// 	{
// 	public:
// 		GeometryLineStripWithTexture2d() {}
// 		~GeometryLineStripWithTexture2d() {}
// 
// 
// 	protected:
// 		SharedTexture m_texture;
// 	};

	//特殊用法，参考 Sprite2dExt
	class SharedPoint
	{
	public:
		SharedPoint() : m_pnt(nullptr), m_refCount(nullptr) {}
        SharedPoint(const SharedPoint& other) : m_pnt(nullptr), m_refCount(nullptr)
		{
			*this = other;
		}

		SharedPoint(const Vec2f& v)
		{
			m_refCount = new int(1);
			m_pnt = new Vec2f(v);
		}

		SharedPoint(float x, float y)
		{
			m_refCount = new int(1);
			m_pnt = new Vec2f(x, y);
		}

		~SharedPoint();

		inline SharedPoint& operator =(const SharedPoint& other)
		{
			assert(this != &other);
			if(m_refCount != nullptr && --*m_refCount <= 0)
			{
				clear();
			}

			m_pnt = other.m_pnt;
			m_refCount = other.m_refCount;
			++*m_refCount;
			return *this;
		}

		inline Vec2f& pos() { assert(m_pnt != nullptr); return *m_pnt; }
		inline void setPos(const Vec2f& v) { *m_pnt = v; }
		inline void setPos(float x, float y) { m_pnt->x() = x; m_pnt->y() = y; }

	protected:
		void clear();

	private:
		mutable Vec2f* m_pnt;
		mutable int* m_refCount;
	};

}

#endif
