#ifndef __CACHE_H__
#define __CACHE_H__

#include "common.h"

// cache block 64Byte cache:64KB
// 8way 
// 2^3 way * 2^7* 64Byte
#define SET_WIDTH 3
#define LINE_WIDTH 7
#define BLOCK_WIDTH 6
#define TAG_WIDTH (32 - SET_WIDTH - LINE_WIDTH - BLOCK_WIDTH)

#define CACHE_SET (1 << SET_WIDTH)
#define CACHE_LINE (1 << LINE_WIDTH)
#define CACHE_BLOCK (1 << BLOCK_WIDTH)
typedef struct{
    uint32_t valid :1;
    uint32_t tag :TAG_WIDTH;
    uint8_t block[CACHE_BLOCK];
    uint32_t dirt_bit :1;
}Cacheline;



#endif