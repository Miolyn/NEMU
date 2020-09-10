#ifndef __JCC_H__
#define __JCC_H__
#define jcc_hmake(prefix) make_helper(concat3(j, prefix, _rel_b)); \
    make_helper(concat3(j, prefix, _rel_w)); \
    make_helper(concat3(j, prefix, _rel_l)); \
    make_helper(concat3(j, prefix, _rel_v));
// make_helper(je_rel_b);
// make_helper(je_rel_w);
// make_helper(je_rel_l);

// make_helper(je_rel_v);
jcc_hmake(e0z)

#endif