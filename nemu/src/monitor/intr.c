#include <setjmp.h>
#include "monitor/monitor.h"
#include "cpu/helper.h"
#include "cpu/reg.h"
extern jmp_buf jbuf;

void raise_intr(uint8_t NO){
    // Interrupt vector must be within IDT table limits,
    assert((NO << 3) <= cpu.idtr.table_limit);
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
    // Push (long pointer to return location); (* 3 words padded to 4 *) CS:EIP �� selector:offset from gate;
    
    cpu.cs.selector.val = gateDesc.selector;
    load_descriptor(R_CS);
    // IF 32-bit gate
    // THEN CS:EIP �� selector:offset from gate;
    longjmp(jbuf, 1);
}
