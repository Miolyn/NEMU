#include "cpu/exec/template-start.h"

#define instr jcc


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

jcc_helper(a0nbe, (!cpu.CF && !cpu.ZF))
jcc_helper(ae0nb0nc, (!cpu.CF))
jcc_helper(b0c0nae, (cpu.CF))
jcc_helper(be0na, (cpu.CF || cpu.ZF))
// jcc_helper(c, (cpu.CF)))
jcc_helper(e0z, (cpu.ZF))
// ? 
jcc_helper(g0nle, (!cpu.ZF && cpu.SF == cpu.OF))
jcc_helper(ge0nl, (cpu.SF == cpu.OF))
jcc_helper(l0nge, (cpu.SF != cpu.OF))
jcc_helper(le0ng, (cpu.ZF || cpu.SF != cpu.OF))
// jcc_helper(na, (cpu.CF)))
// jcc_helper(nae, (cpu.CF)))
// jcc_helper(nb, (!cpu.CF)))
// jcc_helper(nbe, (!cpu.CF) && !cpu.ZF)))
// jcc_helper(nc, (!cpu.CF)))
jcc_helper(ne0nz, (!cpu.ZF))
// jcc_helper(ng, (cpu.ZF) || cpu.SF) != cpu.OF)))
// jcc_helper(nge, (cpu.SF) != cpu.OF)))
// jcc_helper(nl, (cpu.SF) == cpu.OF)))
// jcc_helper(nle, (cpu.ZF) && cpu.SF) != cpu.OF)))
jcc_helper(no, (!cpu.OF))
jcc_helper(np0po, (!cpu.PF))
jcc_helper(ns, (!cpu.SF))
// jcc_helper(nz, (!cpu.ZF)))
jcc_helper(o, (cpu.OF))
jcc_helper(p0pe, (cpu.PF))
// jcc_helper(pe, (cpu.PF)))
// jcc_helper(po, (!cpu.PF)))
jcc_helper(s, (cpu.SF))
// jcc_helper(z, (cpu.ZF)))

jcc_helper(ecxz, (!REG(R_ECX)))

#undef r
#include "cpu/exec/template-end.h"