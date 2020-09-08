#include "cpu/exec/template-start.h"

#define instr call

// static void do_execute(){
//     cpu.eip = cpu.eip + op_src2->imm;

// }

#if DATA_BYTE == 2
make_helper(call_rel_l){
    decode_si_l(eip);
    cpu.eip = cpu.eip + op_src2->imm;
    return 0;
}
#endif

#if DATA_BYTE == 4
make_helper(call_rel_w){
    decode_si_l(eip);
    cpu.eip = (cpu.eip + op_src->imm) & 0xFFFF;
    return 0;
}
#endif

#include "cpu/exec/template-end.h"