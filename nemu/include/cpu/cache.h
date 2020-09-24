#ifndef __CACHE_H__
#define __CACHE_H__

#include "common.h"

// cache block 64Byte cache:64KB
// 8way
// 2^3 way * 2^7* 64Byte
// extern int8_t *hw_cache;
#define SET_WIDTH 3
#define LINE_WIDTH 7
#define BLOCK_WIDTH 6
#define TAG_WIDTH (32 - SET_WIDTH - LINE_WIDTH - BLOCK_WIDTH)

#define CACHE_SET (1 << SET_WIDTH)
#define CACHE_LINE (1 << LINE_WIDTH)
#define CACHE_BLOCK (1 << BLOCK_WIDTH)
#define SUFFIX l1
#include "cpu/cache-template.h"
#define SET_WIDTH 4
#define LINE_WIDTH 6
#define BLOCK_WIDTH 6
#define TAG_WIDTH (32 - SET_WIDTH - LINE_WIDTH - BLOCK_WIDTH)

#define CACHE_SET (1 << SET_WIDTH)
#define CACHE_LINE (1 << LINE_WIDTH)
#define CACHE_BLOCK (1 << BLOCK_WIDTH)
#define SUFFIX l2
#include "cpu/cache-template.h"
// extern Cache_l1 cache_l1;
// extern Cache_l2 cache_l2;
#define cache_initor(suffix) void concat(init_cache_, suffix)(){ \
    int i, j; \
    for(i = 0; i < concat(CACHE_SET_, suffix); i++){ \
        concat(cache_, suffix).cacheSet[i].setID = i; \
        for(j = 0; j < concat(CACHE_LINE_, suffix); j++){ \
            concat(cache_, suffix).cacheSet[i].cacheLine[j].dirt_bit = 0; \
            concat(cache_, suffix).cacheSet[i].cacheLine[j].valid = 0; \
        } \
    } \
}
extern void init_cache();
#endif