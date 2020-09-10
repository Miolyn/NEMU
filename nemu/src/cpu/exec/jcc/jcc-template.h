#include "cpu/exec/template-start.h"

#define instr jcc

#define r(index) reg_eflags(index)

// make_helper(concat(je_rel_, SUFFIX)){
//     printf("start to deal with je%d\n", DATA_BYTE);
//     // opcode len
//     eip += 1;
//     int len = concat(decode_si_, SUFFIX)(eip);
//     printf("je%d,simm:%x\n", DATA_BYTE, op_src->simm);
//     if(reg_eflags(ZF)){
//         printf("je true, jump to addr:%x\n", eip + op_src->simm + 1);
//         if (ops_decoded.is_operand_size_16){
//             cpu.eip = (eip + op_src->simm) & 0xFFFF;
//         } else{
//             cpu.eip = eip + op_src->simm;
//         }
//     }
//     return len;
// }

jcc_helper(e, r(ZF))

#undef r
#include "cpu/exec/template-end.h"