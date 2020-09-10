#include "cpu/exec/template-start.h"

#define instr jcc

#if DATA_BYTE == 1 || DATA_BYTE == 4

make_helper(concat(je_rel_, SUFFIX)){
    printf("start to deal with je%d\n", DATA_BYTE);
    // opcode len
    eip += 1;
    int len = concat(decode_si_, SUFFIX)(eip);
    printf("je%d,simm:%x\n", DATA_BYTE, op_src->simm);
    if(reg_eflags(ZF)){
        printf("je true, jump to addr:%x\n", eip + op_src->simm);
        cpu.eip = eip + op_src->simm;
    }
    
    return len;
}

#endif

#if DATA_BYTE == 2

make_helper(concat(je_rel_, SUFFIX)){
    printf("start to deal with je%d\n", DATA_BYTE);
    eip += 1;
    int len = concat(decode_si_, SUFFIX)(eip);
    printf("je%d,simm:%x\n", DATA_BYTE, op_src->simm);
    if(reg_eflags(ZF)){
        cpu.eip = (eip + op_src->simm) & 0xFFFF;
    }
    return len;
}

#endif

#include "cpu/exec/template-end.h"