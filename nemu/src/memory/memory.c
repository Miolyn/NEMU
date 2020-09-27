#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	dram_write(addr, len, data);
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

