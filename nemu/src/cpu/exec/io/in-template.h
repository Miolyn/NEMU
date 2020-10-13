#include "cpu/exec/template-start.h"
#include "device/port-io.h"
#define instr in

make_helper(concat(in_i2a_, SUFFIX)){
    assert(0);
    ioaddr_t ioNo = instr_fetch(eip + 1, 1);
    REG(R_EAX) = pio_read(ioNo, DATA_BYTE);
    return 2;
}

make_helper(concat(in_dx2a_,SUFFIX)){
    ioaddr_t ioNo = reg_w(R_EDX);
    assert(0);
    REG(R_EAX) = pio_read(ioNo, DATA_BYTE);
    return 1;
}


#include "cpu/exec/template-end.h"