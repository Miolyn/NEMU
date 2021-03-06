#include "cpu/exec/template-start.h"

#define instr pop

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute(){
    POP_STACK(op_src);
    print_asm(str(instr) str(SUFFIX));
}

make_instr_helper(rm)
make_instr_helper(r)

#endif
#include "cpu/exec/template-end.h"