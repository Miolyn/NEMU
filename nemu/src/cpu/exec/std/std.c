#include "cpu/exec/helper.h"

make_helper(std){
    cpu.DF = 1;
    return 1;
}