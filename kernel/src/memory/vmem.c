#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)

/* Use the function to get the start address of user page directory. */
PDE* get_kpdir();
PDE* get_updir();
void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	// return (PTE *)va_to_pa(kptable);

	int pages = SCR_SIZE / PAGE_SIZE + !!(SCR_SIZE % PAGE_SIZE);
	
	extern void create_user_mapping(int uptable_id, unsigned physaddr, unsigned virtaddr);
	
	printk("create video mapping for user address space ...\n");
	unsigned addr = 0xa0000;
	int i;
	for (i = 0; i < pages; i++) {
	    create_user_mapping(0, addr, addr);
	    addr += PAGE_SIZE;
	}

	// PDE *pdir = get_updir();
	// PTE *pt = my_pt[0];
	// pdir[0].val = make_pde(va_to_pa(pt));
	// int total = SCR_SIZE / PAGE_SIZE + 1;
	// uint32_t addr = VMEM_ADDR;
	// int i = 0;
	// for(; i < total; i++){
	// 	pt[(addr >> 12) & 0x3ff].val = make_pte(addr);
	// 	addr += PAGE_SIZE;
	// }
	// PTE *ptable = getPTE();
	// int i = VMEM_ADDR / PAGE_SIZE;
	// void *pframe_addr = ((void*)VMEM_ADDR);
	// for(; i < (VMEM_ADDR + SCR_SIZE) / PAGE_SIZE + 1; i++){
	// 	ptable[i].val = make_pte(pframe_addr);
	// 	pframe_addr += PAGE_SIZE;
	// }
	// panic("please implement me");
}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

