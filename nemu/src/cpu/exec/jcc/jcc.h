#ifndef __JCC_H__
#define __JCC_H__
#define jcc_hmaker(prefix) make_helper(concat3(j, prefix, _rel_b)); \
    make_helper(concat3(j, prefix, _rel_w)); \
    make_helper(concat3(j, prefix, _rel_l)); \
    make_helper(concat3(j, prefix, _rel_v));

jcc_hmaker(a0nbe);
jcc_hmaker(ae0nb0nc);
jcc_hmaker(b0c0nae);
jcc_hmaker(be0na);
// jcc_hmaker(c);
jcc_hmaker(e0z);
jcc_hmaker(g0nle);
jcc_hmaker(ge0nl);
jcc_hmaker(l0nge);
jcc_hmaker(le0ng);
// jcc_hmaker(na);
// jcc_hmaker(nae);
// jcc_hmaker(nb);
// jcc_hmaker(nbe);
// jcc_hmaker(nc);
jcc_hmaker(ne0nz);
// jcc_hmaker(ng);
// jcc_hmaker(nge);
// jcc_hmaker(nl);
// jcc_hmaker(nle);
jcc_hmaker(no);
jcc_hmaker(np0po);
jcc_hmaker(ns);
// jcc_hmaker(nz);
jcc_hmaker(o);
jcc_hmaker(p0pe);
// jcc_hmaker(pe);
// jcc_hmaker(po);
jcc_hmaker(s);
// jcc_hmaker(z);

jcc_hmaker(ecxz);

#endif