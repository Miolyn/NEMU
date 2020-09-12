#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "movsx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movsx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movsx-template.h"
#undef DATA_BYTE

make_helper_v(movsx_rmb2r)

make_helper(movsx_rmw2r_l){
    int len = decode_rm_w_internal(eip + 1, op_src, op_dest);
    uint32_t res = op_src->val;
    if (sign_bit16(res)){
        res |= 0xFFFF0000;
    }
    write_operand_l(op_dest, res);
    print_asm_template2();
    return len + 1;
}