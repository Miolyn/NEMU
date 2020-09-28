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
        printf("swaddr jmp:%x\n", swaddr_read(op_src->val, 4, R_DS));
        // cpu.eip = swaddr_read(op_src->val, 4, R_DS);
    }
    print_asm_template1();
    reset_all_eflags();
    return 0;
}

make_helper(concat(ljmp_ptr_, SUFFIX)){
    ++eip;
    uint32_t pt0 = 0;
    uint16_t pt1 = 0;
    int len = 0;
    if(ops_decoded.is_operand_size_16){
        pt0 = instr_fetch(eip, 2);
        pt1 = instr_fetch(eip + 2, 2);
        len = -1;
    } else{
        pt0 = instr_fetch(eip, 4);
        pt1 = instr_fetch(eip + 4, 2);
    }
    cpu.eip = pt0;
    cpu.sRegs[R_CS].selector.val = pt1;
    load_descriptor(R_CS);
    print_asm("ljmp" " %x, %x", pt0, pt1);
    return len;
}
#endif

#include "cpu/exec/template-end.h"