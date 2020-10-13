#include "cpu/exec/helper.h"
#define easy(instruction) make_helper_v(concat(instruction, _rm2r))

#define instr cmovb
#define CONDITION (cpu.CF)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
easy(instr);
#undef instr
#undef CONDITION


#define instr cmove
#define CONDITION (cpu.ZF)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
easy(instr);
#undef instr
#undef CONDITION


#define instr cmovne
#define CONDITION (!cpu.ZF)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
easy(instr);
#undef instr
#undef CONDITION

#define instr cmovbe
#define CONDITION (cpu.CF || cpu.ZF)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
easy(instr);
#undef instr
#undef CONDITION

#define instr cmova
#define CONDITION (!cpu.CF && !cpu.ZF)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
easy(instr);
#undef instr
#undef CONDITION

#define instr cmovs
#define CONDITION (cpu.SF)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
easy(instr);
#undef instr
#undef CONDITION


#define instr cmovns
#define CONDITION (!cpu.SF)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
easy(instr);
#undef instr
#undef CONDITION


#define instr cmovl
#define CONDITION (cpu.SF != cpu.OF)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
easy(instr);
#undef instr
#undef CONDITION


#define instr cmovle
#define CONDITION (cpu.ZF || cpu.SF != cpu.OF)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
easy(instr);
#undef instr
#undef CONDITION


#define instr cmovg
#define CONDITION (!cpu.ZF && cpu.SF == cpu.OF)
#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE
easy(instr);
#undef instr
#undef CONDITION

#undef easy