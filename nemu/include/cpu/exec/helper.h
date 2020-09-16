#ifndef __EXEC_HELPER_H__
#define __EXEC_HELPER_H__

#include "cpu/helper.h"
#include "cpu/decode/decode.h"

#define make_helper_v(name) \
	make_helper(concat(name, _v)) { \
		return (ops_decoded.is_operand_size_16 ? concat(name, _w) : concat(name, _l)) (eip); \
	}

#define do_execute concat4(do_, instr, _, SUFFIX)

#define make_instr_helper(type) \
	make_helper(concat5(instr, _, type, _, SUFFIX)) { \
		return idex(eip, concat4(decode_, type, _, SUFFIX), do_execute); \
	}

#define write_reg(name) \
	void_helper(concat(write_reg_, name)) { \
		concat(reg_, name) = src; \
	}

#define setcc_helper(prefix, condition) \
	make_helper(concat3(set, prefix, _rm_b)) { \
		int len = decode_rm_b(eip + 1); \
		int res = 0; \
		if (condition){ \
			res = 1; \
		} \
		write_operand_b(op_src, res); \
		return len + 1; \
	}




extern char assembly[];
#ifdef DEBUG
#define print_asm(...) Assert(snprintf(assembly, 80, __VA_ARGS__) < 80, "buffer overflow!")
#else
#define print_asm(...)
#endif

#define print_asm_template1() \
	print_asm(str(instr) str(SUFFIX) " %s", op_src->str)

#define print_asm_template2() \
	print_asm(str(instr) str(SUFFIX) " %s,%s", op_src->str, op_dest->str)

#define print_asm_template3() \
	print_asm(str(instr) str(SUFFIX) " %s,%s,%s", op_src->str, op_src2->str, op_dest->str)

#define print_asm_template4() \
	print_asm(str(instr) str(SUFFIX) " %s,%s", cpu.eip, op_src->str, )

#define print_asm_template5() \
	print_asm(str(instr) str(SUFFIX))

#define print_temp1() \
	printf("dest:0x%x, src:0x%x, res:0x%x\n", op_dest->val, op_src->val, res);

#define jcc_helper(prefix, condition) \
	make_helper(concat4(j, prefix, _rel_, SUFFIX)){ \
		eip += 1; \
		int len = concat(decode_si_, SUFFIX)(eip); \
		if(condition){ \
			if (ops_decoded.is_operand_size_16){ \
				cpu.eip = (eip + op_src->val) & 0xFFFF; \
			} else{ \
				cpu.eip = eip + op_src->val; \
			} \
			if(ops_decoded.is_2byte){ \
				cpu.eip -= 1; \
			} \
		} else { \
			len += 1; \
		} \
		print_asm(str(instr) str(SUFFIX) str(prefix) " %d", condition); \
		return len; \
	}

#endif



