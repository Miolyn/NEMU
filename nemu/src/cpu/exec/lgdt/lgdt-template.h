#include "cpu/exec/template-start.h"

#define instr lgdt

#if DATA_BYTE != 1
make_helper(concat(lgdt_m_, SUFFIX)){
    int len = concat3(decode_rm_, SUFFIX, _internal)(eip, op_src, op_src2);
    return 0;
}
#endif

#include "cpu/exec/template-end.h"