#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE)
/* Use the function to get the start address of user page directory. */
PDE* get_updir();
PTE vmem_pt[1024] align_to_page;
void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	PDE *pdp = (PDE *)va_to_pa(get_updir());
//	pdp[VMEM_ADDR/PT_SIZE].val=0;
	pdp[VMEM_ADDR/PT_SIZE].page_frame=(uint32_t)va_to_pa(vmem_pt)>>12;
	pdp[VMEM_ADDR/PT_SIZE].present=1;
	int index = VMEM_ADDR/PAGE_SIZE;
	for(;index<(VMEM_ADDR+SCR_SIZE)/PAGE_SIZE+1;index++){
		//Log("addr is %x",vmem_pt+index);
//		vmem_pt[index].val=0;
		vmem_pt[index].page_frame=index+(NR_PT-1)*NR_PTE;
		//Log("%x.val is %x\n",index,vmem_pt[index].page_frame);
		vmem_pt[index].present=1;
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

