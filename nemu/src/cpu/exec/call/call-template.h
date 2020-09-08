#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
    W_REG(eip, (cpu.eip + op_src.imm));
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"