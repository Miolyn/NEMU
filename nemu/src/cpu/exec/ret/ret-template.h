#include "cpu/exec/template-start.h"

#define instr ret

#if DATA_BYTE != 1
make_helper(concat(ret_rel_, SUFFIX)){
    printf("``````````````````````````````````````````````````````````````````````````````\n");
    printf("start to deal with ret%d\n", DATA_BYTE);
    printf("ret%d read %x\n", DATA_BYTE, swaddr_read(REG(R_SP), DATA_BYTE) & 0xFFFF);
    if (ops_decoded.is_operand_size_16){
        cpu.eip = swaddr_read(REG(R_ESP), DATA_BYTE) & 0xFFFF;
    } else{
        cpu.eip = swaddr_read(REG(R_SP), DATA_BYTE);
    }
    
    REG(R_ESP) = REG(R_ESP) + 4;
    // control the len 
    // read one opcode before deal
    return -1;
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
    REG(R_ESP) = REG(R_ESP) + op_src->val;
    // control the len 
    // read one opcode before deal
    return 0;
}
#endif


#include "cpu/exec/template-end.h"