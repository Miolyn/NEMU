#include "cpu/exec/helper.h"
#define re(index) reg_eflags(index)


setcc_helper(a0nbe, (!re(CF) && !re(ZF)))
setcc_helper(ae0nb0nc, (!re(CF)))
setcc_helper(b0c0nae, (re(CF)))
setcc_helper(be0na, (re(CF) || re(ZF)))
// setcc_helper(c, (re(CF)))
setcc_helper(e0z, (re(ZF)))
// ? 
setcc_helper(g0nle, (!re(ZF) && re(SF) == re(OF)))
setcc_helper(ge0nl, (re(SF) == re(OF)))
setcc_helper(l0nge, (re(SF) != re(OF)))
setcc_helper(le0ng, (re(ZF) && re(SF) != re(OF)))
// setcc_helper(na, (re(CF)))
// setcc_helper(nae, (re(CF)))
// setcc_helper(nb, (!re(CF)))
// setcc_helper(nbe, (!re(CF) && !re(ZF)))
// setcc_helper(nc, (!re(CF)))
setcc_helper(ne0nz, (!re(ZF)))
// setcc_helper(ng, (re(ZF) || re(SF) != re(OF)))
// setcc_helper(nge, (re(SF) != re(OF)))
// setcc_helper(nl, (re(SF) == re(OF)))
// setcc_helper(nle, (re(ZF) && re(SF) != re(OF)))
setcc_helper(no, (!re(OF)))
setcc_helper(np0po, (!re(PF)))
setcc_helper(ns, (!re(SF)))
// setcc_helper(nz, (!re(ZF)))
setcc_helper(o, (re(OF)))
setcc_helper(p0pe, (re(PF)))
// setcc_helper(pe, (re(PF)))
// setcc_helper(po, (!re(PF)))
setcc_helper(s, (re(SF)))
// setcc_helper(z, (re(ZF)))

#undef re