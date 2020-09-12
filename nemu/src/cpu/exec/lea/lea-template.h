#include "cpu/exec/template-start.h"

#define instr lea

#if DATA_BYTE != 1
static void do_execute(){
    if(ops_decoded.is_operand_size_16){
        reg_w(op_dest->reg) = op_src->addr & 0xFFFF;
    } else{
        if(op_src->size != 4){
            reg_l(op_dest->reg) = op_src->addr & 0xFFFF;
        } else{
            reg_l(op_dest->reg) = op_src->addr;
        }
        
    }
    print_asm_template2();
}
make_instr_helper(rm2r)
#endif


#include "cpu/exec/template-end.h"