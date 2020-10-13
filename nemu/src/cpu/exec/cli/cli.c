#include "cpu/exec/helper.h"

make_helper(cli){
    cpu.IF = 0;
    print_asm("cli");
    return 1;
    
}