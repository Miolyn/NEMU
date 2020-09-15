#include "cpu/exec/template-start.h"

#define instr pop

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute(){
    POP_STACK(op_src);
    // printf("after pop %x\n", reg_l(R_ESP));
    print_asm(str(instr) str(SUFFIX));
}

make_instr_helper(m)
make_instr_helper(r)

#endif
#include "cpu/exec/template-end.h"