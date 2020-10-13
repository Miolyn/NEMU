#include <setjmp.h>
#include "monitor/monitor.h"
#include "cpu/helper.h"
#include "cpu/reg.h"
extern jmp_buf jbuf;

static void push(uint32_t val){
    reg_l(R_ESP) -= 4;
    swaddr_write(reg_l(R_ESP), 4, val, R_SS);
}

void raise_intr(uint8_t NO){
    // Interrupt vector must be within IDT table limits,
    assert((NO << 3) <= cpu.idtr.table_limit);
    if(!cpu.cr0.protect_enable){
        cpu.IF = 0;
        cpu.TF = 0;
    }
    GateDesc gateDesc;
    uint32_t addr = cpu.idtr.base_addr + (NO << 3);
    gateDesc.dword0 = lnaddr_read(addr, 4);
    gateDesc.dword1 = lnaddr_read(addr + 4, 4);
    // Gate must be present, else #NP(vector number * 8+2+EXT);
    assert(gateDesc.present);
    // Selector must be within its descriptor table limits
    assert(((gateDesc.selector) >> 3 << 3) <= cpu.gdtr.table_limit);

    // INTERRUPT-TO-SAME-PRIVILEGE-LEVEL:
    // IF 32-bit gate
    // THEN
    // Push (EFLAGS);
    // Push (long pointer to return location); (* 3 words padded to 4 *) CS:EIP ¡û selector:offset from gate;
    // Figure 9-5 instr the stack
    push(cpu.ef);
    push(cpu.cs.selector.val);
    push(cpu.eip);
    cpu.cs.selector.val = gateDesc.selector;
    printf("before start load\n");
    if(cpu.cr0.protect_enable){
        load_descriptor(R_CS);
    }
    printf("complete load\n");
    // IF 32-bit gate
    // THEN CS:EIP ¡û selector:offset from gate;

    uint32_t offset = gateDesc.offset_15_0 + (gateDesc.offset_31_16 << 16);
    cpu.eip = (cpu.sRegs[R_CS].base_addr << 4) + offset; 
    longjmp(jbuf, 1);
}
