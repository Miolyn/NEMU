#include "cpu/exec/template-start.h"

#define instr call

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(call_rel_, SUFFIX)){
    int len = concat(decode_si_, SUFFIX)(eip + 1);
    if(ops_decoded.is_operand_size_16){
        PUSH_STACK((eip + len + 1) & 0xFFFF);
        cpu.eip = (eip + op_src->val) & 0xFFFF;
    } else{
        PUSH_STACK(eip + len + 1);
        cpu.eip = eip + op_src->val;
    }
    reset_all_eflags();
    print_asm(str(instr) str(SUFFIX) " 0x%x", cpu.eip + len + 1);
    return len + 1;
}

make_helper(concat(call_rm_, SUFFIX)){

    int len = concat(decode_rm_, SUFFIX)(eip + 1);
    if(ops_decoded.is_operand_size_16){
        PUSH_STACK((eip + len + 1) & 0xFFFF);
        // cpu.eip = (eip + op_src->val) & 0xFFFF;
        cpu.eip = op_src->val & 0xFFFF;
    } else{
        PUSH_STACK(eip + len + 1);
        // cpu.eip = eip + op_src->val;
        cpu.eip = op_src->val;
    }
    reset_all_eflags();
    print_asm(str(instr) str(SUFFIX) str(cpu.eip + len + 1));
    return len + 1;
}
#endif

#include "cpu/exec/template-end.h"