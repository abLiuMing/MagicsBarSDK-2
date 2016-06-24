/*
* fmSprite2d.h
*
*  Created on: 2014-9-9
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#if !defined(_FMSPRITE2D_H_) && !defined(_FM_ONLY_FILTERS_)
#define _FMSPRITE2D_H_

#include "fmSpriteCommon.h"

namespace FM
{

	class GeometryLineStrip2d;
	class SharedPoint;

	class SpriteInterface2d : public SpriteCommonSettings
	{
	public:
		SpriteInterface2d();
		//模板兼容接口, 不应该被调用
// 		SpriteInterface2d(GLuint textureID, int width, int height) { assert(0); }
// 		//模板兼容接口, 不应该被调用
// 		SpriteInterface2d(const SharedTexture& texture) { assert(0); }
//		virtual ~SpriteInterface2d();

		inline const FM::Vec2f& getPosition() const { return m_pos; }
		inline const FM::Vec2f& getScaling() const { return m_scaling; }
		inline const FM::Vec2f& getHotspot() const { return m_hotspot; }
		inline float getRotation() const { return m_rotation; }
		inline float getAlpha() const { return m_alpha; }
		inline float getZ() const { return m_zIndex; }

		//(0, 0) 表示中心, (-1, -1)表示左上角, (1, 1) 表示右下角
		virtual void setHotspot(float sx, float sy)
		{
			m_hotspot[0] = sx;
			m_hotspot[1] = sy;
		}

		virtual void setHotspot2Center()
		{
			m_hotspot[0] = 0;
			m_hotspot[1] = 0;
		}

		virtual void move(float dx, float dy)
		{
			m_pos[0] += dx;
			m_pos[1] += dy;
		}

		virtual void moveTo(float x, float y)
		{
			m_pos[0] = x;
			m_pos[1] = y;
		}

		virtual void scale(float dsx, float dsy)
		{
			m_scaling[0] *= dsx;
			m_scaling[1] *= dsy;
		}

		virtual void scaleTo(float sx, float sy)
		{
			m_scaling[0] = sx;
			m_scaling[1] = sy;
		}

		virtual void rotate(float dRot)
		{
			m_rotation += dRot;
		}

		virtual void rotateTo(float rot)
		{
			m_rotation = rot;
		}

		virtual void setAlpha(float alpha)
		{
			m_alpha = alpha;
		}

		virtual void setZ(float z)
		{
			m_zIndex = z;
		}

		virtual void render() {	}

		inline bool operator<(const SpriteInterface2d& other) const
		{
			return m_zIndex < other.m_zIndex;
		}

		static bool compZ(const SpriteInterface2d& left, const SpriteInterface2d& right)
		{
			return left.m_zIndex < right.m_zIndex;
		}

		static bool compZp(const SpriteInterface2d* left, const SpriteInterface2d* right)
		{
			return left->m_zIndex < right->m_zIndex;
		}

	protected:
		static FMConstString paramAttribPositionName;
		static FMConstString paramProjectionMatrixName;
		static FMConstString paramHalfTexSizeName;
		static FMConstString paramRotationName;
		static FMConstString paramScalingName;
		static FMConstString paramTranslationName;
		static FMConstString paramHotspotName;
		static FMConstString paramAlphaName;
		static FMConstString paramZIndexName;
		static FMConstString paramTextureName;
		static FMConstString paramFilpCanvasName;
		static FMConstString paramFlipSpriteName;
		static FMConstString paramBlendColorName;

	protected:
		FM::Vec2f m_pos; //Sprite2d 的位置
		FM::Vec2f m_scaling; //Sprite2d 的缩放
		FM::Vec2f m_hotspot; //Sprite2d 的重心位置(乘以sprite本身尺寸的相对位置)
		float m_rotation; //Sprite2d 的旋转
		float m_alpha; //Sprite2d 的透明度
		float m_zIndex; //Sprite2d 的z值(仅作读取参考，不作为排序依据)
	};

	//使用虚继承， 以使得时间轴可扩展
	class Sprite2d : public virtual SpriteInterface2d
	{
	protected:
		Sprite2d(bool bInitProgram);
		Sprite2d(GLuint textureID, int width, int height, bool bInitProgram);
		Sprite2d(const SharedTexture& texture, bool bInitProgram);

		Sprite2d(); //兼容性接口， 不应该被调用

	public:
		static ShaderObject *spVertexShader, *spFragmentShader;
		static Sprite2d* sCreateWithBuffer(const void* bytes, int width, int height, FMBufferFormat format);
		static FMConstString getVertexString();
		static FMConstString getFragmentString();

		Sprite2d(GLuint textureID, int width, int height);
		Sprite2d(const SharedTexture& texture);
		virtual ~Sprite2d();

		SharedTexture& getTexture() { return m_texture; }
		void setTexture(const SharedTexture& tex);

		virtual void setCanvasSize(int width, int height)
		{
			Mat4 m = Mat4::makeOrtho(0.0f, (float)width, 0.0f, (float)height, -1e6f, 1e6f);
			m_program.bind();
			glUniformMatrix4fv(m_projectionLocation, 1, false, m[0]);
		}

		virtual void restoreCanvasSize()
		{
			m_program.bind();
			glUniformMatrix4fv(m_projectionLocation, 1, false, sOrthoProjectionMatrix[0]);
		}

		//笛卡尔坐标系与屏幕坐标系的Y轴方向相反，所以默认上下翻转。
		//如果要把sprite绘制到纹理中，可设置不翻转。
		virtual void setCanvasFlip(bool flipX, bool flipY)
		{
			float fx = flipX ? -1.0f : 1.0f;
			float fy = flipY ? -1.0f : 1.0f;
			m_program.bind();
			glUniform2f(m_canvasFlipLocation, fx, fy);
		}

		void restoreCanvasFlip()
		{
			float fx = sCanvasFlipX ? -1.0f : 1.0f;
			float fy = sCanvasFlipY ? -1.0f : 1.0f;
			m_program.bind();
			glUniform2f(m_canvasFlipLocation, fx, fy);
		}

		//默认上下翻转，在直接绘制到canvas上时以正常方向显示图像。
		virtual void setSpriteFlip(bool flipX, bool flipY)
		{
			float fx = flipX ? -1.0f : 1.0f;
			float fy = flipY ? -1.0f : 1.0f;
			m_program.bind();
			glUniform2f(m_spriteFilpLocation, fx, fy);
		}

		void restoreSpriteFlip()
		{
			float fx = sSpriteFlipX ? -1.0f : 1.0f;
			float fy = sSpriteFlipY ? -1.0f : 1.0f;
			m_program.bind();
			glUniform2f(m_spriteFilpLocation, fx, fy);
		}

		virtual void setZ(float z)  //z值范围: -1e20 ~ 1e20 (精度取决于float)
		{
			m_zIndex = z;
			m_program.bind();
			glUniform1f(m_zIndexLocation, z);
		}

		virtual void setAlpha(float alpha)
		{
			m_alpha = alpha;
			m_program.bind();
			glUniform1f(m_alphaLocation, alpha);
		}

		virtual void setHotspot(float sx, float sy)
		{
			m_hotspot[0] = sx;
			m_hotspot[1] = sy;
			m_program.bind();
			glUniform2f(m_hotspotLocation, sx, sy);
		}

		virtual void setHotspot2Center()
		{
			m_hotspot[0] = 0.0f;
			m_hotspot[1] = 0.0f;
			m_program.bind();
			glUniform2f(m_hotspotLocation, 0.0f, 0.0f);
		}

		virtual void move(float dx, float dy)
		{
			m_pos[0] += dx;
			m_pos[1] += dy;
		}

		virtual void moveTo(float x, float y)
		{
			m_pos[0] = x;
			m_pos[1] = y;
		}

		virtual void scale(float dsx, float dsy)
		{
			m_scaling[0] *= dsx;
			m_scaling[1] *= dsy;
		}

		virtual void scaleTo(float sx, float sy)
		{
			m_scaling[0] = sx;
			m_scaling[1] = sy;
		}

		virtual void rotate(float dRot)
		{
			m_rotation += dRot;			
		}

		virtual void rotateTo(float rot)
		{
			m_rotation = rot;
		}

		virtual void render();

	private:
		bool _initProgram();
		bool _tryLinkWithStaticShader();
		void _clearProgram();

	protected:
		//抽离绘制方式，使子类对绘制有更灵活控制。
		virtual void _drawFunc();
		void _initProgramVars();

		GLint m_posAttribLocation;
		GLint m_projectionLocation;
		GLint m_halfTexLocation;
		GLint m_rotationLocation;
		GLint m_scalingLocation;
		GLint m_translationLocation;
		GLint m_hotspotLocation;
		GLint m_alphaLocation;
		GLint m_zIndexLocation;
		GLint m_textureLocation;
		GLint m_canvasFlipLocation;
		GLint m_spriteFilpLocation;

		ProgramObject m_program;
		SharedTexture m_texture;
	};

	//在Sprite2d 的基础上提供更多的扩展功能

	class Sprite2dExt : public Sprite2d
	{
	protected:
		Sprite2dExt(); //兼容性接口

	public:
		static ShaderObject *spVertexShader, *spFragmentShader;
		static FMConstString getVertexString();
		static FMConstString getFragmentString();
		
        Sprite2dExt(GLuint textureID, int width, int height);
        Sprite2dExt(const SharedTexture& texture);
		~Sprite2dExt();

		//设置缩放比， 用以重复贴图。
		void setScaleRatio(float sx, float sy)
		{
			m_program.bind();
			glUniform2f(m_scaleRatioLocation, sx, sy);
		}

		void setBlendColor(float r, float g, float b)
		{
			m_program.bind();
			m_program.sendUniformf(paramBlendColorName, r, g, b);
		}

		void setLineWidth(float lineWidth);
		void setLineColor(float r, float g, float b, float a = 1.0f);

		bool enableClip(bool flag);
		virtual bool clipZone();
		virtual void restoreZone();

		void setBlendMode(FMGlobalBlendMode blendmode)
		{
			m_blendMode = blendmode;
		}

		//zone point 取值范围: [-1.0, 1.0]
		//(-1.0, -1.0) 代表左上角, 至少3个zone-point才能构成一个zone
		void pushZonePoint(const SharedPoint& pnt) { m_zone.push_back(pnt); }
		void pushZonePoint(float x, float y) { SharedPoint pnt(x, y); m_zone.push_back(pnt); }

		std::vector<SharedPoint>& getZone() { return m_zone; }
		void setZone(const std::vector<SharedPoint>& zone) { m_zone = zone; }
		void clearZone() { m_zone.clear(); }

//		virtual void render();

		void setGradient(float gradient);

		static void sSetZoneFlip(bool flipX, bool flipY)
		{
			sZoneFlip[0] = flipX ? -1.0f : 1.0f;
			sZoneFlip[1] = flipY ? -1.0f : 1.0f;
			if(sClipProgram != nullptr)
			{
				sClipProgram->bind();
				sClipProgram->sendUniformf("blankflip", sZoneFlip[0], sZoneFlip[1]);
			}
		}

		static Vec2f sZoneFlip;
		static void sReleaseClipProgram();
	private:
		bool _initProgram();
		bool _tryLinkWithStaticShader();
		void _clearProgram();

	protected:
		virtual void _drawFunc();

		GLint m_scaleRatioLocation;

		//使用一系列连续点确定一个控制区域，进行裁切。
		//默认会使用第一个点同时作为结束点，以使得整个区域封闭。
		//zone 至少需要三个点才能构成
		std::vector<SharedPoint> m_zone; 
		std::vector<float> m_dataCache;	
		static ProgramObject* sClipProgram;		
		GeometryLineStrip2d* m_lineSprite;
		FMGlobalBlendMode m_blendMode;
		float m_lineWidth;
	};

	class Sprite2dWithAlphaGradient : public Sprite2d
	{
	protected:
		Sprite2dWithAlphaGradient(); //兼容性接口
	public:
		static ShaderObject *spVertexShader, *spFragmentShader;
		static FMConstString getVertexString();
		static FMConstString getFragmentString();
		
		Sprite2dWithAlphaGradient(GLuint textureID, int width, int height);
		Sprite2dWithAlphaGradient(const SharedTexture& texture);
		~Sprite2dWithAlphaGradient();

		void setAlphaTexture(const SharedTexture& tex) { m_texAlpha = tex; }

		void setAlphaFactor(float start, float end);

	private:
		bool _initProgram();
		bool _tryLinkWithStaticShader();
		void _clearProgram();

	protected:
		virtual void _drawFunc();

		static FMConstString paramAlphaFactorName;
		static FMConstString paramTexAlphaName;
		
		GLint m_texAlphaLocation, m_alphaFactorLocation;

		SharedTexture m_texAlpha;
	};

	//////////////////////////////////////////////////////////////////////////

	class Sprite2dInterChange : public Sprite2d
	{
	protected:
		Sprite2dInterChange(); //兼容性接口

	public:
		static ShaderObject *spVertexShader, *spFragmentShader;
		static FMConstString getVertexString();
		static FMConstString getFragmentString();

		Sprite2dInterChange(GLuint textureID, int width, int height);
		Sprite2dInterChange(const SharedTexture& texture);
		~Sprite2dInterChange();

		//viewArea 取值范围 [0, 1]
		inline void setViewArea(const Vec4f& viewArea)
		{
			m_program.bind();
			glUniform4f(m_viewAreaLocation, viewArea[0], viewArea[1], viewArea[2], viewArea[3]);
		}

	private:
		bool _initProgram();
		bool _tryLinkWithStaticShader();
		void _clearProgram();

	protected:
//		virtual void _drawFunc();

		static FMConstString paramViewAreaName;
		GLuint m_viewAreaLocation;
	};

	//提供类似于gif图片显示的效果.  需要提供一张合并了多张小图的“大”纹理
	//然后指定每一帧所在的纹理区域， 之后根据所设置的区域， 以及更新时间间隔进行更新。
	class Sprite2dInterChangeExt : public Sprite2dInterChange
	{
	protected:
		Sprite2dInterChangeExt() { assert(0); } //兼容性接口, 不应该被调用
	public:
		Sprite2dInterChangeExt(GLuint textureID, int width, int height) : Sprite2dInterChange(textureID, width, height), m_frameIndex(0), m_deltaTime(100.0f), m_deltaAccum(0.0f), m_lastTime(0.0f), m_blendMode(FMGLOBAL_BLEND_NONE), m_shouldLoop(true) {}
		Sprite2dInterChangeExt(const SharedTexture& texture) : Sprite2dInterChange(texture), m_frameIndex(0), m_deltaTime(100.0f), m_deltaAccum(0.0f), m_lastTime(0.0f), m_blendMode(FMGLOBAL_BLEND_NONE), m_shouldLoop(true) {}
		~Sprite2dInterChangeExt() {}

		void firstFrame();
		void nextFrame(unsigned int offset = 1);

		void updateFrame(double dt); //根据两帧之间的间隔时间更新

		void setFrameTime(double t) { m_lastTime = t; } //设置开始的总时间， 一般为当前时间
		void updateByTime(double t); //根据总时间更新

		void setFPS(float fps) { m_deltaTime = 1000.0f / fps; } //设置sprite切换的帧率， 默认10 fps

		//viewArea 单个分量取值范围 [0, 1]
		void pushViewArea(const Vec4f& area) { m_vecFrames.push_back(area); }
		void flushViewArea();
		void enableLoop(bool loop) { m_shouldLoop = loop; }

		void setBlendMode(FMGlobalBlendMode blendMode) { m_blendMode = blendMode; }

        inline bool isLastFrame() { return m_frameIndex >= m_vecFrames.size() - 1; }
        inline void setToLastFrame() { m_frameIndex = (unsigned int)m_vecFrames.size() - 1; }
        
	protected:
		void _drawFunc();

		std::vector<Vec4f> m_vecFrames;
		unsigned int m_frameIndex;
		double m_deltaTime, m_deltaAccum, m_lastTime;
		FMGlobalBlendMode m_blendMode;
		bool m_shouldLoop;
	};

    //////////////////////////////////////////////////////////////////////////
    
    class Sprite2dSequence : public Sprite2d
    {
    protected:
        Sprite2dSequence() {assert(0); }
    public:
        Sprite2dSequence(int width, int height) : Sprite2d(-1, width, height), m_frameIndex(0), m_deltaTime(100.0), m_deltaAccum(0.0), m_lastTime(0.0), m_shouldLoop(true), m_canUpdate(true) {}

        ~Sprite2dSequence();
        
        void firstFrame();
        void nextFrame(unsigned int offset = 1);
        
        virtual void updateFrame(double dt); //根据两帧之间的间隔时间更新
        
        void setFrameTime(double t) { m_lastTime = t; } //设置开始的总时间， 一般为当前时间
        void updateByTime(double t); //根据总时间更新
        size_t getFrameCount () {return m_frameTextures.size();}
        
        //useSec 为 true时表示使用秒为单位， 为 false 时使用毫秒， 默认 false
        void setFPS(double fps, bool useSec = false); //设置sprite切换的帧率， 默认10 fps
        double getCurrentTime(){return (double)m_frameIndex * m_deltaTime ;}
        inline void setFrameDuring(double during) { m_deltaTime = during; }
        
        inline void enableLoop(bool loop) { m_shouldLoop = loop; }
        
        inline bool isLastFrame() { return m_frameIndex >= m_frameTextures.size() - 1; }
        inline void setToLastFrame() { m_frameIndex = (unsigned int)m_frameTextures.size() - 1; }
        
        inline unsigned int getFrameIndex() { return m_frameIndex; }
        
        inline void addFrameTexture(GLuint texID) { m_frameTextures.push_back(texID); }
        inline void setFrameTextures(const std::vector<GLuint> v) { m_frameTextures = v; }
        inline std::vector<GLuint>& getFrameTextures() { return m_frameTextures; }
        
        inline void setUpdate(bool update) { m_canUpdate = update; }
        inline bool canUpdate() { return m_canUpdate; }
        
    protected:
        void _drawFunc();
        
        unsigned int m_frameIndex;
        std::vector<GLuint> m_frameTextures;
        double m_deltaTime, m_deltaAccum, m_lastTime;
        bool m_shouldLoop, m_canUpdate;
    };
    
	//////////////////////////////////////////////////////////////////////////

	//为slideshow 提供逻辑方法
	class Sprite2dWith3dSpaceHelper : virtual public SpriteInterface2d
	{
	public:
		Sprite2dWith3dSpaceHelper() : m_rotMatrix(Mat3::makeIdentity()) {}

		void rotate(float rot, float x, float y, float z)
		{
			m_rotMatrix.rotate(rot, x, y, z);
		}

		void rotate(float rot, const Vec3f& axis)
		{
			m_rotMatrix.rotate(rot, axis[0], axis[1], axis[2]);
		}

		void rotateX(float rot)
		{
			m_rotMatrix.rotateX(rot);
		}

		void rotateY(float rot)
		{
			m_rotMatrix.rotateY(rot);
		}

		void rotateZ(float rot)
		{
			m_rotMatrix.rotateZ(rot);
		}

		void rotateTo(float rot, float x, float y, float z)
		{
			m_rotMatrix = Mat3::makeRotation(rot, x, y, z);
		}

		void rotateTo(float rot, const Vec3f& axis)
		{
			m_rotMatrix = Mat3::makeRotation(rot, axis[0], axis[1], axis[2]);
		}

		void rotateXTo(float rot)
		{
			m_rotMatrix = Mat3::makeXRotation(rot);
		}

		void rotateYTo(float rot)
		{
			m_rotMatrix = Mat3::makeYRotation(rot);
		}

		void rotateZTo(float rot)
		{
			m_rotMatrix = Mat3::makeZRotation(rot);
		}

		inline Mat3 getRotationMatrix() { return m_rotMatrix; }

	protected:
		FM::Mat3 m_rotMatrix;
	};

	//////////////////////////////////////////////////////////////////////////

	class Sprite2dWith3dSpace : public Sprite2d
	{
	protected:
		Sprite2dWith3dSpace(); //兼容性接口
	public:
		static ShaderObject *spVertexShader, *spFragmentShader;
		static FMConstString getVertexString();
		static FMConstString getFragmentString();

		Sprite2dWith3dSpace(GLuint textureID, int width, int height);
		Sprite2dWith3dSpace(const SharedTexture& texture);
		~Sprite2dWith3dSpace();

		void render();

		void rotate(float rot, float x, float y, float z)
		{
			m_rotMatrix.rotate(rot, x, y, z);
		}

		void rotate(float rot, const Vec3f& axis)
		{
			m_rotMatrix.rotate(rot, axis[0], axis[1], axis[2]);
		}

		void rotateX(float rot)
		{
			m_rotMatrix.rotateX(rot);
		}

		void rotateY(float rot)
		{
			m_rotMatrix.rotateY(rot);
		}

		void rotateZ(float rot)
		{
			m_rotMatrix.rotateZ(rot);
		}

		void rotateTo(float rot, float x, float y, float z)
		{
			m_rotMatrix = Mat3::makeRotation(rot, x, y, z);
		}

		void rotateTo(float rot, const Vec3f& axis)
		{
			m_rotMatrix = Mat3::makeRotation(rot, axis[0], axis[1], axis[2]);
		}

		void rotateXTo(float rot)
		{
			m_rotMatrix = Mat3::makeXRotation(rot);
		}

		void rotateYTo(float rot)
		{
			m_rotMatrix = Mat3::makeYRotation(rot);
		}

		void rotateZTo(float rot)
		{
			m_rotMatrix = Mat3::makeZRotation(rot);
		}

		//virtual void setCanvasSize(int width, int height)
		//{
		//	Mat4 m = Mat4::makeOrtho(0.0f, (float)width, 0.0f, (float)height, -1e20f, 1e20f);
		//	m_program.bind();
		//	glUniformMatrix4fv(m_projectionLocation, 1, false, m[0]);
		//}

		//virtual void restoreCanvasSize()
		//{
		//	m_program.bind();
		//	glUniformMatrix4fv(m_projectionLocation, 1, false, sOrthoProjectionMatrix[0]);
		//}

		void setHotspot(float sx, float sy)
		{
			m_hotspot[0] = sx;
			m_hotspot[1] = sy;
			m_program.bind();
			glUniform3f(m_hotspotLocation, sx, sy, m_hotspotZ);
		}

		void setHotspot3d(float sx, float sy, float sz)
		{
			m_hotspot[0] = sx;
			m_hotspot[1] = sy;
			m_hotspotZ = sz;
			m_program.bind();
			glUniform3f(m_hotspotLocation, sx, sy, sz);
		}

		void setHotspot2Center()
		{
			m_hotspot[0] = 0.0f;
			m_hotspot[1] = 0.0f;
			m_hotspotZ = 0.0f;
			m_program.bind();
			glUniform3f(m_hotspotLocation, 0.0f, 0.0f, 0.0f);
		}

		void setMVPMatrix(const Mat4& matrix)
		{
			m_program.bind();
			glUniformMatrix4fv(m_projectionLocation, 1, false, matrix[0]);
		}

		inline Mat3 getRotationMatrix() { return m_rotMatrix; }

	private:
		bool _initProgram();
		bool _tryLinkWithStaticShader();
		void _clearProgram();

		void rotate(float dRot)	{ assert(0); } //此接口不可用
		void rotateTo(float rot) { assert(0); } //同上

	protected:
		FM::Mat3 m_rotMatrix;
		float m_hotspotZ;
	};

}


#endif
