#include "cpu/exec/template-start.h"

#define instr jmp


make_helper(concat(jmp_rel_, SUFFIX)){
    eip += 1;
    int len = concat(decode_si_, SUFFIX)(eip);
    if(ops_decoded.is_operand_size_16){
        cpu.eip = (eip + op_src->val) & 0xFFFF;
    } else{
        cpu.eip = eip + op_src->val;
    }
    reset_all_eflags();
    print_asm_template1();
    return len;
}

#if DATA_BYTE != 1
make_helper(concat(jmp_rm_, SUFFIX)){
    eip += 1;
    concat(decode_rm_, SUFFIX)(eip);
    if(ops_decoded.is_operand_size_16){
        cpu.eip = op_src->val & 0xffff;
    } else{
        cpu.eip = op_src->val;
    }
    print_asm_template1();
    reset_all_eflags();
    return 0;
}
#endif

#include "cpu/exec/template-end.h"