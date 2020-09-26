#include "cpu/exec/helper.h"

make_helper(mov_cr2r_v){
    assert(cpu.cr0.protect_enable == 0);
    int modrm = instr_fetch(eip + 1, 1);
    int no = (modrm >> 3 & 0x7);
    if(ops_decoded.is_operand_size_16){
        if (no == 0){
            reg_w(R_EAX) = cpu.cr0.val;
        } else if(no == 3){
            reg_w(R_EAX) = cpu.cr3.val;
        }
    } else{
        if (no == 0){
            reg_l(R_EAX) = cpu.cr0.val;
        } else if(no == 3){
            reg_l(R_EAX) = cpu.cr3.val;
        }
    }
    return 2;
}


make_helper(mov_r2cr_v){
    assert(cpu.cr0.protect_enable == 0);
    int modrm = instr_fetch(eip + 1, 1);
    int no = (modrm >> 3 & 0x7);
    if(ops_decoded.is_operand_size_16){
        if (no == 0){
            cpu.cr0.val = reg_w(R_EAX);
        } else if(no == 3){
            cpu.cr3.val= reg_w(R_EAX);
        }
    } else{
        if (no == 0){
            cpu.cr0.val = reg_l(R_EAX);
        } else if(no == 3){
            cpu.cr3.val = reg_l(R_EAX);
        }
    }
    return 2;
}