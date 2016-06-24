/*
 * fmFaceTracker.h
 *
 *  Created on: 2016-1-16
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _FM_FACETRACKER_H_
#define _FM_FACETRACKER_H_

#include "cv_face.h"
#include "fmVec.h"
#include "fmFaceCommon.h"
#include <vector>

namespace FM
{
    class FMFaceTracker
    {
    public:
        FMFaceTracker(int flag = CV_FACE_SKIP_BELOW_THRESHOLD | CV_FACE_RESIZE_IMG_320W);
        ~FMFaceTracker();
        
        void setFaceLimit(int cnt); //-1 表示不限制
        
        bool updateFace(const void* imgBuffer, int w, int h, int stride, cv_pixel_format format = CV_PIX_FMT_GRAY8, cv_face_orientation orientation = CV_FACE_UP);
        
        static const std::vector<int>& getFeatureIndices(FMFaceFeature feature);
        
        //脸部位置
        inline Vec4f getFaceRect() const
        {
            const cv_rect_t& rt = m_faceResult->rect;
            Vec4f v;
            v[0] = rt.left / m_faceImageSize[0];
            v[1] = rt.top / m_faceImageSize[1];
            v[2] = (rt.right - rt.left) / m_faceImageSize[0];
            v[3] = (rt.bottom - rt.top) / m_faceImageSize[1];
            
            return v;
        }
        
        inline int getFaceImageWidth() const { return m_faceImageSize[0]; }
        inline int getFaceImageHeight() const { return m_faceImageSize[1]; }
        
        bool isMouthOpen() const; //嘴巴是否张开
        Vec2f getMouthPos() const; //嘴巴位置
        
        Vec2f getLeftEyePos() const; //左眼位置
        Vec2f getRightEyePos() const; //右眼位置
        Vec2f getUpDir() const; //头部向上方向 (以左右双眼形成的向量作为参考)
        Vec2f getRightDir() const; //头部水平方向 (以左右双眼形成的向量作为参考)
        
        Vec2f getCenterPos() const; //面部中心位置
        Vec2f getNoseCenterPos() const; //鼻子中心位置
        Vec2f getEyeCenterPos() const; //双眼正中位置
        Vec2f getJawBottomPos() const; //下巴最下方位置
        
        inline void resetFrame() { cv_face_reset_tracker_106(m_stHandler); }
        
        inline bool hasFacePoints() const { return m_faceResult != nullptr; }
        
        inline cv_face_106_t* getFaceResult() { return m_faceResult; }
        inline cv_pointf_t& getFacePointByIndex(int index) { return m_faceResult->points_array[index]; }

        inline int getFaceCount() { return m_faceCount; }
        
    protected:
        
        static std::vector<int> vecFaceIndices[FM_FACE_TOTAL_FEATURE_NUM];
        
        Vec2i m_faceImageSize;
        cv_handle_t m_stHandler;
        cv_face_106_t* m_faceResult;
        int m_faceCount;
        
        float m_yaw, m_pitch, m_roll;
    };
}

#endif