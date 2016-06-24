//
//  MagicsColorUtil.h
//  Magics Demo
//
//  Created by  Aphasia丶 爆米花 on 15/3/5.
//  Copyright (c) 2015年 刘~~村长. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface MagicsFaceColorUtil : NSObject

- (UIColor *)getColor:(NSString *)hexColor;
+ (UIColor *)getColor:(NSString *)hexColor;
+(void)setColor:(NSString *)hexColor inContext:(CGContextRef)context;

@end
