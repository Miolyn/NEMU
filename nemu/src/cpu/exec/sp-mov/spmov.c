#include "cpu/exec/helper.h"

make_helper(mov_cr2r_v){
    // assert(cpu.cr0.protect_enable == 0);
    decode_r2rm_l(eip + 1);
    if(op_src->reg == 0){
        printf("mov cr0 2 %d\n", op_dest->reg);
        reg_l(op_dest->reg) = cpu.cr0.val;
        printf("cr0 pe:%d\n", cpu.cr0.protect_enable);
    } else if(op_src->reg == 3){
        reg_l(op_dest->reg) = cpu.cr3.val;
    }
    return 2;
}


make_helper(mov_r2cr_v){
    // assert(cpu.cr0.protect_enable == 0);
    decode_rm2r_l(eip + 1);
    if(op_dest->reg == 0){
        printf("mov %d 2 cr0\n", op_dest->reg);
        cpu.cr0.val = reg_l(op_src->reg);
        printf("cr0 pe:%d\n", cpu.cr0.protect_enable);
    } else if(op_dest->reg == 3){
        assert(0);
        cpu.cr3.val = reg_l(op_src->reg);
    }
    return 2;
}