#include "cpu/exec/template-start.h"

#define instr ret

#if DATA_BYTE != 1
make_helper(concat(ret_rel_, SUFFIX)){
    printf("``````````````````````````````````````````````````````````````````````````````\n");
    printf("start to deal with ret%d\n", DATA_BYTE);
    printf("ret%d read %x\n", DATA_BYTE, swaddr_read(REG(R_SP), DATA_BYTE) );
    if (ops_decoded.is_operand_size_16){
        cpu.eip = swaddr_read(REG(R_ESP), DATA_BYTE) & 0xFFFF;
    } else{
        cpu.eip = swaddr_read(REG(R_SP), DATA_BYTE);
    }
    
    REG(R_ESP) = REG(R_ESP) + 4;
    print_asm(str(instr) str(SUFFIX));
    // control the len 
    // read one opcode before deal
    return 0;
}

make_helper(concat(ret_i_, SUFFIX)){
    printf("ret at rsp%x\n", reg_l(R_ESP));
    if (ops_decoded.is_operand_size_16){
        cpu.eip = swaddr_read(reg_l(R_ESP), 4) & 0xFFFF;
    } else{
        cpu.eip = swaddr_read(reg_l(R_SP), 4);
    }
    
    REG(R_ESP) = REG(R_ESP) + 4;
    decode_i_w(eip + 1);
    printf("eip:%x,val:%x\n", cpu.eip, op_src->val);
    reg_l(R_ESP) = reg_l(R_ESP) + op_src->val;
    print_asm_template1();
    // control the len 
    // read one opcode before deal
    return 0;
}
#endif


#include "cpu/exec/template-end.h"