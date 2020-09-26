#ifndef __HELPER_H__
#define __HELPER_H__

#include "nemu.h"
#include "cpu/decode/operand.h"

/* All function defined with 'make_helper' return the length of the operation. */
#define make_helper(name) int name(swaddr_t eip)
#define void_helper(name) void name(uint32_t src)
#define void_op_helper(name) void name(Operand *op)

static inline uint32_t instr_fetch(swaddr_t addr, size_t len) {
	return swaddr_read(addr, len, R_CS);
}

/* Instruction Decode and EXecute */
static inline int idex(swaddr_t eip, int (*decode)(swaddr_t), void (*execute) (void)) {
	/* eip is pointing to the opcode */
	int len = decode(eip + 1);
	execute();
	return len + 1;	// "1" for opcode
}



/* shared by all helper function */
extern Operands ops_decoded;

#define op_src (&ops_decoded.src)
#define op_src2 (&ops_decoded.src2)
#define op_dest (&ops_decoded.dest)


#endif
