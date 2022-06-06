//
// Created by liaoy on 2021/6/10.
//

#ifndef BIN_MODIFY_H
#define BIN_MODIFY_H

#include <stdint.h>
#include <stdbool.h>


extern int thumbnail_scale;

typedef struct
{
    int width;
    int height;
    int size;
    int radius;
} bmp_info_t;


bmp_info_t get_bmp_size(uint8_t *src, uint32_t size);
bmp_info_t get_thumbnail_size(uint8_t *src, uint32_t size);

bool modify(uint8_t *dst,
            uint8_t *src,
            uint32_t size,
            uint8_t *bg_src,
            int16_t x,
            int16_t y,
            uint8_t r,
            uint8_t g,
            uint8_t b
            );

bool thumbnail_modify(uint8_t *src, uint32_t size, const uint8_t *thumbnail_src);


uint8_t *bmp888_to_565(int32_t dst_size, uint8_t *src);
uint8_t *bmp888_to_565_thumbnail(int32_t dst_size, uint8_t *src);

#endif //BIN_MODIFY_H
