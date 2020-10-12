#include "cpu/exec/template-start.h"

#define instr pop

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute(){
    POP_STACK(op_src);
    print_asm(str(instr) str(SUFFIX));
}

make_instr_helper(rm)
make_instr_helper(r)

make_helper(concat(popa_, SUFFIX)){
    REG(R_EDI) = POP();
    REG(R_ESI) = POP();
    REG(R_EBP) = POP();
    POP();
    REG(R_EBP) = POP();
    REG(R_EDX) = POP();
    REG(R_ECX) = POP();
    REG(R_EAX) = POP();
    return 1;
}
#endif
#include "cpu/exec/template-end.h"