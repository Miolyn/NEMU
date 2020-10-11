#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "common.h"

#define HW_MEM_SIZE (128 * 1024 * 1024)

extern uint8_t *hw_mem;

/* convert the hardware address in the test program to virtual address in NEMU */
#define hwa_to_va(p) ((void *)(hw_mem + (unsigned)p))
/* convert the virtual address in NEMU to hardware address in the test program */
#define va_to_hwa(p) ((hwaddr_t)((void *)p - (void *)hw_mem))

#define hw_rw(addr, type) *(type *)({\
	Assert(addr < HW_MEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
	hwa_to_va(addr); \
})

uint32_t _swaddr_read(swaddr_t, size_t);
uint32_t swaddr_read(swaddr_t, size_t, uint32_t);
uint32_t swaddr_read_seg(swaddr_t, size_t, uint32_t);
extern uint32_t lnaddr_read(lnaddr_t, size_t);
uint32_t hwaddr_read(hwaddr_t, size_t);
extern void _swaddr_write(swaddr_t, size_t, uint32_t);
extern void swaddr_write(swaddr_t, size_t, uint32_t, uint32_t);
extern void swaddr_write_seg(swaddr_t, size_t, uint32_t, uint32_t);
void lnaddr_write(lnaddr_t, size_t, uint32_t);
void hwaddr_write(hwaddr_t, size_t, uint32_t);
#define swr4(addr, sReg) swaddr_read(addr, 4, sReg)
uint32_t c_read(uint32_t addr, uint32_t len);
void c_write(uint32_t addr, uint32_t len, uint32_t data);
extern uint32_t buf2uint(uint8_t *buf);
lnaddr_t seg_translate(swaddr_t addr, uint32_t len, uint32_t sReg);
uint32_t page_translate(lnaddr_t addr, uint32_t len);

extern void uint2buf(uint8_t *buf, uint32_t data);
#endif
