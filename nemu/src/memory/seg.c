#include "nemu.h"


void load_descriptor(uint8_t sReg){
	assert(cpu.cr0.protect_enable);
	assert(R_ES <= sReg && sReg <= R_GS);
	uint32_t baseAddr = cpu.gdtr.base_addr;
	// printf("seg base addr:%x\n", baseAddr);
	uint32_t index = cpu.sRegs[sReg].selector.index;
	SegDescriptor des;
	des.dword0 = lnaddr_read(baseAddr + index * 8, 4);
	des.dword1 = lnaddr_read(baseAddr + index * 8 + 4, 4);
	cpu.sRegs[sReg].base_addr0 = des.seg_base0;
	cpu.sRegs[sReg].base_addr1 = des.seg_base1;
	cpu.sRegs[sReg].base_addr2 = des.seg_base2;
	cpu.sRegs[sReg].seg_limit0 = des.seg_limit0;
	cpu.sRegs[sReg].seg_limit1 = des.seg_limit1;
	// printf("sreg:%d, base:%d\n", sReg, cpu.sRegs[sReg].base_addr);
	if(!des.G){
		// byte
		cpu.sRegs[sReg].seg_limit &= 0x000fffff;
	} else{
		// 4k
		cpu.sRegs[sReg].seg_limit <<= 12;
		cpu.sRegs[sReg].seg_limit &= 0xffffffff;
	}
}


lnaddr_t seg_translate(swaddr_t addr, uint32_t len, uint32_t sReg){
	lnaddr_t baseAddr = cpu.sRegs[sReg].base_addr;
	lnaddr_t lnAddr = (baseAddr << 4) + addr;
	// lnaddr_t lnAddr = (baseAddr) + addr;
	assert(lnAddr + 4 <= cpu.sRegs[sReg].seg_limit);
// #ifdef IA32_SEG
	if(cpu.cr0.protect_enable){
		return lnAddr;
	} else{
		return addr;
	}
}