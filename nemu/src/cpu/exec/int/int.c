#include "cpu/exec/helper.h"

make_helper(int_i_b){
    push_stack_l(cpu.ef);
    cpu.IF = 0;
    cpu.TF = 0;
    return 1;
}