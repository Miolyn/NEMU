#include "common.h"

// 2^3 * 2^7 * 2^6
#define SET_WIDTH_l1 3
#define LINE_WIDTH_l1 7
#define BLOCK_WIDTH_l1 6
#define TAG_WIDTH_l1 (32 - SET_WIDTH_l1 - LINE_WIDTH_l1 - BLOCK_WIDTH_l1)

#define CACHE_SET_l1 (1 << SET_WIDTH_l1)
#define CACHE_LINE_l1 (1 << LINE_WIDTH_l1)
#define CACHE_BLOCK_l1 (1 << BLOCK_WIDTH_l1)


#define SET_WIDTH_l2 4
#define LINE_WIDTH_l2 6
#define BLOCK_WIDTH_l2 6
#define TAG_WIDTH_l2 (32 - SET_WIDTH_l2 - LINE_WIDTH_l2 - BLOCK_WIDTH_l2)

#define CACHE_SET_l2 (1 << SET_WIDTH_l2)
#define CACHE_LINE_l2 (1 << LINE_WIDTH_l2)
#define CACHE_BLOCK_l2 (1 << BLOCK_WIDTH_l2)

uint8_t _cache_l1[CACHE_SET_l1][CACHE_LINE_l1][CACHE_BLOCK_l1];
uint8_t _cache_l2[CACHE_SET_l2][CACHE_LINE_l2][CACHE_BLOCK_l2];
uint8_t *hw_cache_l1 = (void*)_cache_l1;
uint8_t *hw_cache_l2 = (void*)_cache_l2;
void init_cache(){
    
}
 
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
