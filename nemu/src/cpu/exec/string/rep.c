#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(rep) {
	int len;
	int count = 0;
	if(instr_fetch(eip + 1, 1) == 0xc3) {
		/* repz ret */
		exec(eip + 1);
		len = 0;
	}
	else {
		printf("ecx%x\n", cpu.ecx);
		while(cpu.ecx) {
			exec(eip + 1);
			count ++;
			cpu.ecx --;
			assert(ops_decoded.opcode == 0xa4	// movsb
				|| ops_decoded.opcode == 0xa5	// movsw
				|| ops_decoded.opcode == 0xaa	// stosb
				|| ops_decoded.opcode == 0xab	// stosw
				|| ops_decoded.opcode == 0xa6	// cmpsb
				|| ops_decoded.opcode == 0xa7	// cmpsw
				|| ops_decoded.opcode == 0xae	// scasb
				|| ops_decoded.opcode == 0xaf	// scasw
				);
			uint32_t oc = ops_decoded.opcode;
			// /* TODO: Jump out of the while loop if necessary. */
			if((oc == 0xa6 || oc == 0xa7 || oc == 0xae || oc == 0xaf) && !cpu.ZF){
				printf("error\n");
				break;
			}
		}
		len = 1;
	}

#ifdef DEBUG
	char temp[80];
	sprintf(temp, "rep %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif
	
	return len + 1;
}

make_helper(repnz) {
	int count = 0;
	while(cpu.ecx) {
		exec(eip + 1);
		count ++;
		cpu.ecx --;
		assert(ops_decoded.opcode == 0xa6	// cmpsb
				|| ops_decoded.opcode == 0xa7	// cmpsw
				|| ops_decoded.opcode == 0xae	// scasb
				|| ops_decoded.opcode == 0xaf	// scasw
			  );

		/* TODO: Jump out of the while loop if necessary. */
		if(cpu.ZF){
			break;
		}
	}

#ifdef DEBUG
	char temp[80];
	sprintf(temp, "repnz %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif

	return 1 + 1;
}
