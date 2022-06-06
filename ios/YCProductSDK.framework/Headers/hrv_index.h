//
// Created by liaoy on 2020/11/30.
//

#ifndef _HRV_INDEX_H
#define _HRV_INDEX_H

/**
 * RRI: 传入的RRI数组 （传入）
 * len: RRI数组的长度 （传入）
 * heavy_load: 负荷指数 （越大越不好 传出）
 * pressure: 压力指数 （越大越不好 传出）
 * HRV_norm: HRV指数 （越大越好 传出）
 * body: 身体指数 （越大越好 传出）
 * 返回： 0正常 -1错误
 * */
int hrv_norm(const float RRI[], int len, float *heavy_load, float *pressure, float *HRV_norm, float *body);


void hrv_norm_init(void);

int get_hrv_norm(float RRI, float *heavy_load, float *pressure, float *HRV_norm, float *body);

#endif //_HRV_INDEX_H
