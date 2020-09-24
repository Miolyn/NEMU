#include "common.h"

typedef struct {
    uint32_t valid : 1;
    uint32_t tag : TAG_WIDTH;
    uint8_t block[CACHE_BLOCK];
    uint32_t dirt_bit : 1;
} concat(CacheLine_, SUFFIX);

typedef struct {
    concat(CacheLine_, SUFFIX) cacheLine[CACHE_LINE];
    uint32_t setID;
} concat(CacheSet_, SUFFIX);

#pragma pack(1)
typedef struct {
    uint32_t tag : TAG_WIDTH;
    uint32_t set : SET_WIDTH;
    uint32_t blockOffset : BLOCK_WIDTH;
} concat(CacheAddr_, SUFFIX);
#pragma pack()

typedef struct {
    concat(CacheSet_, SUFFIX) cacheSet[CACHE_SET];
    int (*cache_read)(uint32_t addr, uint32_t len);
    void (*cache_write)(uint32_t adr, uint32_t len, uint32_t data);
}concat(Cache_, SUFFIX);

extern uint8_t *concat(hw_cache_, SUFFIX);
extern concat(Cache_, SUFFIX) concat(cache_, SUFFIX);
#define cache_initor(suffix) void concat(init_cache_, suffix)(){ \
    int i, j; \
    for(i = 0; i < CACHE_SET; i++){ \
        concat(cache_, SUFFIX).cacheSet[i].setID = i; \
        for(j = 0; j < CACHE_LINE; j++){ \
            concat(cache_, SUFFIX).cacheSet[i].cacheLine[j].dirt_bit = 0; \
            concat(cache_, SUFFIX).cacheSet[i].cacheLine[j].valid = 0; \
        } \
    } \
}

#undef SET_WIDTH
#undef LINE_WIDTH
#undef BLOCK_WIDTH
#undef TAG_WIDTH

#undef CACHE_SET
#undef CACHE_LINE
#undef CACHE_BLOCK
#undef SUFFIX