#include "nemu.h"
// 2^3 * 2^7 * 2^6
Cache cache_l1;
Cache cache_l2;

cache_initor(l1);
cache_initor(l2);
AddrHelper getCacheAddr(struct Cache *this, uint32_t *addr);
void init_cache(){
    init_cache_l1();
    init_cache_l2();
    cache_l1.getCacheAddr = getCacheAddr;
}
AddrHelper getCacheAddr(struct Cache *this, uint32_t *addr){
    AddrHelper ar;
    if(this->type == 0){
        CacheAddr_l1 cAddr = *((CacheAddr_l1*)addr);
        ar.set = cAddr.set;
        ar.tag = cAddr.tag;
        ar.blockOffset = cAddr.blockOffset;
    } else{
        CacheAddr_l2 cAddr = *((CacheAddr_l2*)addr);
        ar.set = cAddr.set;
        ar.tag = cAddr.tag;
        ar.blockOffset = cAddr.blockOffset;
    }
    return ar;
}


int cache_find(struct Cache *this, int setID, uint32_t tag){
    // CacheSet set = this->cacheSet[setID];
    // int i;
    return 0;
}
uint32_t cache_read(struct Cache *this, uint32_t addr, uint32_t len){
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    AddrHelper cAddr = this->getCacheAddr(this, &addr);
    this->cache_find(this, cAddr.set, cAddr.tag);

    // CacheAddr_l1 cAddr = *((CacheAddr_l1*)(&addr));
    // int i;
    // int setID =cAddr.set;
    // CacheSet set = this->set;
    // for(i = 0; i < CACHE_LINE_l1; i++){
    //     if(set.cacheLine[i].valid && set.cacheLine[i].tag == cAddr.tag){
    //         break;
    //     }
    // }
    return 0;
}
