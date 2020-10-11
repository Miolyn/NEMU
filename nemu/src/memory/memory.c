#include "common.h"

lnaddr_t seg_translate(swaddr_t addr, uint32_t len, uint32_t sReg);
uint32_t page_translate(lnaddr_t addr, uint32_t len);
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t c_read(uint32_t addr, uint32_t len);
void c_write(uint32_t addr, uint32_t len, uint32_t data);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	// return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	return c_read(addr, len);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	// dram_write(addr, len, data);
	c_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	printf("len lnaddr:%d\n", (int)len);
    addr = page_translate(addr, len);
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
    addr = page_translate(addr, len);
	hwaddr_write(addr, len, data);
}

// uint32_t swaddr_read_(swaddr_t addr, size_t len) {
// #ifdef DEBUG
// 	assert(len == 1 || len == 2 || len == 4);
// #endif
// 	return lnaddr_read(addr, len);
// }

// void swaddr_write_(swaddr_t addr, size_t len, uint32_t data) {
// #ifdef DEBUG
// 	assert(len == 1 || len == 2 || len == 4);
// #endif
// 	lnaddr_write(addr, len, data);
// }



uint32_t swaddr_read(swaddr_t addr, size_t len, uint32_t sReg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif

    addr = seg_translate(addr, len, sReg);
	// int res = c_read(addr, len);
    // lnaddr_read(addr, len);
    return lnaddr_read(addr, len);
}





void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint32_t sReg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    addr = seg_translate(addr, len, sReg);
    lnaddr_write(addr, len, data);
}

