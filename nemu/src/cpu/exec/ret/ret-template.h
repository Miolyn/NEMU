#include "cpu/exec/template-start.h"

#define instr ret

#if DATA_BYTE != 1
make_helper(concat(ret_rel_, SUFFIX)){
    if (ops_decoded.is_operand_size_16){
        // cpu.eip = swaddr_read(reg_l(R_ESP), 4) & 0xFFFF;
        cpu.eip = swaddr_read(reg_l(R_ESP), 2) & 0xFFFF;
        reg_l(R_ESP) = reg_l(R_ESP) + 2;
    } else{
        cpu.eip = swaddr_read(reg_l(R_SP), 4);
        reg_l(R_ESP) = reg_l(R_ESP) + 4;
    }
    print_asm(str(instr) str(SUFFIX));
    // control the len 
    // read one opcode before deal
    return 0;
}

make_helper(concat(ret_i_, SUFFIX)){
    // printf("ret at rsp%x\n", reg_l(R_ESP));
    // panic("s");
    if (ops_decoded.is_operand_size_16){
        // cpu.eip = swaddr_read(reg_l(R_ESP), 4) & 0xFFFF;
        cpu.eip = swaddr_read(reg_l(R_ESP), 2) & 0xFFFF;
        reg_l(R_ESP) = reg_l(R_ESP) + 2;
    } else{
        cpu.eip = swaddr_read(reg_l(R_SP), 4);
        reg_l(R_ESP) = reg_l(R_ESP) + 4;
    }
    
    decode_si_w(eip + 1);
    // printf("eip:%x,val:%x\n", cpu.eip, op_src->val);
    reg_l(R_ESP) = reg_l(R_ESP) + op_src->val;
    print_asm_template1();
    // control the len 
    // read one opcode before deal
    return 0;
}
#endif


#include "cpu/exec/template-end.h"