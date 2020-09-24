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
extern Cache_l1 cache_l1;
// extern Cache_l2 cache_l2;
#endif