#include "cpu/exec/template-start.h"

#define instr cwd

#if DATA_BYTE != 1
make_helper(concat(cwd_, SUFFIX)){
    if(REG(R_EAX) >> ((DATA_BYTE << 3) - 1)){
        REG(R_EDX) = concat(f, DATA_BYTE);
    } else{
        REG(R_EDX) = 0;
    }
    return 1;
}
#endif



#include "cpu/exec/template-end.h"