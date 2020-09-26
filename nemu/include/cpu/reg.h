#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../lib-common/x86-inc/cpu.h"
enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { CF, POS1, PF, POS3, AF, POS5, ZF, SF, TF, IF, DF, OF, OL, IP, NT, POS15, RF, VM };
enum { R_ES, R_CS, R_SS, R_DS, R_FS, R_GS, };
/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef union{
	struct{
		uint16_t index :13;
		uint16_t ti : 2;
		uint16_t rpl :1;
	};
	uint32_t val;
}Selector;

typedef struct {
	Selector selector;
	union{
		struct{
			// the base addr of the segment
			uint32_t base_addr0 : 16;
			uint32_t base_addr1 : 8;
			uint32_t base_addr2 : 8;
		};
		uint32_t base_addr;
	};
	union{
		struct{
			// max offset of the segment
			uint32_t seg_limit0 : 16;
			uint32_t seg_limit1 : 4;
			uint32_t seg_limit2 : 12;
		};
		uint32_t seg_limit;
	};
	struct{
		// some attribute of the segment
		uint16_t attr;
	};
} SegReg;



typedef struct {
	union {
        union {
		    uint32_t _32;
		    uint16_t _16;
		    uint8_t _8[2];
	    } gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */
        struct {
	        uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
        };
    };
	// swaddr_t eip;
	uint32_t eip;

	union {
		uint32_t ef;
		struct {
			uint32_t CF : 1;
			uint32_t POS1 : 1;
			uint32_t PF : 1;
			uint32_t POS3 : 1;
			uint32_t AF : 1;
			uint32_t POS5 : 1;
			uint32_t ZF : 1;
			uint32_t SF : 1;
			uint32_t TF : 1;
			uint32_t IF : 1;
			uint32_t DF : 1;
			uint32_t OF : 1;
			uint32_t OL : 1;
			uint32_t IP : 1;
			uint32_t NT : 1;
			uint32_t POS15 : 1;
			uint32_t RF : 1;
			uint32_t VM : 1;
			uint32_t no : 14;
		};
	};

	struct GDTR{
		uint32_t base_addr;
		uint16_t table_limit;
	} gdtr;

	CR0 cr0;
	CR3 cr3;

	union {
		SegReg sRegs[6];
		struct{
			SegReg es, cs, ds, ss, fs, gs;
		};
	};
	

} CPU_state;


extern CPU_state cpu;

typedef struct{
	union{
		uint32_t dword0;
		struct{
			uint32_t seg_limit0 : 16;
			uint32_t seg_base0 : 16;
		};
	};
	union{
		uint32_t dword1;
		struct{
			uint32_t seg_base1 : 8;
			uint32_t type :5;
			uint32_t dpl :2;
			uint32_t p :1;

			uint32_t seg_limit1 : 4;
			uint32_t AVL :1;
			uint32_t ZERO :1;
			uint32_t B :1;
			// depending on the setting of the granularity bit:
			// 1 In units of one byte, to define a limit of up to 1 megabyte. (DWORD)Limit & 0X000FFFFF
			// 2 In units of 4 Kilobytes, to define a limit of up to 4 gigabytes.¡¢ ((DWORD)Limit << 12) | 0XFFFFFFFF£©
			//  The limit is shifted left by 12 bits when loaded, and low-order one-bits are inserted.
			uint32_t G :1;
			uint32_t seg_base2 : 8;
		};
	};
	
}Descriptor;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])
#define reg_eip cpu.eip
// #define reg_eflags(pos) cpu.eflags[pos]._1
// #define set_eflags(pos) cpu.eflags[pos]._1 = 1
// #define reset_eflags(pos) cpu.eflags[pos]._1 = 0
#define f4 0xFFFFFFFF
#define f2 0xFFFF
#define f1 0xFF
#define sign_bit32(res) ((res >> 31) & 1)
#define sign_bit16(res) ((res >> 15) & 1)
#define sign_bit8(res) ((res >> 7) & 1)
#define low8(res) (res & 0xFF)
extern void reset_all_eflags();
extern int carry_flag(int dest, int src);
extern int carry_flag_sub(int dest, int src);
extern void parity_flag(int res);
extern void adjust_flag(int dest, int src);
extern void zero_flag(int res);
extern void sign_flag(int res);
extern void overflow_flag(int dest, int src);

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* regef[];
extern uint32_t cSreg;
extern uint32_t get_reg_by_str(bool *success, char *e);
extern void load_descriptor(uint8_t sReg);
extern lnaddr_t seg_translate(swaddr_t addr, uint32_t len, uint32_t sReg);
#define cf_sub(dest, src) cpu.CF = dest < src
#define cf_add(dest, src) cpu.CF = ((dest + src) < dest)
#define sf_add(dest, src) cpu.SF = ((dest + src) < 0)
#define sf_sub(dest, src) cpu.SF = ((dest - src) < 0)
#define of_add(dest, src) cpu.OF = (sign_bit32(dest) == sign_bit32(src) && sign_bit32(dest) != cpu.SF) ;
#define of_sub(dest, src) cpu.OF= (sign_bit32(dest) != sign_bit32(src) && sign_bit32(src) == cpu.SF) ;
#define zf_add(dest, src) cpu.ZF = ((dest + src) == 0)
#define zf_sub(dest, src) cpu.ZF = ((dest - src) == 0)
#define pf_add(dest, src) int NEVER_USE = (dest + src); NEVER_USE ^= NEVER_USE >>4; NEVER_USE ^= NEVER_USE >>2; NEVER_USE ^= NEVER_USE >>1; cpu.PF=!(NEVER_USE & 1);
#define pf_sub(dest, src) int NEVER_USE = (dest - src); NEVER_USE ^= NEVER_USE >>4; NEVER_USE ^= NEVER_USE >>2; NEVER_USE ^= NEVER_USE >>1; cpu.PF=!(NEVER_USE & 1);
#define eadd(dest, src) cf_add(dest, src); sf_add(dest, src); of_add(dest, src); zf_add(dest, src); pf_add(dest, src);
#define esub(dest, src) cf_sub(dest, src); sf_sub(dest, src); of_sub(dest, src); zf_sub(dest, src); pf_sub(dest, src);

#endif
