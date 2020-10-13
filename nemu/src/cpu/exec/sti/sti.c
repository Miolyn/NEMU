#include "cpu/exec/helper.h"

make_helper(sti){
    cpu.IF = 1;
    return 1;
}