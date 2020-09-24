#include "common.h"

// typedef struct {
//     uint32_t valid : 1;
//     uint32_t tag;
//     uint8_t block[CACHE_BLOCK];
//     uint32_t dirt_bit : 1;
// } concat(CacheLine_, SUFFIX);

// typedef struct {
//     concat(CacheLine_, SUFFIX) cacheLine[CACHE_LINE];
//     uint32_t setID;
// } concat(CacheSet_, SUFFIX);

// #pragma pack(1)
// typedef struct {
//     uint32_t tag : TAG_WIDTH;
//     uint32_t set : SET_WIDTH;
//     uint32_t blockOffset : BLOCK_WIDTH;
// } concat(CacheAddr_, SUFFIX);
// #pragma pack()

// typedef struct {
//     concat(CacheSet_, SUFFIX) cacheSet[CACHE_SET];
//     uint32_t (*cache_read)(uint32_t addr, uint32_t len);
//     int (*cache_find)(uint32_t tag);
//     void (*cache_write)(uint32_t adr, uint32_t len, uint32_t data);
// }concat(Cache_, SUFFIX);

// extern uint8_t *concat(hw_cache_, SUFFIX);
// extern concat(Cache_, SUFFIX) concat(cache_, SUFFIX);


// #undef SET_WIDTH
// #undef LINE_WIDTH
// #undef BLOCK_WIDTH
// #undef TAG_WIDTH

// #undef CACHE_SET
// #undef CACHE_LINE
// #undef CACHE_BLOCK
// #undef SUFFIX