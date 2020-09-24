#include "nemu.h"
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
Cache_l1 cache_l1;
Cache_l2 cache_l2;

cache_initor(l1);
cache_initor(l2);

void init_cache(){
    init_cache_l1();
    init_cache_l2();
}

