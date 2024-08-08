//
//  YCECGAlgorithmManager.h
//  SmartHealthPro
//
//  Created by yc on 2020/11/25.
//  Copyright © 2020 yc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NorchFilter.h"
#import "ECGAnaly.h"
#import "hrv_index.h"


NS_ASSUME_NONNULL_BEGIN

@interface YCECGAlgorithmManager : NSObject
 
/// 获得HRV的值
@property (copy, nonatomic) void(^calculateHrvCallBack)(NSInteger hrvValue);

/// 获得RR间隔与心率
@property (copy, nonatomic) void(^rriCache)(float rri, int heartRate);

/// 算法初始化
/// @param isEncryption 是否加密
- (void)initECGAlgorithmLibrary:(BOOL)isEncryption;

/// 接收中间的ECG数据
- (CGFloat)receiveECGData:(CGFloat)data;


/// 获取最后的测量结果
- (void)getMeasurementResult:(short *)heartRate
                     qrsType:(int *)qrsType
                      afflag:(int *)afflag;



/// 获取身体状态 返回0表示正常，其它值错误 (过时)
/// @param rriDatas RR间期数据
/// @param heavyLoad 负荷指数
/// @param pressure 压力指数
/// @param hrvNorm HRV指数
/// @param body 身体指数
- (int)getBody:(NSArray *)rriDatas
      heavyLoad:(float *)heavyLoad
       pressure:(float *)pressure
        hrvNorm:(float *)hrvNorm
           body:(float *)body;

/// 获取ECG身体相关的参数
/// @param heavyLoad 负荷指数 0 ~ 100 五级
/// @param pressure 压力指数 0 ~ 100 四级
/// @param hrvNorm HRV指数 0 ~ 100 三级
/// @param body 身指指数 0 ~ 100  三级
/// @param sympatheticParasympathetic 交感神经和副交感神经活跃指数 三级
/// @param respiratoryRate 吸吸率
- (void)getBody:(float *)heavyLoad
       pressure:(float *)pressure
        hrvNorm:(float *)hrvNorm
           body:(float *)body
sympatheticParasympathetic:(float *)sympatheticParasympathetic
respiratoryRate:(uint8_t *)respiratoryRate;


/// 转换ECG数据
/// @param sourceData 原始ECG数据
/// @param isOriginalData 是否为原始标记
/// @param gridSize 每个格子的大小
/// @param limitCount 上下每一侧的格子数
+ (NSMutableArray *)converDrawData:(NSArray *)sourceData isOriginalData:(BOOL *)isOriginalData
              gridSize:(CGFloat) gridSize
            limitCount:(NSInteger)limitCount;

/// 将原始数据转换为康源需要的数据
/// @param sourceData 原始ECG数据
+ (NSMutableArray *)converKanYanData:(NSArray *)sourceData;

/// 转换绘图数据
/// @param sroceData <#sroceData description#>
/// @param gridSize 每个格子的大小
/// @param limitCount 上下两端限制值的倍数
+ (NSMutableArray *)converECGDrawLineData:(NSArray *)sroceData gridSize:(CGFloat)gridSize limitCount:(NSInteger)limitCount ecgModel:(int)ecgModel ;

@end

NS_ASSUME_NONNULL_END
