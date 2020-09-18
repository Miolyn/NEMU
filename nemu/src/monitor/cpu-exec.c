#include "monitor/monitor.h"
#include "cpu/helper.h"
#include <setjmp.h>
#include "monitor/watchpoint.h"
/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10

int nemu_state = STOP;

int exec(swaddr_t);

char assembly[80];
char asm_buf[128];

/* Used with exception handling. */
jmp_buf jbuf;

void print_bin_instr(swaddr_t eip, int len) {
	int i;
	int l = sprintf(asm_buf, "%8x:   ", eip);
	for(i = 0; i < len; i ++) {
		l += sprintf(asm_buf + l, "%02x ", instr_fetch(eip + i, 1));
	}
	sprintf(asm_buf + l, "%*.s", 50 - (12 + 3 * len), "");
}

/* This function will be called when an `int3' instruction is being executed. */
void do_int3() {
	printf("\nHit breakpoint at eip = 0x%08x\n", cpu.eip);
	nemu_state = STOP;
}

/* Simulate how the CPU works. */
void cpu_exec(volatile uint32_t n) {
	if(nemu_state == END) {
		printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
		return;
	}
	nemu_state = RUNNING;

#ifdef DEBUG
	volatile uint32_t n_temp = n;
#endif

	setjmp(jbuf);
	int cnt = 0;
	for(; n > 0; n --) {
#ifdef DEBUG
		swaddr_t eip_temp = cpu.eip;
		if((n & 0xffff) == 0) {
			/* Output some dots while executing the program. */
			fputc('.', stderr);
		}
#endif

		/* Execute one instruction, including instruction fetch,
		 * instruction decode, and the actual execution. */
		// if (cpu.eip == 0x100199){
		// 	assert(0);
		// }
		printf("----------------------------------------------------------------------------\n");
		printf("start to exec at eip:%x and opcode is %x\n", cpu.eip, instr_fetch(cpu.eip, 1));

		int instr_len = exec(cpu.eip);
		printf("end exec\n");
		int j;
		for(j = R_EAX; j <= R_EDI; j++){
			if (j != R_ESP && j != R_EBP && j != R_ESI && j != R_EDI){
				printf("reg_%s:0x%x ;", regsl[j], reg_l(j));
			}
				
		}
		printf("\n");
		j = R_ESI;
		if (reg_l(j) < (1 << (10 + 10 + 3 + (27 - 10 - 10 - 3)))){
			printf("reg %s:0x%x ,mem:l:0x%x,w:0x%x,b:0x%x ;\n", regsl[j], reg_l(j), swaddr_read(reg_l(j), 4), swaddr_read(reg_l(j), 2), swaddr_read(reg_l(j), 1));
		} else{
			printf("reg_%s:0x%x ;\n", regsl[j], reg_l(j));
		}
		j = R_EDI;
		if (reg_l(j) < (1 << (10 + 10 + 3 + (27 - 10 - 10 - 3)))){
			printf("reg %s:0x%x ,mem:l:0x%x,w:0x%x,b:0x%x ;\n", regsl[j], reg_l(j), swaddr_read(reg_l(j), 4), swaddr_read(reg_l(j), 2), swaddr_read(reg_l(j), 1));
		} else{
			printf("reg_%s:0x%x ;\n", regsl[j], reg_l(j));
		}
		j = R_EBP;
		printf("reg_%s:0x%x ;\n", regsl[j], reg_l(j));
		j = R_ESP;
		if (reg_l(j) < (1 << (10 + 10 + 3 + (27 - 10 - 10 - 3)))){
			printf("reg %s:0x%x ,mem:l:0x%x,w:0x%x,b:0x%x ;\n", regsl[j], reg_l(j), swaddr_read(reg_l(j), 4), swaddr_read(reg_l(j), 2), swaddr_read(reg_l(j), 1));
		} else{
			printf("reg_%s:0x%x ;\n", regsl[j], reg_l(j));
		}
		printf("CF:%d; PF:%d; AF:%d; ZF:%d; SF:%d; OF:%d; \n", cpu.CF,
			cpu.PF, cpu.AF, cpu.ZF, cpu.SF, cpu.OF);
		j = R_ESP;
		if ((int)reg_l(j) - 16 >= 0 && reg_l(j) + 16 <= (1 << 27)){
			int k;
			printf("ESP\n");
			for(k = -4; k <= 4; k++){
				int addr = reg_l(j) + 4 * k;
				printf("addr:(+0x%x)0x%x ,mem:l:0x%x,w:0x%x,b:0x%x ;\n", 4 * k, addr, swaddr_read(addr, 4), swaddr_read(addr, 2), swaddr_read(addr, 1));
			}
			
		}
		j = R_EBP;
		if (reg_l(j) - 16 >= 0 && reg_l(j) + 16 < (1 << (10 + 10 + 3 + (27 - 10 - 10 - 3)))){
			int k;
			printf("EBP\n");
			for(k = -4; k <= 4; k++){
				int addr = reg_l(j) + 4 * k;
				printf("addr:(+0x%x)0x%x ,mem:l:0x%x,w:0x%x,b:0x%x ;\n", 4 * k, addr, swaddr_read(addr, 4), swaddr_read(addr, 2), swaddr_read(addr, 1));
			}
			
		}
		printf("----------------------------------------------------------------------------\n");
		cpu.eip += instr_len;
		if(cpu.tmp){
			cpu.tmp = false;
			nemu_state = STOP;
			return;
		}
// #ifdef DEBUG
		print_bin_instr(eip_temp, instr_len);
		strcat(asm_buf, assembly);
		Log_write("%s\n", asm_buf);
		if(n_temp < MAX_INSTR_TO_PRINT) {
			printf("%s\n", asm_buf);
		}
// #endif

		/* TODO: check watchpoints here. */
        check_wp(&nemu_state);

#ifdef HAS_DEVICE
		extern void device_update();
		device_update();
#endif
		++cnt;
		printf("cnt:%d\n", cnt);
		if(nemu_state != RUNNING) { return; }
	}

	if(nemu_state == RUNNING) { nemu_state = STOP; }
}
