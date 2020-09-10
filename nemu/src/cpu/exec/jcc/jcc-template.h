#include "cpu/exec/template-start.h"

#define instr jcc


make_helper(concat(je_rel_, SUFFIX)){
    printf("start to deal with je%d\n", DATA_BYTE);
    // opcode len
    eip += 1;
    int len = concat(decode_si_, SUFFIX)(eip);
    printf("je%d,simm:%x\n", DATA_BYTE, op_src->simm);
    if(reg_eflags(ZF)){
        printf("je true, jump to addr:%x\n", eip + op_src->simm + 1);
        if (ops_decoded.is_operand_size_16){
            cpu.eip = (eip + op_src->simm) & 0xFFFF;
        } else{
            cpu.eip = eip + op_src->simm;
        }
        
    }
    
    return len;
}


// #if DATA_BYTE == 2

// make_helper(concat(je_rel_, SUFFIX)){
//     printf("start to deal with je%d\n", DATA_BYTE);
//     eip += 1;
//     int len = concat(decode_si_, SUFFIX)(eip);
//     printf("je%d,simm:%x\n", DATA_BYTE, op_src->simm);
//     if(reg_eflags(ZF)){
//         printf("je true, jump to addr:%x\n", eip + op_src->simm + 1);
//         cpu.eip = (eip + op_src->simm) & 0xFFFF;
//     }
//     return len;
// }

// #endif

#include "cpu/exec/template-end.h"