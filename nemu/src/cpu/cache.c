#include "nemu.h"
#include <stdlib.h>
#include <time.h>
#define random(x) (rand()%x)
// 2^3 * 2^7 * 2^6
Cache cache_l1;
Cache cache_l2;
uint8_t buf[64];

cache_initor(l1);
cache_initor(l2);

AddrHelper getCacheAddr1(struct Cache *this, uint32_t addr);
AddrHelper getCacheAddr2(struct Cache *this, uint32_t addr);
int cache_find(struct Cache *this, uint32_t setID, uint32_t tag);
int cache_miss(struct Cache *this, uint32_t addr);
void cache_read(struct Cache *this, uint8_t *buf, uint32_t addr, uint32_t len);
void cache_write(struct Cache *this, uint8_t *buf, uint32_t addr, uint32_t len);
int cache_miss(struct Cache *this, uint32_t addr);
void cache_deal_dirt_l1(struct Cache *this, uint32_t addr, uint32_t setID, uint32_t lineID);
void cache_deal_dirt_l2(struct Cache *this, uint32_t addr, uint32_t setID, uint32_t lineID);
void cache_load_miss_l1(struct Cache *this, uint32_t addr, CacheLine *pl, uint32_t len);
void cache_load_miss_l2(struct Cache *this, uint32_t addr, CacheLine *pl, uint32_t len);
void init_cache(){
    srand((int)time(0));
    init_cache_l1();
    init_cache_l2();
    cache_l1.getCacheAddr = getCacheAddr1;
    cache_l2.getCacheAddr = getCacheAddr2;

    cache_l1.cache_find = cache_find;
    cache_l2.cache_find = cache_find;

    cache_l1.cache_read = cache_read;
    cache_l2.cache_read = cache_read;

    cache_l1.cache_write = cache_write;
    cache_l2.cache_write = cache_write;

    cache_l1.cache_miss = cache_miss;
    cache_l2.cache_miss = cache_miss;

    cache_l1.cache_deal_dirt = cache_deal_dirt_l1;
    cache_l2.cache_deal_dirt = cache_deal_dirt_l2;

    cache_l1.cache_load_miss = cache_load_miss_l1;
    cache_l2.cache_load_miss = cache_load_miss_l2;
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

int cache_find(struct Cache *this, uint32_t setID, uint32_t tag){
    CacheSet *set = &(this->cacheSet[setID]);
    int i;
    for(i = 0; i < this->lineNum; i++){
        if(set->cacheLine[i].valid && set->cacheLine[i].tag == tag){
            break;
        }
    }
    return i == this->lineNum ? -1 : i;
}

void cache_read(struct Cache *this, uint8_t *buf, uint32_t addr, uint32_t len){
    AddrHelper cAddr = this->getCacheAddr(this, addr);
    int loc = this->cache_find(this, cAddr.set, cAddr.tag);
    if(loc == -1) loc = this->cache_miss(this, addr);

    if(cAddr.blockOffset + len > CACHE_BLOCK){
        assert(0);
        memcpy(buf, this->cacheSet[cAddr.set].cacheLine[loc].block + cAddr.blockOffset, CACHE_BLOCK - cAddr.blockOffset);
        this->cache_read(this, buf + CACHE_BLOCK - cAddr.blockOffset, addr + CACHE_BLOCK - cAddr.blockOffset, cAddr.blockOffset + len - CACHE_BLOCK);
    } else{
        memcpy(buf, this->cacheSet[cAddr.set].cacheLine[loc].block + cAddr.blockOffset, len);
    }
}

void cache_write(struct Cache *this, uint8_t *buf, uint32_t addr, uint32_t len){
    AddrHelper cAddr = this->getCacheAddr(this, addr);
    int loc = this->cache_find(this, cAddr.set, cAddr.tag);
    if(loc == -1) loc = this->cache_miss(this, addr);
    if(cAddr.blockOffset + len > CACHE_BLOCK){
        assert(0);
        memcpy(this->cacheSet[cAddr.set].cacheLine[loc].block + cAddr.blockOffset, buf, CACHE_BLOCK - cAddr.blockOffset);
        this->cache_write(this, buf + CACHE_BLOCK - cAddr.blockOffset, addr + CACHE_BLOCK - cAddr.blockOffset, cAddr.blockOffset + len - CACHE_BLOCK);
    } else{
        memcpy(this->cacheSet[cAddr.set].cacheLine[loc].block + cAddr.blockOffset, buf, len);
    }
    this->cacheSet[cAddr.set].cacheLine[loc].dirt_bit = 1;
}

int cache_miss(struct Cache *this, uint32_t addr){
    addr = addr & ~((1 << BLOCK_WIDTH) - 1);

    AddrHelper cAddr = this->getCacheAddr(this, addr);
    CacheSet *sp = &(this->cacheSet[cAddr.set]);
    int i;
    for(i = 0; i < this->lineNum; i++){
        if(sp->cacheLine[i].valid) continue;
        else break;
    }
    if(i == this->lineNum){
        i = random(this->lineNum);
    }
    CacheLine *pl = &(sp->cacheLine[i]);
    this->cache_deal_dirt(this, addr, cAddr.set, i);
    this->cache_load_miss(this, addr, pl, CACHE_BLOCK);
    return i;
}

void cache_deal_dirt_l1(struct Cache *this, uint32_t addr, uint32_t setID, uint32_t lineID){
    if(!this->cacheSet[setID].cacheLine[lineID].dirt_bit || !this->cacheSet[setID].cacheLine[lineID].valid) return;
    cache_l2.cache_write(&cache_l2, this->cacheSet[setID].cacheLine[lineID].block, addr, CACHE_BLOCK);
    this->cacheSet[setID].cacheLine[lineID].dirt_bit = 0;
}

void cache_load_miss_l1(struct Cache *this, uint32_t addr, CacheLine *pl, uint32_t len){
    AddrHelper cAddr = this->getCacheAddr(this, addr);
    cache_l2.cache_read(&cache_l2, pl->block, addr, len);
    pl->tag = cAddr.tag;
    pl->valid = 1;
}

void cache_deal_dirt_l2(struct Cache *this, uint32_t addr, uint32_t setID, uint32_t lineID){
    if(!this->cacheSet[setID].cacheLine[lineID].dirt_bit || !this->cacheSet[setID].cacheLine[lineID].valid) return;
    int i;
    for(i = 0; i < CACHE_BLOCK; i++){
        lnaddr_write(addr + i, 1, this->cacheSet[setID].cacheLine[lineID].block[i]);
    }
    this->cacheSet[setID].cacheLine[lineID].dirt_bit = 0;
}

void cache_load_miss_l2(struct Cache *this, uint32_t addr, CacheLine *pl, uint32_t len){
    AddrHelper cAddr = this->getCacheAddr(this, addr);
    int i;
    for(i = 0; i < CACHE_BLOCK; i++){
        pl->block[i] = lnaddr_read(addr + i, 1);
    }
    pl->tag = cAddr.tag;
    pl->valid = 1;
}

uint32_t c_read(uint32_t addr, uint32_t len){
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);
    memset(buf, 0, sizeof(uint8_t) * 64);
    cache_l1.cache_read(&cache_l1, buf, addr, len);
    return buf2uint(buf);
}

void c_write(uint32_t addr, uint32_t len, uint32_t data){
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);
    memset(buf, 0, sizeof(uint8_t) * 64);
    uint2buf(buf, data);
    cache_l1.cache_write(&cache_l1, buf, addr, len);
}


uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	int res = c_read(addr, len);
    return res;
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	c_write(addr, len, data);
}
