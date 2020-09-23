#include "common.h"
// 2^3 * 2^7 * 2^6
// uint8_t _cache[CACHE_SET][CACHE_LINE][CACHE_BLOCK];
// uint8_t *hw_cache = (void*)_cache;
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
// typedef struct {
//     uint32_t valid : 1;
//     uint32_t tag : TAG_WIDTH;
//     uint8_t block[CACHE_BLOCK];
//     uint32_t dirt_bit : 1;
// } CacheLine;

// typedef struct {
//     CacheLine cacheLine[CACHE_LINE];
//     uint32_t setID;
// } CacheSet;

// #pragma pack(1)
// typedef struct {
//     uint32_t tag : TAG_WIDTH;
//     uint32_t set : SET_WIDTH;
//     uint32_t blockOffset : BLOCK_WIDTH;
// } cacheAddr;
// #pragma pack()

// typedef struct {
//     CacheSet cacheSet[CACHE_SET];
//     int (*cache_read)(uint32_t addr, uint32_t len);
//     void (*cache_write)(uint32_t adr, uint32_t len, uint32_t data);
// }Cache;
