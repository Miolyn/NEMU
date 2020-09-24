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
#define TAG_WIDTH_l1 (32 - SET_WIDTH_l1 - BLOCK_WIDTH)

#define CACHE_SET_l1 (1 << SET_WIDTH_l1)
#define CACHE_LINE_l1 (1 << LINE_WIDTH_l1)

#define SET_WIDTH_l2 4
#define LINE_WIDTH_l2 6
#define TAG_WIDTH_l2 (32 - SET_WIDTH_l2 - BLOCK_WIDTH)

#define CACHE_SET_l2 (1 << SET_WIDTH_l2)
#define CACHE_LINE_l2 (1 << LINE_WIDTH_l2)



typedef struct {
    uint32_t valid : 1;
    uint32_t tag;
    uint8_t *block;
    uint32_t dirt_bit : 1;
} CacheLine;

typedef struct {
    CacheLine *cacheLine;
    uint32_t setID;
} CacheSet;

typedef struct{
    uint32_t tag;
    uint32_t set;
    uint32_t blockOffset;
} AddrHelper;

typedef struct Cache{
    CacheSet *cacheSet;
    int setNum;
    int lineNum;
    int blockSize;
    AddrHelper (*getCacheAddr)(struct Cache *this, uint32_t addr);
    void (*cache_read)(struct Cache *this, uint8_t *buf, uint32_t addr, uint32_t len);
    int (*cache_find)(struct Cache *this, int set, uint32_t tag);
    void (*cache_write)(struct Cache *this, uint8_t *buf, uint32_t addr, uint32_t len);
    int (*cache_miss)(struct Cache *this, uint32_t addr);
    void (*cache_deal_dirt)(struct Cache *this, uint32_t addr, int setID, int lineID);
    void (*cache_load_miss)(struct Cache *this, uint32_t addr, CacheLine *linePointer, uint32_t len);
}Cache;

extern Cache cache_l1;
extern Cache cache_l2;

// extern Cache_l1 cache_l1;
// extern Cache_l2 cache_l2;
#define cache_initor(suffix) void concat(init_cache_, suffix)(){ \
    int i, j; \
    concat(cache_, suffix).setNum = concat(CACHE_SET_, suffix); \
    concat(cache_, suffix).cacheSet = (CacheSet*)malloc(concat(CACHE_SET_, suffix) * sizeof(CacheSet)); \
    concat(cache_, suffix).lineNum = concat(CACHE_LINE_, suffix); \
    concat(cache_, suffix).blockSize = CACHE_BLOCK; \
    for(i = 0; i < concat(CACHE_SET_, suffix); i++){ \
        concat(cache_, suffix).cacheSet[i].setID = i; \
        concat(cache_, suffix).cacheSet[i].cacheLine = (CacheLine*)malloc(concat(CACHE_LINE_, suffix) * sizeof(CacheLine)); \
        for(j = 0; j < concat(CACHE_LINE_, suffix); j++){ \
            concat(cache_, suffix).cacheSet[i].cacheLine[j].dirt_bit = 0; \
            concat(cache_, suffix).cacheSet[i].cacheLine[j].valid = 0; \
            concat(cache_, suffix).cacheSet[i].cacheLine[j].block = (uint8_t*)malloc(CACHE_BLOCK * sizeof(uint8_t)); \
        } \
    } \
}
extern void init_cache();

extern uint32_t c_read(uint32_t addr, int len);
extern void c_write(uint32_t addr, int len, uint32_t data);

#endif