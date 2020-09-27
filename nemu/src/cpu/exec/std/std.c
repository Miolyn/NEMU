#include "cpu/exec/helper.h"

make_helper(std){
    cpu.DF = 1;
    return 1;
}

make_helper(cld){
    cpu.DF = 0;
    return 1;
}