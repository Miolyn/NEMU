#ifndef __CMOV_H__
#define __CMOV_H__
#include "macro.h"
#define ss(name) make_helper(concat3(cmov, name, _rm2r_v));
ss(b);
ss(e);
ss(ne);
ss(be);
ss(a);
ss(s);
ss(ns);
ss(l);
ss(le);
ss(g);
#undef ss
#endif