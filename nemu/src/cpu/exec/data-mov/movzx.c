#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "movzx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movzx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movzx-template.h"
#undef DATA_BYTE

make_helper_v(movzx_rmb2r)

make_helper(movzx_rmw2r_l){
    int len = decode_rm_w_internal(eip + 1, op_src, op_dest);
    uint32_t res = op_src->val;
    res &= 0xffff;
    write_operand_l(op_dest, res);
    print_asm_template2();
    return len + 1;
}