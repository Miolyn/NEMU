#include "cpu/exec/template-start.h"

#define instr jcc

#if DATA_BYTE == 1 || DATA_BYTE == 4

make_helper(concat(je_rel_, SUFFIX)){
    int len = concat(decode_si_, SUFFIX)(eip);
    cpu.eip = cpu.eip + op_src->imm;
    return len;
}

#endif

#if DATA_BYTE == 2

make_helper(concat(je_rel_, SUFFIX)){
    int len = concat(decode_si_, SUFFIX)(eip);
    cpu.eip = (cpu.eip + op_src->imm) & 0xFFFF;
    return len;
}

#endif

#include "cpu/exec/template-end.h"