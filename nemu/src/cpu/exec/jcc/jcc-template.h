#include "cpu/exec/template-start.h"

#define instr jcc

#define re(index) reg_eflags(index)

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

jcc_helper(a0nbe, (!re(CF) && !re(ZF)))
jcc_helper(ae0nb0nc, (!re(CF)))
jcc_helper(b0c0nae, (re(CF)))
jcc_helper(be0na, (re(CF) || re(ZF)))
// jcc_helper(c, (re(CF)))
jcc_helper(e0z, (re(ZF)))
// ? 
jcc_helper(g0nle, (!re(ZF) || re(SF) == re(OF)) || (re(ZF) && re(SF) != re(OF)))
jcc_helper(ge0nl, (re(SF) == re(OF)))
jcc_helper(l0nge, (re(SF) != re(OF)))
jcc_helper(le0ng, (re(ZF) && re(SF) != re(OF)))
// jcc_helper(na, (re(CF)))
// jcc_helper(nae, (re(CF)))
// jcc_helper(nb, (!re(CF)))
// jcc_helper(nbe, (!re(CF) && !re(ZF)))
// jcc_helper(nc, (!re(CF)))
jcc_helper(ne0nz, (!re(ZF)))
// jcc_helper(ng, (re(ZF) || re(SF) != re(OF)))
// jcc_helper(nge, (re(SF) != re(OF)))
// jcc_helper(nl, (re(SF) == re(OF)))
// jcc_helper(nle, (re(ZF) && re(SF) != re(OF)))
jcc_helper(no, (!re(OF)))
jcc_helper(np0po, (!re(PF)))
jcc_helper(ns, (!re(SF)))
// jcc_helper(nz, (!re(ZF)))
jcc_helper(o, (re(OF)))
jcc_helper(p0pe, (re(PF)))
// jcc_helper(pe, (re(PF)))
// jcc_helper(po, (!re(PF)))
jcc_helper(s, (re(SF)))
// jcc_helper(z, (re(ZF)))

jcc_helper(ecxz, (!REG(R_ECX)))

#undef r
#include "cpu/exec/template-end.h"