//
//  YCWatchFaceManager.h
//  YCProductSDK
//
//  Created by macos on 2021/12/1.
//

#import <Foundation/Foundation.h>
#import "modify.h"

NS_ASSUME_NONNULL_BEGIN

@interface YCWatchFaceManager : NSObject

/// 获取原始图片大小
+ (bmp_info_t)getOriginalPictureSize:(NSData *)fileData;

/// 获取缩略图的大小
+ (bmp_info_t)getThumbnailSize:(NSData *)fileData;

/// 修改表盘信息
/// @param fileData bin文件的二进制
/// @param imageData RGB888格式的图片二进制数据
/// @param x 时间的X坐标
/// @param y 时间的Y坐标
/// @param red 颜色
/// @param green 颜色
/// @param blue 颜色
/// @param isFlipColor 是否要翻转颜色
+ (NSData *)modifyWatchFace:(NSData *)fileData
                   fileSize:(NSInteger)fileSize
                  imageData:(NSData *)imageData
                  imageSize:(int)imageSize
              thumbnailData:(NSData *)thumbnailData
              thumbnailSize:(int)thumbnailSize
                      timeX:(int)x
                      timeY:(int)y
                        red:(Byte)red
                      green:(Byte)green
                       blue:(Byte)blue
                isFlipColor:(BOOL)isFlipColor;

@end

NS_ASSUME_NONNULL_END
