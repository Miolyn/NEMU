#ifndef __CACHE_H__
#define __CACHE_H__

#include "common.h"
#include <stdlib.h>
// cache block 64Byte cache:64KB
// 8way
// 2^3 way * 2^7* 64Byte
// extern int8_t *hw_cache;
#define BLOCK_WIDTH 6
#define CACHE_BLOCK (1 << BLOCK_WIDTH)


#define SET_WIDTH_l1 3
#define LINE_WIDTH_l1 7
#define TAG_WIDTH_l1 (32 - SET_WIDTH_l1 - LINE_WIDTH_l1 - BLOCK_WIDTH)

#define CACHE_SET_l1 (1 << SET_WIDTH_l1)
#define CACHE_LINE_l1 (1 << LINE_WIDTH_l1)


#define SET_WIDTH_l2 4
#define LINE_WIDTH_l2 6
#define TAG_WIDTH_l2 (32 - SET_WIDTH_l2 - LINE_WIDTH_l2 - BLOCK_WIDTH)

#define CACHE_SET_l2 (1 << SET_WIDTH_l2)
#define CACHE_LINE_l2 (1 << LINE_WIDTH_l2)



typedef struct {
    uint32_t valid : 1;
    uint32_t tag;
    uint8_t block[CACHE_BLOCK];
    uint32_t dirt_bit : 1;
} CacheLine;

typedef struct {
    CacheLine *cacheLine;
    int lineNum;
    uint32_t setID;
} CacheSet;


typedef struct Cache{
    CacheSet *cacheSet;
    int setNum;
    int type:1;
    uint32_t (*cache_read)(struct Cache *this, uint32_t addr, uint32_t len);
    int (*cache_find)(struct Cache *this, int set, uint32_t tag);
    void (*cache_write)(struct Cache *this, uint32_t addr, uint32_t len, uint32_t data);
}Cache;

extern Cache cache_l1;
extern Cache cache_l2;

// extern Cache_l1 cache_l1;
// extern Cache_l2 cache_l2;
#define cache_initor(suffix) void concat(init_cache_, suffix)(){ \
    int i, j; \
    concat(cache_, suffix).setNum = concat(CACHE_SET_, suffix); \
    concat(cache_, suffix).cacheSet = (CacheSet*)malloc(concat(CACHE_SET_, suffix) * sizeof(CacheSet)); \
    for(i = 0; i < concat(CACHE_SET_, suffix); i++){ \
        concat(cache_, suffix).cacheSet[i].setID = i; \
        concat(cache_, suffix).cacheSet[i].lineNum = concat(CACHE_LINE_, suffix); \
        concat(cache_, suffix).cacheSet[i].cacheLine = (CacheLine*)malloc(concat(CACHE_LINE_, suffix) * sizeof(CacheLine)); \
        for(j = 0; j < concat(CACHE_LINE_, suffix); j++){ \
            concat(cache_, suffix).cacheSet[i].cacheLine[j].dirt_bit = 0; \
            concat(cache_, suffix).cacheSet[i].cacheLine[j].valid = 0; \
        } \
    } \
}
extern void init_cache();


#pragma pack(1)
typedef struct {
    uint32_t tag : TAG_WIDTH_l1;
    uint32_t set : SET_WIDTH_l1;
    uint32_t blockOffset : BLOCK_WIDTH;
} CacheAddr_l1;
typedef struct {
    uint32_t tag : TAG_WIDTH_l2;
    uint32_t set : SET_WIDTH_l2;
    uint32_t blockOffset : BLOCK_WIDTH;
} CacheAddr_l2;
#pragma pack()
#endif