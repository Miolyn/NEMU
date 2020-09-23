#include "cpu/exec/template-start.h"

#define instr bt

#if DATA_BYTE != 1
static void do_execute(){
    cpu.CF = (op_dest->val >> op_src->val) & 1; 
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)
#endif



#include "cpu/exec/template-end.h"