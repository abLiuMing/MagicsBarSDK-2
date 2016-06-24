/*
 * fmUtilFunctions.h
 *
 *  Created on: 2015-7-10
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */


// 仅提供与安卓版一致的接口， 如需更佳的使用体验， 可直接调用C++代码！

#import <UIKit/UIKit.h>
#import "fmSharedGLContext.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef UIImage* (*LoadImageCallback)(const char* name, void* arg);
    typedef void (*LoadImageOKCallback)(UIImage*, void* arg);
    
    void fmSetLoadImageCallback(LoadImageCallback, LoadImageOKCallback, void* arg);
   
    GLuint fmGlobalTextureLoadFunc(const char* source, GLint* w, GLint* h, void* arg);
    
    // 注意， 图片格式必须为 GL_RGBA + GL_UNSIGNED_BYTE
    typedef struct FMFilterImageInfo
    {
        void* data; // char array.
        int width; //图片宽 (真实宽度， 需要将对齐计算在内)
        int height; //图片高 (真实高度， 需要将对齐计算在内)
    }FMFilterImageInfo;
    
    //通过Buffer创建GL纹理, 如果可能， 请使用 GLKTextureLoader 直接完成
    GLuint fmUIImage2Texture(UIImage* uiimage);

    //width和height将被写入texture的真实宽高 (tips: 后期可能存在UIImage过大时， 纹理将被压缩， 所以只有从这里获得的参数才是准确值)
    GLuint fmUIImage2TextureWithSize(UIImage* uiimage, GLint* width, GLint* height);
    
    //较快创建Texture的方法， 当imageBuffer为NULL 时将使用malloc创建合适大小的buffer。
    GLuint fmCGImage2Texture(CGImageRef cgImage, void* imageBuffer);
    
    UIImage* fmCreateUIImageWithBufferRGBA(void* buffer, size_t width, size_t height, size_t bitsPerComponent, size_t bytesPerRow);
    
    //可以传入buffer作为缓存, 否则将创建一份新的buffer (malloc), 调用者需要负责 free
    char* fmGenBufferWithCGImage(CGImageRef cgImage, char* buffer, bool isGray);
    
    CGAffineTransform fmGetUIImageOrientationTransform(UIImage* image);
    UIImage* fmForceUIImageUp(UIImage* image);
    
    /*
     
     fmGetMachineDescriptionString 返回值对应表 
     (from: http://stackoverflow.com/questions/448162/determine-device-iphone-ipod-touch-with-iphone-sdk )
     
     @"iPhone1,1" => @"iPhone 1G"
     @"iPhone1,2" => @"iPhone 3G"
     @"iPhone2,1" => @"iPhone 3GS"
     @"iPhone3,1" => @"iPhone 4"
     @"iPhone3,3" => @"Verizon iPhone 4"
     @"iPhone4,1" => @"iPhone 4S"
     @"iPhone5,1" => @"iPhone 5 (GSM)"
     @"iPhone5,2" => @"iPhone 5 (GSM+CDMA)"
     @"iPhone5,3" => @"iPhone 5c (GSM)"
     @"iPhone5,4" => @"iPhone 5c (GSM+CDMA)"
     @"iPhone6,1" => @"iPhone 5s (GSM)"
     @"iPhone6,2" => @"iPhone 5s (GSM+CDMA)"
     @"iPhone7,1" => @"iPhone 6 Plus"
     @"iPhone7,2" => @"iPhone 6"
     @"iPod1,1" => @"iPod Touch 1G"
     @"iPod2,1" => @"iPod Touch 2G"
     @"iPod3,1" => @"iPod Touch 3G"
     @"iPod4,1" => @"iPod Touch 4G"
     @"iPod5,1" => @"iPod Touch 5G"
     @"iPad1,1" => @"iPad"
     @"iPad2,1" => @"iPad 2 (WiFi)"
     @"iPad2,2" => @"iPad 2 (GSM)"
     @"iPad2,3" => @"iPad 2 (CDMA)"
     @"iPad2,4" => @"iPad 2 (WiFi)"
     @"iPad2,5" => @"iPad Mini (WiFi)"
     @"iPad2,6" => @"iPad Mini (GSM)"
     @"iPad2,7" => @"iPad Mini (GSM+CDMA)"
     @"iPad3,1" => @"iPad 3 (WiFi)"
     @"iPad3,2" => @"iPad 3 (GSM+CDMA)"
     @"iPad3,3" => @"iPad 3 (GSM)"
     @"iPad3,4" => @"iPad 4 (WiFi)"
     @"iPad3,5" => @"iPad 4 (GSM)"
     @"iPad3,6" => @"iPad 4 (GSM+CDMA)"
     @"iPad4,1" => @"iPad Air (WiFi)"
     @"iPad4,2" => @"iPad Air (Cellular)"
     @"iPad4,4" => @"iPad mini 2G (WiFi)"
     @"iPad4,5" => @"iPad mini 2G (Cellular)"
     
     @"iPad4,7" => @"iPad mini 3 (WiFi)"
     @"iPad4,8" => @"iPad mini 3 (Cellular)"
     @"iPad4,9" => @"iPad mini 3 (China Model)"
     
     @"iPad5,3" => @"iPad Air 2 (WiFi)"
     @"iPad5,4" => @"iPad Air 2 (Cellular)"
     
     @"i386" => @"Simulator"
     @"x86_64" => @"Simulator"
     */
    
    NSString* fmGetMachineDescriptionString();
    
    typedef enum { FMDevice_Simulator, FMDevice_iPod, FMDevice_iPhone, FMDevice_iPad } FMDeviceEnum;
    
    typedef struct
    {
        FMDeviceEnum model;
        int majorVerion, minorVersion;
    } FMDeviceDescription;
    
    FMDeviceDescription fmGetDeviceDescription();
    
#ifdef __cplusplus
}
#endif