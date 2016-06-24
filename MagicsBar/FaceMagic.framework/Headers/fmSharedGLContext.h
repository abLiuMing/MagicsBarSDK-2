/*
 * fmSharedGLContext.h
 *
 *  Created on: 2015-7-11
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef fm_fmInterfaces_h
#define fm_fmInterfaces_h

#import <Foundation/Foundation.h>
#import <OpenGLES/EAGL.h>
#import "fmProcessingContext.h"

@interface FMSharedGLContext : FMProcessingContext

@property(nonatomic,strong) EAGLContext *context;

- (void)makeCurrent;

/////////////////////////////////////////////

+ (instancetype)globalGLContext;

+ (void)useGlobalGLContext;
+ (void)clearGlobalGLContext;

+ (void)globalSyncProcessingQueue:(void (^)(void))block;
+ (void)globalAsyncProcessingQueue:(void (^)(void))block;

+(instancetype)createSharedContext:(FMSharedGLContext*)sharedContext;
+(instancetype)createGlobalSharedContext;


@end

#endif
