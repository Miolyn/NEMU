#include "cpu/helper.h"
#include "cpu/decode/modrm.h"

#include "all-instr.h"

// function pointer
typedef int (*helper_fun)(swaddr_t);
// #define make_helper(name) int name(swaddr_t eip)
// make_helper to make a funciton
static make_helper(_2byte_esc);

#define make_group(name, item0, item1, item2, item3, item4, item5, item6, item7) \
	static helper_fun concat(opcode_table_, name) [8] = { \
	/* 0x00 */	item0, item1, item2, item3, \
	/* 0x04 */	item4, item5, item6, item7  \
	}; \
	static make_helper(name) { \
		ModR_M m; \
		m.val = instr_fetch(eip + 1, 1); \
		return concat(opcode_table_, name) [m.opcode](eip); \
	}
	
/* 0x80 */
make_group(group1_b,
	add_i2rm_b, or_i2rm_b, adc_i2rm_b, sbb_i2rm_b, 
	and_i2rm_b, sub_i2rm_b, xor_i2rm_b, cmp_i2rm_b)

/* 0x81 */
make_group(group1_v,
	add_i2rm_v, or_i2rm_v, adc_i2rm_v, sbb_i2rm_v, 
	and_i2rm_v, sub_i2rm_v, xor_i2rm_v, cmp_i2rm_v)

/* 0x83 */
make_group(group1_sx_v,
	add_sib2rm_v, or_sib2rm_v, adc_sib2rm_v, sbb_sib2rm_v, 
	and_sib2rm_v, sub_sib2rm_v, xor_sib2rm_v, cmp_sib2rm_v)



/* 0xc0 */
make_group(group2_i_b,
	inv, inv, inv, inv, 
	shl_rm_imm_b, shr_rm_imm_b, inv, sar_rm_imm_b)

/* 0xc1 */
make_group(group2_i_v,
	inv, inv, inv, inv, 
	shl_rm_imm_v, shr_rm_imm_v, inv, sar_rm_imm_v)

/* 0xd0 */
make_group(group2_1_b,
	inv, inv, inv, inv, 
	shl_rm_1_b, shr_rm_1_b, inv, sar_rm_1_b)

/* 0xd1 */
make_group(group2_1_v,
	inv, inv, inv, inv, 
	shl_rm_1_v, shr_rm_1_v, inv, sar_rm_1_v)

/* 0xd2 */
make_group(group2_cl_b,
	inv, inv, inv, inv, 
	shl_rm_cl_b, shr_rm_cl_b, inv, sar_rm_cl_b)

/* 0xd3 */
make_group(group2_cl_v,
	inv, inv, inv, inv, 
	shl_rm_cl_v, shr_rm_cl_v, inv, sar_rm_cl_v)

/* 0xf6 */
make_group(group3_b,
	test_i2rm_b, inv, not_rm_b, neg_rm_v, 
	mul_rm_b, imul_rm2a_b, div_rm_b, idiv_rm_b)

/* 0xf7 */
make_group(group3_v,
	test_i2rm_v, inv, not_rm_v, neg_rm_v, 
	mul_rm_v, imul_rm2a_v, div_rm_v, idiv_rm_v)

/* 0xfe */
make_group(group4,
	inc_rm_v, dec_rm_b, inv, inv, 
	inv, inv, inv, inv)

/* 0xff */
make_group(group5,
	inc_rm_v, dec_rm_v, call_rm_v, inv, 
	jmp_rm_v, inv, push_rm_v, inv)

make_group(group6,
	inv, inv, inv, inv, 
	inv, inv, inv, inv)

make_group(group7,
	inv, inv, inv, inv, 
	inv, inv, inv, inv)


/* TODO: Add more instructions!!! */

// 2^8 one byte

helper_fun opcode_table [256] = {
/* 0x00 */	add_r2rm_b, add_r2rm_v, add_rm2r_b, add_rm2r_v,
/* 0x04 */	add_i2a_b, add_i2a_v, inv, inv,
/* 0x08 */	or_r2rm_b, or_r2rm_v, or_rm2r_b, or_rm2r_v,
/* 0x0c */	or_i2a_b, or_i2a_v, inv, _2byte_esc,
/* 0x10 */	adc_r2rm_b, adc_r2rm_v, inv, inv,
/* 0x14 */	adc_i2a_b, adc_i2a_v, adc_rm2r_b, adc_rm2r_v,
/* 0x18 */	sbb_r2rm_b, sbb_r2rm_v, sbb_rm2r_b, sbb_rm2r_v,
/* 0x1c */	sbb_i2a_b, sbb_i2a_v, inv, inv,
/* 0x20 */	and_r2rm_b, and_r2rm_v, and_rm2r_b, and_rm2r_v,
/* 0x24 */	and_i2a_b, and_i2a_v, inv, inv,
/* 0x28 */	sub_r2rm_b, sub_r2rm_v, sub_rm2r_b, sub_rm2r_v,
/* 0x2c */	sub_i2a_b, sub_i2a_v, inv, inv,
/* 0x30 */	xor_r2rm_b, xor_r2rm_v, xor_rm2r_b, xor_rm2r_v,
/* 0x34 */	xor_i2a_b, xor_i2a_v, inv, inv,
/* 0x38 */	cmp_r2rm_b, cmp_r2rm_v, cmp_rm2r_b, cmp_rm2r_v,
/* 0x3c */	cmp_i2a_b, cmp_i2a_v, inv, inv,
/* 0x40 */	inc_r_v, inc_r_v, inc_r_v, inc_r_v,
/* 0x44 */	inc_r_v, inc_r_v, inc_r_v, inc_r_v,
/* 0x48 */	dec_r_v, dec_r_v, dec_r_v, dec_r_v,
/* 0x4c */	dec_r_v, dec_r_v, dec_r_v, dec_r_v,
/* 0x50 */	push_r_v, push_r_v, push_r_v, push_r_v,
/* 0x54 */	push_r_v, push_r_v, push_r_v, push_r_v,
/* 0x58 */	pop_r_v, pop_r_v, pop_r_v, pop_r_v,
/* 0x5c */	pop_r_v, pop_r_v, pop_r_v, pop_r_v,
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, operand_size, inv,
/* 0x68 */	push_i_v, imul_i_rm2r_v, push_i_b, imul_si_rm2r_v,
/* 0x6c */	inv, inv, inv, inv,
/* 0x70 */	jo_rel_b, jno_rel_b, jb0c0nae_rel_b, jae0nb0nc_rel_b,
/* 0x74 */	je0z_rel_b, jne0nz_rel_b, jbe0na_rel_b, ja0nbe_rel_b,
/* 0x78 */	js_rel_b, jns_rel_b, jp0pe_rel_b, jnp0po_rel_b,
/* 0x7c */	jl0nge_rel_b, jge0nl_rel_b, jle0ng_rel_b, jg0nle_rel_b,
/* 0x80 */	group1_b, group1_v, inv, group1_sx_v, 
/* 0x84 */	test_r2rm_b, test_r2rm_v, inv, inv,
/* 0x88 */	mov_r2rm_b, mov_r2rm_v, mov_rm2r_b, mov_rm2r_v,
/* 0x8c */	inv, lea, inv, pop_rm_v,
/* 0x90 */	inv, inv, inv, inv,
/* 0x94 */	inv, inv, inv, inv,
/* 0x98 */	inv, cwd_v, inv, inv,
/* 0x9c */	inv, inv, inv, inv,
/* 0xa0 */	mov_moffs2a_b, mov_moffs2a_v, mov_a2moffs_b, mov_a2moffs_v,
/* 0xa4 */	movs_m_b, movs_m_v, inv, inv,
/* 0xa8 */	test_i2a_b, test_i2a_v, stos_m_b, stos_m_v,
/* 0xac */	lods_m_b, lods_m_v, scas_m_b, scas_m_v,
/* 0xb0 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb4 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb8 */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xbc */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xc0 */	group2_i_b, group2_i_v, ret_i_w, ret_rel_v,
/* 0xc4 */	inv, inv, mov_i2rm_b, mov_i2rm_v,
/* 0xc8 */	inv, inv, inv, inv,
/* 0xcc */	int3, inv, inv, inv,
/* 0xd0 */	group2_1_b, group2_1_v, group2_cl_b, group2_cl_v,
/* 0xd4 */	inv, inv, nemu_trap, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, jecxz_rel_b,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	call_rel_v, jmp_rel_v, inv, jmp_rel_b,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, repnz, rep,
/* 0xf4 */	inv, inv, group3_b, group3_v,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, group4, group5
};

helper_fun _2byte_opcode_table [256] = {
/* 0x00 */	group6, group7, inv, inv, 
/* 0x04 */	inv, inv, inv, inv, 
/* 0x08 */	inv, inv, inv, inv, 
/* 0x0c */	inv, inv, inv, inv, 
/* 0x10 */	inv, inv, inv, inv, 
/* 0x14 */	inv, inv, inv, inv, 
/* 0x18 */	inv, inv, inv, inv, 
/* 0x1c */	inv, inv, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	inv, inv, inv, inv, 
/* 0x2c */	inv, inv, inv, inv, 
/* 0x30 */	inv, inv, inv, inv, 
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	inv, inv, inv, inv, 
/* 0x3c */	inv, inv, inv, inv, 
/* 0x40 */	inv, inv, inv, inv, 
/* 0x44 */	inv, inv, inv, inv,
/* 0x48 */	inv, inv, inv, inv, 
/* 0x4c */	inv, inv, inv, inv, 
/* 0x50 */	inv, inv, inv, inv, 
/* 0x54 */	inv, inv, inv, inv,
/* 0x58 */	inv, inv, inv, inv, 
/* 0x5c */	inv, inv, inv, inv, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, inv, inv,
/* 0x68 */	inv, inv, inv, inv, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	inv, inv, inv, inv,
/* 0x74 */	inv, inv, inv, inv,
/* 0x78 */	inv, inv, inv, inv, 
/* 0x7c */	inv, inv, inv, inv, 
/* 0x80 */	jo_rel_v, jno_rel_v, jb0c0nae_rel_v, jae0nb0nc_rel_v,
/* 0x84 */	je0z_rel_v, jne0nz_rel_v, jbe0na_rel_v, ja0nbe_rel_v,
/* 0x88 */	js_rel_v, jns_rel_v, jp0pe_rel_v, jnp0po_rel_v, 
/* 0x8c */	jl0nge_rel_v, jge0nl_rel_v, jle0ng_rel_v, jg0nle_rel_v, 
/* 0x90 */	seto_rm_b, setno_rm_b, setb0c0nae_rm_b, setae0nb0nc_rm_b,
/* 0x94 */	sete0z_rm_b, setne0nz_rm_b, setbe0na_rm_b, seta0nbe_rm_b,
/* 0x98 */	sets_rm_b, setns_rm_b, setp0pe_rm_b, setnp0po_rm_b, 
/* 0x9c */	setl0nge_rm_b, setge0nl_rm_b, setle0ng_rm_b, setg0nle_rm_b, 
/* 0xa0 */	inv, inv, inv, inv, 
/* 0xa4 */	inv, inv, inv, inv,
/* 0xa8 */	inv, inv, inv, inv,
/* 0xac */	shrdi_v, inv, inv, imul_rm2r_v,
/* 0xb0 */	inv, inv, inv, inv, 
/* 0xb4 */	inv, inv, movzx_rmb2r_v, movzx_rmw2r_l, 
/* 0xb8 */	inv, inv, inv, inv,
/* 0xbc */	inv, inv, movsx_rmb2r_v, movsx_rmw2r_l,
/* 0xc0 */	inv, inv, inv, inv,
/* 0xc4 */	inv, inv, inv, inv,
/* 0xc8 */	inv, inv, inv, inv,
/* 0xcc */	inv, inv, inv, inv,
/* 0xd0 */	inv, inv, inv, inv,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	inv, inv, inv, inv,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, inv,
/* 0xf4 */	inv, inv, inv, inv,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, inv, inv
};

// turn to exec(swaddr_t eip)
make_helper(exec) {
	// get the first bit
	ops_decoded.opcode = instr_fetch(eip, 1);
	return opcode_table[ ops_decoded.opcode ](eip);
}

static make_helper(_2byte_esc) {
	eip ++;
	uint32_t opcode = instr_fetch(eip, 1);
	ops_decoded.opcode = opcode | 0x100;
	ops_decoded.is_2byte = true;
	int len = _2byte_opcode_table[opcode](eip) + 1; 
	ops_decoded.is_2byte = false;
	return len;
}

