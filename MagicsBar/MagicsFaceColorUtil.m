//
//  MagicsColorUtil.m
//  Magics Demo
//
//  Created by  Aphasia丶 爆米花 on 15/3/5.
//  Copyright (c) 2015年 刘~~村长. All rights reserved.
//

#import "MagicsFaceColorUtil.h"

@implementation MagicsFaceColorUtil

//RGB颜色转换方法
- (UIColor *)getColor:(NSString *)hexColor
{
    
    unsigned int red,green,blue;
    if([hexColor hasPrefix:@"0X"])hexColor=[hexColor substringFromIndex:2];
    if([hexColor hasPrefix:@"#"])hexColor=[hexColor substringFromIndex:1];
    NSRange range;
    range.length = 2;
    
    range.location = 0;
    [[NSScanner scannerWithString:[hexColor substringWithRange:range]] scanHexInt:&red];
    
    range.location = 2;
    [[NSScanner scannerWithString:[hexColor substringWithRange:range]] scanHexInt:&green];
    
    range.location = 4;
    [[NSScanner scannerWithString:[hexColor substringWithRange:range]] scanHexInt:&blue];
    return [UIColor colorWithRed:(float)(red/255.0f) green:(float)(green / 255.0f) blue:(float)(blue / 255.0f) alpha:1.0f];
}
+(void)setColor:(NSString *)hexColor inContext:(CGContextRef)context
{
    unsigned int red,green,blue;
    NSRange range;
    range.length = 2;
    
    range.location = 0;
    if (hexColor.length<6) {
        for (int i=0; i<6-hexColor.length; i++) {
            hexColor=[@"0" stringByAppendingString:hexColor];
        }
        
    }
    if([hexColor hasPrefix:@"0X"])hexColor=[hexColor substringFromIndex:2];
    if([hexColor hasPrefix:@"#"])hexColor=[hexColor substringFromIndex:1];
    [[NSScanner scannerWithString:[hexColor substringWithRange:range]] scanHexInt:&red];
    
    range.location = 2;
    [[NSScanner scannerWithString:[hexColor substringWithRange:range]] scanHexInt:&green];
    
    range.location = 4;
    [[NSScanner scannerWithString:[hexColor substringWithRange:range]] scanHexInt:&blue];
    
    CGContextSetRGBStrokeColor(context,(float)(red/255.0f),(float)(green / 255.0f), (float)(blue / 255.0f),1);
    
}

+ (UIColor *)getColor:(NSString *)hexColor
{
    
    unsigned int red,green,blue;
    if([hexColor hasPrefix:@"0X"])hexColor=[hexColor substringFromIndex:2];
    if([hexColor hasPrefix:@"#"])hexColor=[hexColor substringFromIndex:1];
    NSRange range;
    range.length = 2;
    
    range.location = 0;
    [[NSScanner scannerWithString:[hexColor substringWithRange:range]] scanHexInt:&red];
    
    range.location = 2;
    [[NSScanner scannerWithString:[hexColor substringWithRange:range]] scanHexInt:&green];
    
    range.location = 4;
    [[NSScanner scannerWithString:[hexColor substringWithRange:range]] scanHexInt:&blue];
    return [UIColor colorWithRed:(float)red/255.0f green:(float)green / 255.0f blue:(float)blue / 255.0f alpha:1.0f];
}



@end
