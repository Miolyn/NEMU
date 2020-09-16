#include "nemu.h"
#include <stdlib.h>
#include <time.h>

#define ll long long 
CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
const char *regef[] = {"cf", "pos1", "pf", "pos3", "af", "pos5", "zf", "sf", "tf", "if", "df", "of", "ol", "ip", "nt", "pos15", "rf", "vm"};
void reg_test() {
	srand(time(0));
	uint32_t sample[8];
	uint32_t eip_sample = rand();
	cpu.eip = eip_sample;

	int i;
	for(i = R_EAX; i <= R_EDI; i++) {
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);

	assert(eip_sample == cpu.eip);
}

uint32_t get_reg_by_str(bool *success, char *e){
    int i;
    if (strlen(e) == 3){
        for (i = R_EAX; i <= R_EDI; i++){
            if (strcmp(e, regsl[i]) == 0){
                return reg_l(i);
            }
        }
		if (strcmp(e, "eip") == 0){
			return cpu.eip;
		}
		*success = false;
        return 0;
    } else if(strlen(e) == 2){
        for(i = R_AX; i <= R_DI; i++){
            if(strcmp(e, regsw[i]) == 0){
                return reg_w(i);
            }
        }
        for(i = R_AL; i <= R_BH; i++){
            if(strcmp(e, regsb[i]) == 0){
                return reg_b(i);
            }
        }
		// for(i = CF; i <= VM; i++){
		// 	if(strcmp(e, regef[i]) == 0){
		// 		return reg_eflags(i);
		// 	}
		// }
		*success = false;
        return 0;
    } else{
        *success = false;
        return 0;
    }
    return 0;
}
void reset_all_eflags(){
	cpu.CF = 0;
	cpu.PF = 0;
	cpu.AF = 0;
	cpu.ZF = 0;
	cpu.SF = 0;
	cpu.OF = 0;
}

// for unsigned int
int carry_flag(int dest, int src){
	int res = dest + src;
	// uint64_t d = dest;
	// uint64_t s = src;
	// uint64_t r = d + s;
	// reg_eflags(CF) = r >> 32;
	// return (uint32_t)r; 
	// uint64_t res = ((uint64_t)dest & 0xFFFFFFFF) + ((uint64_t)src & 0xFFFFFFFF);
	// reg_eflags(CF) = (res >> 32) & 1;
	// printf("res>>32:0x%x\n", (int)(res >> 32));
	cpu.CF = res < dest;

	return (uint32_t)res;
}



void parity_flag(int res){
	int low = low8(res);
	int cnt = 0;
	int i = 0;
	for(i = 0; i < 7; i++){
		cnt += low & 1;
		low >>= 1;
	}
	cnt += low & 1;
	cpu.PF = !(cnt & 1);
	return;
	int tmp = (low >> 4) ^ (low & 0xF);
	int tmp1 = (tmp >> 2) ^ (tmp & 0b11);
	// odd
	
	if ((tmp1 >> 1) ^ (tmp1 & 1)){
		cpu.PF = 0;
	} else{
		cpu.PF = 1;
	}
}

void adjust_flag(int dest, int src){
	int low4dest = dest & 0xF;
	int low4src = src & 0xF;
	if (low4dest + low4src > 0xF){
		cpu.AF = 1;
	} else{
		cpu.AF = 0;
	}
}

void zero_flag(int res){
	// printf("%x,b:%d\n", res, (res==0));
	cpu.ZF = (res == 0);
}

void sign_flag(int res){
	cpu.SF = sign_bit32(res);
	return;
	if (res < 0){
		cpu.SF = 1;
	} else{
		cpu.SF = 0;
	}
}

void overflow_flag(int dest, int src){
	uint32_t res = dest + src;
	if(sign_bit32(dest) && sign_bit32(src) && !sign_bit32(res)){
		cpu.OF = 1;
	} else if(!sign_bit32(dest) && !sign_bit32(src) && sign_bit32(res)){
		cpu.OF = 1;
	} else{
		cpu.OF = 0;
	}
	return;
	if (dest < 0 && src < 0 && res > 0){
		cpu.OF = 1;
	} else if(dest > 0 && src > 0 && res < 0){
		cpu.OF = 1;
	} else{
		cpu.OF = 0;
	}
	// return res;
}
