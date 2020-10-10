#include "cpu/exec/helper.h"

make_helper(mov_cr2r_v){
    // assert(cpu.cr0.protect_enable == 0);
    decode_r2rm_l(eip + 1);
    if(op_src->reg == 0){
        // printf("mov cr0 2 %d\n", op_dest->reg);
        // reg_l(op_dest->reg) = cpu.cr0.val;
        reg_l(R_EAX) = cpu.cr0.val;
        // printf("cr0 pe:%d, pagin:%d\n", cpu.cr0.protect_enable, cpu.cr0.paging);
    } else if(op_src->reg == 3){
        // reg_l(op_dest->reg) = cpu.cr3.val;
        reg_l(R_EAX) = cpu.cr3.val;
    }
    print_asm("mov cr%d -> eax", op_src->reg);
    return 2;
}


make_helper(mov_r2cr_v){
    // assert(cpu.cr0.protect_enable == 0);
    decode_rm2r_l(eip + 1);
    if(op_dest->reg == 0){
        // printf("mov %d 2 cr0\n", op_dest->reg);
        // cpu.cr0.val = reg_l(op_src->reg);
        cpu.cr0.val = reg_l(R_EAX);
        // printf("cr0 pe:%d, pagin:%d\n", cpu.cr0.protect_enable, cpu.cr0.paging);
    } else if(op_dest->reg == 3){
        // cpu.cr3.val = reg_l(op_src->reg);
        cpu.cr3.val = reg_l(R_EAX);
        reset_tlb();
        printf("page base addr%x\n", cpu.cr3.page_directory_base);
    }
    print_asm("mov eax -> cr%d", op_dest->reg);
    return 2;
}