#include "nemu.h"

uint32_t page_translate(lnaddr_t addr, uint32_t len){
	if(!cpu.cr0.protect_enable || !cpu.cr0.paging){
		return addr;
	}

	PageTableEntry dirPageEntry;
	PageTableEntry pageEntry;
	LinearAddr lnAddr;
	lnAddr.val = addr;
	bool suc = false;

	uint32_t tlb_res = read_tlb(addr, &suc);
	if(suc) return tlb_res + lnAddr.offset;
	uint32_t dirBaseAddr = cpu.cr3.page_directory_base;

	uint32_t dirPageEntryVal = hwaddr_read(FRAME_ADDR(dirBaseAddr) + lnAddr.dir * 4, 4);
	dirPageEntry.val = dirPageEntryVal;
	// printf("dir val: %x\n", dirPageEntryVal);
	assert(dirPageEntry.p == 1);
	uint32_t pageEntryVal = hwaddr_read(FRAME_ADDR(dirPageEntry.pageFrameAddr) + lnAddr.page * 4, 4);
	pageEntry.val = pageEntryVal;
	assert(pageEntry.p == 1);
	uint32_t res;
	if(lnAddr.offset + len <= 1 << 12){
		// res = hwaddr_read(FRAME_ADDR(pageEntry.pageFrameAddr) + lnAddr.offset, len);
		res = FRAME_ADDR(pageEntry.pageFrameAddr) + lnAddr.offset;
		write_tlb(addr, FRAME_ADDR(pageEntry.pageFrameAddr));
		// printf("page addr:0x%x\n", res);
	} else{
		assert(0);
	}

	return res;
}