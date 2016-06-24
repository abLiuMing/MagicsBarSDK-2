//
//  BaseProtocol.m
//  emotionStore
//
//  Created by 刘铭 on 16/2/25.
//  Copyright © 2016年 刘铭. All rights reserved.
//

#import "MagicsBaseProtocol.h"
#import "AFNetworking.h"
#import "FileUtil.h"
#import "SSZipArchive.h"

@interface MagicsBaseProtocol ()


@property (nonatomic,strong) NSDictionary* resultData;

@property (nonatomic,  copy) NSString* filePath;

@property (nonatomic,strong) NSDictionary* userToken;
@end

const static NSString* rootUrl = @"http://mapi.appmagics.cn/";

@implementation MagicsBaseProtocol

static MagicsBaseProtocol* protocolInstance;

+ (instancetype)protocolInstance
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        protocolInstance = [[self alloc]init];
    });
    return protocolInstance;
}

/**
 *  @brief 授权登录
 */
- (void)appAuthorization:(CallBack)callBack {
    NSString* url = @"http://console.appmagics.cn/clients/appMagics/apps/FaceMagic/auth/sdk/FaceMagic";
    NSDictionary *paramters = @{@"userToken" : @"dc8b5770-daea-11e5-a69e-3ba4f5297e4a"};
    [self getRequestWithBaseUrl:url andType:NO header:paramters success:^(id responseObject) {
        NSString *time = [self getTime];
        NSDictionary* data = responseObject[@"entity"];
        
        NSDictionary* dic = @{
                              @"time"      : time,
                              @"userToken" : data[@"token"],
                              @"sdk_id"    : [self nullToString:data[@"sdk_id"] defaultValue:@""],
                              @"app_id"    : [self nullToString:data[@"app_id"] defaultValue:@""],
                              @"id"        : [self nullToString:data[@"id"] defaultValue:@""]
                              };
        
        [[NSUserDefaults standardUserDefaults]setObject:dic forKey:@"MaicsBarUserToken"];
        self.userToken = dic;
        callBack (responseObject);
    } failure:^(NSError *error, NSData *responseData) {
        callBack (nil);
        NSLog(@"🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻获取token失败🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻🙋🏻%@",error);
    }];
}

//魔贴商店
- (void)byGetShopAllDataCallBack:(CallBack)callBack
{
    if (![self contrastTime]) {
        [self appAuthorization:^(id obj) {
            if (obj) {
                [self byGetShopAllDataCallBack:^(id obj) {
                }];
            }
        }];
    }else{
        NSString* hostUrl = [rootUrl stringByAppendingFormat:@"%@", @"FaceM/video-sticker/full_tree"];//[rootUrl stringByAppendingFormat:@"add-ons/sticker/tree"];
        NSDictionary *paramters = @{};
        
        [self getRequestWithBaseUrl:hostUrl andType:YES header:paramters success:^(id responseObject) {
            self.resultData = responseObject;
            callBack(self.resultData);
        } failure:^(NSError *error, NSData *responseData) {
            self.resultData = nil;
            callBack(nil);
        }];
    }
    
}

- (void)byGetPasterShopCallBack:(CallBack)callBack
{
    if (![self contrastTime]) {
        [self appAuthorization:^(id obj) {
            if (obj) {
                callBack (@"获取成功");
            }
        }];
    }else{
        //?mark=100 取face++魔贴数据
        NSString* hostUrl =[rootUrl stringByAppendingFormat:@"%@",@"FaceM/video-sticker/entry_tree" ];//[rootUrl stringByAppendingFormat:@"add-ons/sticker/tree"];
        NSDictionary *paramters = @{};
        
        [self getRequestWithBaseUrl:hostUrl andType:YES header:paramters success:^(id responseObject) {
            self.resultData = responseObject;
            callBack(self.resultData);
        } failure:^(NSError *error, NSData *responseData) {
            self.resultData = nil;
            callBack(self.resultData);
        }];
    }
    
}

//获取魔贴商店数据
- (void)getStickerStoreDataWithStickerID:(NSString*)stickerID andPage:(NSString*)page andPageSize:(NSString*)pageSize andDataCallBack:(CallBack)callBack
{
    if (![self contrastTime]) {
        [self appAuthorization:^(id obj) {
            callBack (nil);
        }];
    }else{
        NSString* hostUrl = [rootUrl stringByAppendingFormat:@"FaceM/video-sticker?package=%@&page=%@&pageSize=%@",stickerID,page,pageSize];//&mark=100 取face++魔贴数据
        NSDictionary *paramters = @{};
        
        [self getRequestWithBaseUrl:hostUrl andType:YES header:paramters success:^(id responseObject) {
            self.resultData = responseObject;
            callBack(self.resultData);
        } failure:^(NSError *error, NSData *responseData) {
            self.resultData = nil;
            callBack(self.resultData);
        }];
    }
    
}

//魔贴下载统计
- (void)byGetStatisticSticker:(NSString*)stickerID
{
    if (![self contrastTime]) {
        [self appAuthorization:^(id obj) {
            
        }];
    }else{
        NSString* hostUrl = [rootUrl stringByAppendingFormat:@"FaceM/video-sticker/download-counter/%@",stickerID];
        //    [@"http://test.appmagics.cn:8080/FaceM/video-sticker/download-counter/" stringByAppendingFormat:@"%@",stickerID];
        NSDictionary *paramters = @{};
        
        [self getRequestWithBaseUrl:hostUrl andType:YES header:paramters success:^(id responseObject) {
            
        } failure:^(NSError *error, NSData *responseData) {
            
        }];
    }
    
}

- (void)getRequestWithBaseUrl:(NSString *)baseUrl andType:(BOOL)type header:(NSDictionary *)paramters success:(void(^)(id responseObject))success failure:(void(^)(NSError *error, NSData *responseData))failure
{
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    manager.requestSerializer = [AFJSONRequestSerializer serializer];
    manager.requestSerializer.timeoutInterval = 30;//
    //    NSLog(@"token=========%@",self.accessToken[@"token"]);
    if (type) {
       [manager.requestSerializer  setValue:self.userToken[@"userToken"] forHTTPHeaderField:@"auth_token"];
    }else{
        [manager.requestSerializer  setValue:@"dc8b5770-daea-11e5-a69e-3ba4f5297e4a" forHTTPHeaderField:@"ctoken"];
    }
    
    
    //    NSLog(@"加密===%@",[DES3Util encrypt:self.accessToken[@"token"]]);9c1bbc59047041f89b5795a4fbbdd078  access_token
    //__block dispatch_semaphore_t sem = dispatch_semaphore_create(0);
    
    [manager GET:baseUrl parameters:paramters success:^(AFHTTPRequestOperation *operation, id responseObject) {
        success(responseObject);
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        failure(error, operation.responseData);
        
    }];
}

//判断魔贴文件是否存在
-(BOOL)judgeFileExist:(NSString * )fileName
{
    NSString *filePath = [NSString stringWithFormat:@"%@/Documents%@/%@", NSHomeDirectory(), @"/Stickers",fileName];
    NSFileManager* fm = [NSFileManager defaultManager];
    if ([fm fileExistsAtPath:filePath]) {
        return YES;
    }else{
        return NO;
    }
}

//魔贴详细数据
- (void)downLoadWithFileUrl:(NSString*)fileUrl andPathName:(NSString*)name andProgressView:(UIProgressView*)progressView andData:(CallBack)callBack
{
    
    if ([self judgeFileExist:name]) {
        NSString *filePath = [NSString stringWithFormat:@"%@/Documents%@/%@", NSHomeDirectory(), @"/Stickers",name];
        callBack(filePath);
    }else{
        if (!fileUrl) {
            return;
        }
         NSString * path = [FileUtil getFullFilePathInDocuments:@"/myStickers" fileName:name];
         NSString * path1 = [FileUtil getFullFilePathInDocuments:@"/Stickers" fileName:name];
        
        NSURL *url = [[NSURL alloc] initWithString:fileUrl];
        NSURLRequest *request = [NSURLRequest requestWithURL:url];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        operation.inputStream   = [NSInputStream inputStreamWithURL:url];
        operation.outputStream  = [NSOutputStream outputStreamToFileAtPath:path append:NO];
        
        //        下载进度控制
        [operation setDownloadProgressBlock:^(NSUInteger bytesRead, long long totalBytesRead, long long totalBytesExpectedToRead) {
            CGFloat precent = (CGFloat)totalBytesRead / totalBytesExpectedToRead;
//            NSLog(@"is download：%f", precent);
            progressView.progress = precent;
        }];

        // 设置下载完成操作
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {

            BOOL sucess = [SSZipArchive unzipFileAtPath:path toDestination:path1];
            if (sucess) {
                NSString* dataPath = [self getDataFile:path1];
                if (dataPath) {
                    dataPath = [path1 stringByAppendingFormat:@"/%@",dataPath];
                    callBack (dataPath);
                }else{
                    NSLog(@"解压失败的========");
                    [FileUtil removeFile:path];
                    [FileUtil removeFile:path1];
                    callBack (nil);
                }
//                [[NSFileManager defaultManager] removeItemAtPath:path error:nil];
            }else {
                [FileUtil removeFile:path];
                [FileUtil removeFile:path1];
                callBack (nil);
            }
            
            // 下一步可以进行进一步处理，或者发送通知给用户。
            } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                [FileUtil removeFile:path];
                [FileUtil removeFile:path1];
                callBack (nil);
               NSLog(@"下载失败");
            }];
         [operation start];
    }
}

- (BOOL)contrastTime {
    self.userToken = [[NSUserDefaults standardUserDefaults]objectForKey:@"MaicsBarUserToken"];
    if (!self.userToken[@"userToken"]) {
        return NO;
    }
    float time1 = [self.userToken[@"time"] floatValue];
    float time2 = [[self getTime]floatValue];
    if ((time2 - time1) < 15 * 60 * 60 * 1000) {
        return YES;
    }else{
        return NO;
    }
}

- (NSString *)getTime {
    NSDate* dat = [NSDate dateWithTimeIntervalSinceNow:0];
    NSTimeInterval a=[dat timeIntervalSince1970]*1000; // *1000 是精确到毫秒
    NSString *timeString = [NSString stringWithFormat:@"%f", a]; //转为字符型
    
    return timeString;
}

- (NSString*)getDataFile:(NSString*)path
{
    NSString* pathStr;
    NSError* error;
    NSFileManager * fm = [NSFileManager defaultManager];
    NSArray  *arr = [fm  contentsOfDirectoryAtPath:path error:&error];
    if (error) {
        pathStr = nil;
    }else{
        for (NSString* name in arr) {
            pathStr = name;
        }
    }
    return pathStr;
}

- (NSString *)nullToString:(id)value defaultValue:(NSString *)defaultValue
{
    if(value == [NSNull null]) {
        return defaultValue;
    }
    if(value == nil) {
        return defaultValue;
    }
    return [NSString stringWithFormat:@"%@", value];
}

@end
