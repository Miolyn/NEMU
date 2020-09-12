#include "cpu/exec/template-start.h"

#define instr movzx

#if DATA_BYTE != 1
make_helper(concat(movzx_rmb2r_, SUFFIX)){
    int len = decode_rm_b_internal(eip + 1, op_src, op_dest);
    uint32_t res = op_src->val;
    concat(write_operand_, SUFFIX)(op_dest, res);
    print_asm_template2();
    return len + 1;
}
#endif

#include "cpu/exec/template-end.h"