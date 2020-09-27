#include "cpu/exec/helper.h"

make_helper(mov_sreg2rm_w){
    decode_r2rm_w(eip + 1);
    reg_w(op_dest->reg) = cpu.sRegs[op_src->sreg].selector.val;
    print_asm("movsreg " "sreg%d", op_src->sreg);
    return 2;
}

make_helper(mov_rm2sreg_w){
    decode_rm2r_w(eip + 1);
    // cpu.sRegs[op_src->sreg].selector.val = reg_w(op_dest->reg);
    // printf("op_src %d\n", op_src->reg);
    // printf("sreg %d\n", op_dest->sreg);
    cpu.sRegs[op_dest->sreg].selector.val = reg_w(op_src->reg);
    if(op_dest->sreg == R_SS) printf("hello\n");
    load_descriptor(op_dest->sreg);
    print_asm("movsreg " "sreg%d", op_dest->sreg);
    return 2;
}