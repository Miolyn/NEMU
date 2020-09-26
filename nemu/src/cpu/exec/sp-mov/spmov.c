#include "cpu/exec/helper.h"

make_helper(mov_cr2r_v){
    // assert(cpu.cr0.protect_enable == 0);
    decode_r2rm_l(eip + 1);
    if(op_src->val == 0){
        reg_l(op_dest->reg) = cpu.cr0.val;
    } else if(op_src->val == 3){
        reg_l(op_dest->reg) = cpu.cr3.val;
    }
    return 2;
}


make_helper(mov_r2cr_v){
    // assert(cpu.cr0.protect_enable == 0);
    decode_rm2r_l(eip + 1);
    if(op_dest->val == 0){
        cpu.cr0.val = reg_l(op_src->reg);
    } else if(op_dest->val == 3){
        cpu.cr3.val = reg_l(op_src->reg);
    }
    return 2;
}