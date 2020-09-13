#include "cpu/exec/template-start.h"

#include "cpu/decode/modrm.h"

#define decode_r_internal concat3(decode_r_, SUFFIX, _internal)
#define decode_rm_internal concat3(decode_rm_, SUFFIX, _internal)
#define decode_i concat(decode_i_, SUFFIX)
#define decode_si concat(decode_si_, SUFFIX)
#define decode_a concat(decode_a_, SUFFIX)
#define decode_r2rm concat(decode_r2rm_, SUFFIX)

/* Ib, Iv */
make_helper(concat(decode_i_, SUFFIX)) {
	/* eip here is pointing to the immediate */
	op_src->type = OP_TYPE_IMM;
	op_src->imm = instr_fetch(eip, DATA_BYTE);
	op_src->val = op_src->imm;

#ifdef DEBUG
	snprintf(op_src->str, OP_STR_SIZE, "$0x%x", op_src->imm);
#endif
	return DATA_BYTE;
}


/* sign immediate */
make_helper(concat(decode_si_, SUFFIX)) {
	op_src->type = OP_TYPE_IMM;

	/* TODO: Use instr_fetch() to read `DATA_BYTE' bytes of memory pointed
	 * by `eip'. Interpret the result as an signed immediate, and assign
	 * it to op_src->simm.
	 *
	op_src->simm = ???
	 */
	op_src->simm = instr_fetch(eip, DATA_BYTE);
	// printf("res %x\n", (op_src->simm >> (DATA_BYTE * 8 - 1)));
	if (DATA_BYTE != 4 && (op_src->simm >> (DATA_BYTE * 8 - 1)) & 1){
		int st = 0xFFFF0000;
		if (DATA_BYTE == 1) st = 0xFFFFFF00;
		op_src->simm |= st;
	}
	// panic("please implement me");

	op_src->val = op_src->simm;

#ifdef DEBUG
	snprintf(op_src->str, OP_STR_SIZE, "$0x%x", op_src->val);
#endif
	return DATA_BYTE;
}


/* eAX */
static int concat(decode_a_, SUFFIX) (swaddr_t eip, Operand *op) {
	op->type = OP_TYPE_REG;
	op->reg = R_EAX;
	op->val = REG(R_EAX);

#ifdef DEBUG
	snprintf(op->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EAX));
#endif
	return 0;
}

/* eax and imm*/
make_helper(concat(decode_ai_, SUFFIX)){
	concat(decode_a_, SUFFIX)(eip, op_dest);
	/* eip here is pointing to the immediate */
	op_src->type = OP_TYPE_IMM;
	op_src->imm = instr_fetch(eip, DATA_BYTE);
	op_src->val = op_src->imm;

#ifdef DEBUG
	snprintf(op_src->str, OP_STR_SIZE, "$0x%x", op_src->imm);
#endif
	return DATA_BYTE;
}

/* eXX: eAX, eCX, eDX, eBX, eSP, eBP, eSI, eDI */
static int concat3(decode_r_, SUFFIX, _internal) (swaddr_t eip, Operand *op) {
	op->type = OP_TYPE_REG;
	op->reg = ops_decoded.opcode & 0x7;
	op->val = REG(op->reg);

#ifdef DEBUG
	snprintf(op->str, OP_STR_SIZE, "%%%s", REG_NAME(op->reg));
#endif
	return 0;
}

int concat3(decode_rm_, SUFFIX, _internal) (swaddr_t eip, Operand *rm, Operand *reg) {
	rm->size = DATA_BYTE;
	int len = read_ModR_M(eip, rm, reg);
	reg->val = REG(reg->reg);

#ifdef DEBUG
	snprintf(reg->str, OP_STR_SIZE, "%%%s", REG_NAME(reg->reg));
#endif
	return len;
}

/* Eb <- Gb
 * Ev <- Gv
 */
make_helper(concat(decode_r2rm_, SUFFIX)) {
	return decode_rm_internal(eip, op_dest, op_src);
}

/* Gb <- Eb
 * Gv <- Ev
 */
make_helper(concat(decode_rm2r_, SUFFIX)) {
	return decode_rm_internal(eip, op_src, op_dest);
}


/* AL <- Ib
 * eAX <- Iv
 */
make_helper(concat(decode_i2a_, SUFFIX)) {
	decode_a(eip, op_dest);
	return decode_i(eip);
}

/* AL <- Ib
 * eAX <- Iv
 */
make_helper(concat(decode_si2a_, SUFFIX)) {
	decode_a(eip, op_dest);
	return decode_si(eip);
}

/* Gv <- EvIb
 * Gv <- EvIv
 * use for imul */
make_helper(concat(decode_i_rm2r_, SUFFIX)) {
	int len = decode_rm_internal(eip, op_src2, op_dest);
	len += decode_i(eip + len);
	return len;
}

/* Eb <- Ib
 * Ev <- Iv
 */
make_helper(concat(decode_i2rm_, SUFFIX)) {
	int len = decode_rm_internal(eip, op_dest, op_src2);		/* op_src2 not use here */
	len += decode_i(eip + len);
	return len;
}

#if DATA_BYTE != 1
make_helper(concat(decode_sib2rm_, SUFFIX)){
	int len = decode_rm_internal(eip, op_dest, op_src2);		/* op_src2 not use here */
	len += decode_si_b(eip + len);
	return len;
}
#endif

/* Eb <- Ib
 * Ev <- Iv
 */
// make_helper(concat(decode_si2rm_, SUFFIX)) {
// 	int len = decode_rm_internal(eip, op_dest, op_src2);		/* op_src2 not use here */
// 	len += decode_si(eip + len);
// 	return len;
// }
/* XX <- Ib 
 * eXX <- Iv 
 */
make_helper(concat(decode_i2r_, SUFFIX)) {
	decode_r_internal(eip, op_dest);
	return decode_i(eip);
}

/* XX <- Ib 
 * eXX <- Iv 
 */
make_helper(concat(decode_si2r_, SUFFIX)) {
	decode_r_internal(eip, op_dest);
	return decode_si(eip);
}

/* used by unary operations */
make_helper(concat(decode_rm_, SUFFIX)) {
	return decode_rm_internal(eip, op_src, op_src2);		/* op_src2 not use here */
}

make_helper(concat(decode_r_, SUFFIX)) {
	return decode_r_internal(eip, op_src);
}


make_helper(concat(decode_m_, SUFFIX)) {

	op_src->type = OP_TYPE_MEM;
	op_src->addr = instr_fetch(eip, DATA_BYTE);
	op_src->val = op_src->addr;
	op_src->size = DATA_BYTE;
	return DATA_BYTE;
}


// #if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(decode_si2rm_, SUFFIX)) {
	int len = decode_rm_internal(eip, op_dest, op_src2);	/* op_src2 not use here */
	len += decode_si_b(eip + len);
	return len;
}

make_helper(concat(decode_si_rm2r_, SUFFIX)) {
	int len = decode_rm_internal(eip, op_src2, op_dest);
	len += decode_si_b(eip + len);
	return len;
}
// #endif

/* used by shift instructions */
make_helper(concat(decode_rm_1_, SUFFIX)) {
	int len = decode_r2rm(eip);
	op_src->type = OP_TYPE_IMM;
	op_src->imm = 1;
	op_src->val = 1;
#ifdef DEBUG
	sprintf(op_src->str, "$1");
#endif
	return len;
}

make_helper(concat(decode_rm_cl_, SUFFIX)) {
	int len = decode_r2rm(eip);
	op_src->type = OP_TYPE_REG;
	op_src->reg = R_CL;
	op_src->val = reg_b(R_CL);
#ifdef DEBUG
	sprintf(op_src->str, "%%cl");
#endif
	return len;
}

make_helper(concat(decode_rm_imm_, SUFFIX)) {
	int len = decode_r2rm(eip);
	len += decode_i_b(eip + len);
	return len;
}

void concat(write_operand_, SUFFIX) (Operand *op, DATA_TYPE src) {
	if(op->type == OP_TYPE_REG) { REG(op->reg) = src; }
	else if(op->type == OP_TYPE_MEM) { swaddr_write(op->addr, op->size, src); }
	else { assert(0); }
}



void_helper(concat(push_stack_, SUFFIX)){
	// printf("push stack at rsp:%x, data:%x\n", reg_l(R_ESP), src);
	reg_l(R_ESP) = reg_l(R_ESP) - DATA_BYTE;
	swaddr_write(reg_l(R_ESP), DATA_BYTE, src);
	// printf("after %x, data:%x \n", reg_l(R_ESP), swaddr_read(reg_l(R_ESP), 4));
}

void_op_helper(concat(pop_stack_, SUFFIX)) {
	// printf("pop stack op read %x\n", swaddr_read(REG(R_SP), DATA_BYTE));
	concat(write_operand_, SUFFIX)(op, swaddr_read(reg_l(R_ESP), DATA_BYTE));
	reg_l(R_ESP) = reg_l(R_ESP) + DATA_BYTE;
}


#include "cpu/exec/template-end.h"
