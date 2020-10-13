#include "cpu/exec/helper.h"

make_helper(int_i_b){
    uint8_t NO = instr_fetch(eip + 1, 1);
    cpu.eip += 2;
    print_asm("int %x",NO);
    raise_intr(NO);
    return 0;
}