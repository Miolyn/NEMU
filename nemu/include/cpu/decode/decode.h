#ifndef __DECODE_H__
#define __DECODE_H__

#include "cpu/helper.h"

make_helper(decode_r_b);
make_helper(decode_r_w);
make_helper(decode_r_l);
make_helper(decode_rm_b);
make_helper(decode_rm_w);
make_helper(decode_rm_l);
make_helper(decode_i_b);
make_helper(decode_i_w);
make_helper(decode_i_l);
make_helper(decode_si_b);
make_helper(decode_si_w);
make_helper(decode_si_l);

make_helper(decode_m_b);
make_helper(decode_m_w);
make_helper(decode_m_l);


make_helper(decode_i2rm_b);
make_helper(decode_i2rm_w);
make_helper(decode_i2rm_l);
make_helper(decode_i2a_b);
make_helper(decode_i2a_w);
make_helper(decode_i2a_l);
make_helper(decode_i2r_b);
make_helper(decode_i2r_w);
make_helper(decode_i2r_l);
make_helper(decode_si2rm_b);
make_helper(decode_si2rm_w);
make_helper(decode_si2rm_l);
make_helper(decode_si_rm2r_w);
make_helper(decode_si_rm2r_l);
make_helper(decode_i_rm2r_w);
make_helper(decode_i_rm2r_l);
make_helper(decode_r2rm_b);
make_helper(decode_r2rm_w);
make_helper(decode_r2rm_l);
make_helper(decode_rm2r_b);
make_helper(decode_rm2r_w);
make_helper(decode_rm2r_l);
make_helper(decode_rm2a_b);
make_helper(decode_rm2a_w);
make_helper(decode_rm2a_l);

make_helper(decode_rm_1_b);
make_helper(decode_rm_1_w);
make_helper(decode_rm_1_l);
make_helper(decode_rm_cl_b);
make_helper(decode_rm_cl_w);
make_helper(decode_rm_cl_l);
make_helper(decode_rm_imm_b);
make_helper(decode_rm_imm_w);
make_helper(decode_rm_imm_l);

make_helper(decode_ai_b);
make_helper(decode_ai_w);
make_helper(decode_ai_l);

make_helper(decode_si2a_b);
make_helper(decode_si2a_w);
make_helper(decode_si2a_l);

void write_operand_b(Operand *, uint8_t);
void write_operand_w(Operand *, uint16_t);
void write_operand_l(Operand *, uint32_t);

void_helper(push_stack_w);
void_helper(push_stack_l);

void_op_helper(pop_stack_w);
void_op_helper(pop_stack_l);
#endif
