#include "cpu/exec/helper.h"

make_helper(leave){

    int bt = ops_decoded.is_operand_size_16 ? 2 : 4;
    int i;
    for(i = reg_l(R_ESP); i <= reg_l(R_EBP); i += bt){
        swaddr_write(i, bt, 0);
    }
    reg_l(R_ESP) = reg_l(R_EBP);
    if(ops_decoded.is_operand_size_16){
        reg_w(R_EBP) = swaddr_read(reg_l(R_ESP), 2);
        reg_l(R_ESP) += 2;
    } else{
        reg_l(R_EBP) = swaddr_read(reg_l(R_ESP), 4);
        reg_l(R_ESP) += 4;
    }
    print_asm("leave");
    return 1;
}