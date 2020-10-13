#include "cpu/exec/helper.h"

#define instr cmove
#define CONDITION (cpu.ZF == 1)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
#undef instr
#undef CONDITION

