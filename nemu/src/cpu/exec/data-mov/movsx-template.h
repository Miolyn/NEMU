#include "cpu/exec/template-start.h"

#define instr movsx

#if DATA_BYTE != 1
make_helper(concat(movsx_rmb2r_, SUFFIX)){
    uint32_t len = decode_rm_b_internal(eip + 1, op_src, op_dest);
    uint32_t res = op_src->val;
    if (sign_bit8(res)){
        res |= 0xFFFFFF00;
    }
#if DATA_BYTE == 2
    res &= 0xffff;
#endif
    // uint32_t st = 0xFFFF;
    // if(DATA_BYTE != 4) res &= st;
    concat(write_operand_, SUFFIX)(op_dest, res);
    print_asm_template2();
    return len + 1;
}
#endif

#include "cpu/exec/template-end.h"