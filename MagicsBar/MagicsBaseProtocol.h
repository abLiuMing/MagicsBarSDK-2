//
//  BaseProtocol.h
//  emotionStore
//
//  Created by 刘铭 on 16/2/25.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef void (^CallBack)(id obj);

@interface MagicsBaseProtocol : NSObject

+ (instancetype)protocolInstance;

// 授权登录

- (void)appAuthorization:(CallBack)callBack;

//魔贴商店
- (void)byGetShopAllDataCallBack:(CallBack)callBack;


//魔贴推荐数据
- (void)byGetPasterShopCallBack:(CallBack)callBack;

//魔贴详细数据
- (void)downLoadWithFileUrl:(NSString*)fileUrl andPathName:(NSString*)name andProgressView:(UIProgressView*)progressView andData:(CallBack)callBack;

//判断魔贴文件是否存在
- (BOOL)judgeFileExist:(NSString * )fileName;

//魔贴下载统计
- (void)byGetStatisticSticker:(NSString*)stickerID;

//获取魔贴商店数据
- (void)getStickerStoreDataWithStickerID:(NSString*)stickerID andPage:(NSString*)page andPageSize:(NSString*)pageSize andDataCallBack:(CallBack)callBack;

@end
