#include "nemu.h"
// 2^3 * 2^7 * 2^6
Cache cache_l1;
Cache cache_l2;

cache_initor(l1);
cache_initor(l2);

void init_cache(){
    init_cache_l1();
    init_cache_l2();
}
uint32_t cache_l1_read(struct Cache *this, uint32_t addr, uint32_t len){
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
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
