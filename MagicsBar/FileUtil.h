//
//  FileUtil.h
//  HarryPhoto
//
//  Created by xuye on 13-1-12.
//  Copyright (c) 2013年 superteam. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface FileUtil : NSObject
+ (void)initContext;
+ (NSString *)getPicturePath:(NSString *)uid fileName:(NSString *)fileName;
+ (NSString *)getFullFilePathInDocuments:(NSString *)subFilePath fileName:(NSString *)fileName;
+ (NSArray *)getArrayFromFile:(NSString *)filePath;
+ (NSDictionary *)getDictFromFile:(NSString *)filePath;
+ (NSString *)getStringFromFile:(NSString *)filePath;

+ (NSMutableArray *)loadCachedPictures:(NSString *)cacheFileName inSubDir:(NSString *)subDir;
+ (BOOL)saveCachedPictures:(NSArray *)pictures toFile:(NSString *)cacheFileName inSubDir:(NSString *)subDir;
+ (NSMutableArray *)loadCachedPictures:(NSString *)filePath;
+ (BOOL)saveCachedPictures:(NSArray *)pictures toFilePath:(NSString *)filePath;
+ (BOOL)downloadFile:(NSString *)fileUrl filePath:(NSString *)filePath forceDownload:(BOOL)forceDownload;
+ (BOOL)downloadPicture:(NSString *)fileUrl filePath:(NSString *)filePath forceDownload:(BOOL)forceDownload;
//only the file in current directory, not recursive
+ (void)zipDir:(NSString *)unzipDir to:(NSString *)filePath;
//only the file in current directory, not recursive
+ (BOOL)unzipFile:(NSString *)filePath to:(NSString *)unzipDir;
+ (BOOL)downloadFile:(NSString *)fileUrl andUnzipTo:(NSString *)unzipDir forceDownload:(BOOL)forceDownload;

+ (void)removeFile:(NSString *)filePath;
+ (void)removeFilesInDocuments:(NSString *)subFilePath;
+ (void)removeTmpFiles;
+ (BOOL)isFileExists:(NSString *)filePath;
+ (void)moveFile:(NSString *)filePath to:(NSString *)targetFilePath;
+ (void)copyFile:(NSString *)filePath to:(NSString *)targetFilePath;
+ (void)copyDirectory:(NSString *)fromPath toPath:(NSString *)toPath;
@end
