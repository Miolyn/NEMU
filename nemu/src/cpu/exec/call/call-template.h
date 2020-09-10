#include "cpu/exec/template-start.h"

#define instr call

// static void do_execute(){
//     cpu.eip = cpu.eip + op_src2->imm;

// }

#if DATA_BYTE == 2
make_helper(call_rel_l){
    printf("start call_rel_l at %x, %x\n", eip, instr_fetch(eip + 1, 4));
    // eip += opcode len
    eip += 1;
    int len = decode_i_l(eip);
    printf("len:%d,src:%d,call_op-push_addr:%x\n", len, op_src->imm, eip + len);
    PUSH_STACK(eip + len);
    cpu.eip = eip + op_src->imm;
    return len;
}
#endif

#if DATA_BYTE == 4
make_helper(call_rel_w){
    printf("start call_rel_w\n");
    eip += 1;
    int len = decode_i_w(eip);
    PUSH_STACK(eip + len);
    cpu.eip = (eip + op_src->imm) & 0xFFFF;
    // 0x66 prefix return length=1
    // 1 means the len of the opcode
    return len;
}
#endif

#include "cpu/exec/template-end.h"