#include "cpu/exec/template-start.h"

#define instr lea

#if DATA_BYTE != 1
static void do_execute(){
    printf("src:%x,dest:%x\n", op_src->addr, op_dest->reg);
    assert(0);
    // if(ops_decoded.is_operand_size_16){
    //     reg_w(op_dest->reg) = op_src->addr & 0xFFFF;
    // } else{
    //     reg_l(op_src->reg) = op_src->addr;
    // }
}
make_instr_helper(rm2r)
#endif


#include "cpu/exec/template-end.h"