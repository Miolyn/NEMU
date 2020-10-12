#include "cpu/exec/helper.h"

#if DATA_BYTE == 1

#define SUFFIX b
#define DATA_TYPE uint8_t
#define DATA_TYPE_S int8_t

#elif DATA_BYTE == 2

#define SUFFIX w
#define DATA_TYPE uint16_t
#define DATA_TYPE_S int16_t

#elif DATA_BYTE == 4

#define SUFFIX l
#define DATA_TYPE uint32_t
#define DATA_TYPE_S int32_t

#else

#error unknown DATA_BYTE

#endif

#define REG(index) concat(reg_, SUFFIX) (index)
#define REG_NAME(index) concat(regs, SUFFIX) [index]
#define W_REG(name, src) concat(write_, reg) (name) (src)
#define PUSH_STACK(val) concat(push_stack_, SUFFIX) (val)
#define POP_STACK(op_dest) concat(pop_stack_, SUFFIX) (op_dest)
#define POP() concat(pop_, SUFFIX)(cpu.eip)
// #define MEM_R(addr) swaddr_read(addr, DATA_BYTE)
#define MEM_R(addr, sReg) swaddr_read(addr, DATA_BYTE, sReg)
// #define MEM_W(addr, data) swaddr_write(addr, DATA_BYTE, data)
#define MEM_W(addr, data, sReg) swaddr_write(addr, DATA_BYTE, data, sReg)
#define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)

#define MSB(n) ((DATA_TYPE)(n) >> ((DATA_BYTE << 3) - 1))

