#include "cpu/exec/template-start.h"

#define instr call

// static void do_execute(){
//     cpu.eip = cpu.eip + op_src2->imm;

// }

// #if DATA_BYTE == 2
// make_helper(call_rel_l){
//     printf("start call_rel_l at %x, %x\n", eip, instr_fetch(eip + 1, 4));
//     // eip += opcode len
//     eip += 1;
//     int len = concat(decode_i_, SUFFIX)(eip);
//     printf("len:%d,src:%d,call_op-push_addr:%x\n", len, op_src->imm, eip + len);
//     PUSH_STACK((eip + len) & 0xFFFF);
//     cpu.eip = (eip + op_src->imm) & 0xFFFF;
//     return len;
// }
// #endif

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(call_rel_, SUFFIX)){
    int len = concat(decode_si_, SUFFIX)(eip + 1);
    if(ops_decoded.is_operand_size_16){
        panic("hj");
        PUSH_STACK((eip + len + 1) & 0xFFFF);
        cpu.eip = (eip + op_src->val) & 0xFFFF;
    } else{
        PUSH_STACK(eip + len + 1);
        cpu.eip = eip + op_src->val;
    }
    reset_all_eflags();
    print_asm_template1();
    return len + 1;
}

make_helper(concat(call_rm_, SUFFIX)){
    // printf("start call_rm_%d at %x, %x\n", DATA_BYTE, eip, instr_fetch(eip + 1, DATA_BYTE));
    // eip += 1;
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
    print_asm_template1();
    return len + 1;
}
#endif

#include "cpu/exec/template-end.h"