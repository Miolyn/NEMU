#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE
#define jcc_hvmaker(prefix) make_helper_v(concat3(j, prefix, _rel))


jcc_hvmaker(a0nbe);
jcc_hvmaker(ae0nb0nc);
jcc_hvmaker(b0c0nae);
jcc_hvmaker(be0na);
// jcc_hvmaker(c);
jcc_hvmaker(e0z);
jcc_hvmaker(g0nle);
jcc_hvmaker(ge0nl);
jcc_hvmaker(l0nge);
jcc_hvmaker(le0ng);
// jcc_hvmaker(na);
// jcc_hvmaker(nae);
// jcc_hvmaker(nb);
// jcc_hvmaker(nbe);
// jcc_hvmaker(nc);
jcc_hvmaker(ne0nz);
// jcc_hvmaker(ng);
// jcc_hvmaker(nge);
// jcc_hvmaker(nl);
// jcc_hvmaker(nle);
jcc_hvmaker(no);
jcc_hvmaker(np0po);
jcc_hvmaker(ns);
// jcc_hvmaker(nz);
jcc_hvmaker(o);
jcc_hvmaker(p0pe);
// jcc_hvmaker(pe);
// jcc_hvmaker(po);
jcc_hvmaker(s);
// jcc_hvmaker(z);

jcc_hvmaker(ecxz);