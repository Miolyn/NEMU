#include "cpu/exec/helper.h"

make_helper(hlt){
    print_asm("hlt");
    if(!cpu.INTR){
        return 0;
    }
    return 1;
}

