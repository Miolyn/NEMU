#include "cpu/exec/template-start.h"

#define instr pop

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute(){
    printf("pop start\n");
    POP_STACK(op_src);
}

make_instr_helper(m)
make_instr_helper(r)

#endif
#include "cpu/exec/template-end.h"