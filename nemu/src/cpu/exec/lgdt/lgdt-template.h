#include "cpu/exec/template-start.h"

#define instr lgdt

#if DATA_BYTE != 1
make_helper(concat(lgdt_m_, SUFFIX)){
    int len = concat3(decode_rm_, SUFFIX, _internal)(eip + 1, op_src, op_src2);
    uint16_t limit = lnaddr_read(op_src->addr, 2);
    uint32_t base = 0;
    if(ops_decoded.is_operand_size_16){
        base = lnaddr_read(op_src->addr + 2, 3);
    } else{
        base = lnaddr_read(op_src->addr + 2, 4);
    }
    cpu.gdtr.base_addr = base;
    // printf("seg table base:%x\n", base);
    cpu.gdtr.table_limit = limit;
    print_asm(str(instr) str(SUFFIX) " %x", op_src->addr);
    return len + 1;
}
#endif

#include "cpu/exec/template-end.h"