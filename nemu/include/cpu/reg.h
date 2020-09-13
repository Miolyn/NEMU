#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { CF, POS1, PF, POS3, AF, POS5, ZF, SF, TF, IF, DF, OF, OL, IP, NT, POS15, RF, VM };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

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
	swaddr_t eip;

	union {
		uint32_t ef : 18;
		struct {
			uint32_t _1: 1;
		}eflags[18];
		
		struct {
			uint32_t CF : 1, POS1 : 1, PF : 1, POS3 : 1, AF : 1, POS5 : 1, ZF : 1, SF : 1, TF : 1, IF : 1, DF : 1, OF : 1, OL : 1, IP : 1, NT : 1, POS15 : 1, RF : 1, VM : 1;
		};
	};
	
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])
#define reg_eip (cpu.eip)
#define reg_eflags(pos) cpu.eflags[pos]._1
#define set_eflags(pos) cpu.eflags[pos]._1 = 1
#define reset_eflags(pos) cpu.eflags[pos]._1 = 0
#define f4 0xFFFFFFFF
#define f2 0xFFFF
#define f1 0xFF
#define sign_bit32(res) (res >> 31)
#define sign_bit16(res) (res >> 15)
#define sign_bit8(res) (res >> 7)
#define low8(res) (res & 0xFF)
extern int carry_flag(int dest, int src);
extern int carry_flag3(int dest, int src);
extern void parity_flag(int res);
extern void adjust_flag(int dest, int src);
extern void adjust_flag3(int dest, int src);
extern void zero_flag(int res);
extern void sign_flag(int res);
extern int overflow_flag(int dest, int src);
extern int overflow_flag3(int dest, int src);
extern int oszapc(uint32_t dest, uint32_t src, uint32_t width);


extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

extern uint32_t get_reg_by_str(bool *success, char *e);
#endif
