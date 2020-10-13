#include "cpu/exec/template-start.h"
#include "device/port-io.h"
#define instr out

make_helper(concat(out_a2i_, SUFFIX)){
    ioaddr_t ioNo = instr_fetch(eip + 1, DATA_BYTE);
    pio_write(ioNo, DATA_BYTE, REG(R_EAX));
    return 1 + DATA_BYTE;
}

make_helper(concat(out_a2dx_,SUFFIX)){
    // assert(0);

    pio_write(reg_w(R_EDX), DATA_BYTE, REG(R_EAX));
    return 1;
}


#include "cpu/exec/template-end.h"