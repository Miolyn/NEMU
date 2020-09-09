#include "cpu/exec/template-start.h"

#define instr call

// static void do_execute(){
//     cpu.eip = cpu.eip + op_src2->imm;

// }

#if DATA_BYTE == 2
make_helper(call_rel_l){
    printf("start call_rel_l\n");
    int len = decode_si_l(eip);
    PUSH_STACK(eip + len);
    cpu.eip = cpu.eip + op_src->imm;
    return -1;
}
#endif

#if DATA_BYTE == 4
make_helper(call_rel_w){
    printf("start call_rel_w\n");
    int len = decode_si_w(eip);
    PUSH_STACK(eip + len);
    cpu.eip = (cpu.eip + op_src->imm) & 0xFFFF;
    // 0x66 prefix return length=1
    return -1;
}
#endif

#include "cpu/exec/template-end.h"