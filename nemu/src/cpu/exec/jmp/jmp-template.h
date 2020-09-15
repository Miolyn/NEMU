#include "cpu/exec/template-start.h"

#define instr jmp


make_helper(concat(jmp_rel_, SUFFIX)){
    // printf("start jmp_rel_%d at %x, rel8:0x%x\n", DATA_BYTE, eip, instr_fetch(eip + 1, DATA_BYTE));
    eip += 1;
    int len = concat(decode_si_, SUFFIX)(eip);
    printf("op_src:0x%x\n", op_src->val);
    if(ops_decoded.is_operand_size_16){
        cpu.eip = (eip + op_src->val) & 0xFFFF;
    } else{
        cpu.eip = eip + op_src->val;
    }
    print_asm_template1();
    printf("len%d\n", len);
    return len;
}

#if DATA_BYTE != 1
make_helper(concat(jmp_rm_, SUFFIX)){
    eip += 1;
    int len = concat(decode_rm_, SUFFIX)(eip);
    if(ops_decoded.is_operand_size_16){
        cpu.eip = (eip + op_src->val) & 0xFFFF;
    } else{
        cpu.eip = eip + op_src->val;
    }
    print_asm_template1();
    return len;
}
#endif

#include "cpu/exec/template-end.h"