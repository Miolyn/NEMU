#include "nemu.h"
// 2^3 * 2^7 * 2^6
Cache cache_l1;
Cache cache_l2;

cache_initor(l1);
cache_initor(l2);

AddrHelper getCacheAddr1(struct Cache *this, uint32_t addr);
AddrHelper getCacheAddr2(struct Cache *this, uint32_t addr);

static uint8_t buf[20];
void init_cache(){
    init_cache_l1();
    init_cache_l2();
    cache_l1.getCacheAddr = getCacheAddr1;
    cache_l1.getCacheAddr = getCacheAddr2;
}
AddrHelper getCacheAddr1(struct Cache *this, uint32_t addr){
    AddrHelper ar;
    ar.blockOffset = addr & ((1 << BLOCK_WIDTH) - 1);
    ar.set = (addr >> BLOCK_WIDTH) & ((1 << SET_WIDTH_l1) - 1);
    ar.tag = (addr >> (BLOCK_WIDTH + SET_WIDTH_l1)) & ((1 << TAG_WIDTH_l1) - 1);

    return ar;
}
AddrHelper getCacheAddr2(struct Cache *this, uint32_t addr){
    AddrHelper ar;
    ar.blockOffset = addr & ((1 << BLOCK_WIDTH) - 1);
    ar.set = (addr >> BLOCK_WIDTH) & ((1 << SET_WIDTH_l2) - 1);
    ar.tag = (addr >> (BLOCK_WIDTH + SET_WIDTH_l2)) & ((1 << TAG_WIDTH_l2) - 1);
    return ar;
}

int cache_find(struct Cache *this, int setID, uint32_t tag){
    CacheSet set = this->cacheSet[setID];
    int i;
    for(i = 0; i < this->lineNum; i++){
        if(set.cacheLine[i].valid && set.cacheLine[i].tag == tag){
            break;
        }
    }
    return i == this->lineNum ? -1 : i;
}

uint32_t cache_read(struct Cache *this, uint32_t addr, uint32_t len){
#ifdef DEBUG
	assert(len <= 4);
#endif
    AddrHelper cAddr = this->getCacheAddr(this, addr);
    int loc = this->cache_find(this, cAddr.set, cAddr.tag);
    memset(buf, 0, sizeof(buf));
    if(loc != -1){
        if(cAddr.blockOffset + len > CACHE_BLOCK){
            memcpy(buf, this->cacheSet[cAddr.set].cacheLine[loc].block + cAddr.blockOffset, CACHE_BLOCK - cAddr.blockOffset);
        } else{
            memcpy(buf, this->cacheSet[cAddr.set].cacheLine[loc].block + cAddr.blockOffset, len);
            // return unalign_rw(buf, 4);
        }
    }

    return 0;
}
