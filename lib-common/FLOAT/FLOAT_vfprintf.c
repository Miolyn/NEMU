#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include "FLOAT.h"
#include "sys/mman.h"
#include "trap.h"
extern char _vfprintf_internal;
extern char _fpmaxtostr;
extern char _ppfs_setargs;
extern int __stdio_fwrite(char *buf, int len, FILE *stream);
#define nop 0x90

int tres = 0;
int tf = 0;
int tfz = 0;
int tcnt = 0;
int powTen(int n){
    int res = 1;
    while(n-- != 0){
        res = res * 10;
    }
    return res;
}
int cntTen(int n){
	int res = 0;
	while(n != 0){
		res += 1;
		n = n / 10;
	}
	return res;
}
int trans(int floatZone){
	tfz = floatZone;
	int p[20];
	int punish[20] = {0, 0, 0, 0, -1, -1, -1, -2, -2, -3, -3, -3, -3, -3, -4, -4};
    int i;
	int bound = 9;
    p[1] = 5;
    for(i = 2; i <= 16; i++){
        p[i] = 5 * p[i - 1];
    }
    int res = 0;
    // for(i = 1; i <= 15; i++){
    //     // if((floatZone >> (16 - i)) & 1 == 1){
	// 	if(floatZone & (1 << (16 - i))){
	// 		int tenC = cntTen(p[i]);
	// 		if(tenC > bound){
	// 			res += p[i] / powTen(tenC - bound) / powTen(-punish[i]);
	// 		} else{
	// 			res += p[i] * powTen(bound - tenC) / powTen(-punish[i]);
	// 		}
            
    //     }
    // }
	for(i = 15; i >= 1; i--){
		floatZone >>= 1;
		if(floatZone & 1){
			int tenC = cntTen(p[i]);
			if(tenC > bound){
				res += p[i] / powTen(tenC - bound) / powTen(-punish[i]);
			} else{
				res += p[i] * powTen(bound - tenC) / powTen(-punish[i]);
			}
            
        }
    }
	int cnt = cntTen(res);
	if(cnt > 6){ 
		tcnt = cnt - 6;
		res = res / powTen(cnt - 6);
	}
	tres = res;
	// tcnt = powTen(cnt - 6);
    return res;
}
__attribute__((used)) static int format_FLOAT(FILE *stream, FLOAT f) {
	/* TODO: Format a FLOAT argument `f' and write the formating
	 * result to `stream'. Keep the precision of the formating
	 * result with 6 by truncating. For example:
	 *              f          result
	 *         0x00010000    "1.000000"
	 *         0x00013333    "1.199996"
	 */
	tf = f;
	char buf[80];
	int noS = f;
    if((f >> 31) & 1) noS *= -1;
    int intZone = noS >> 16;
    int floatZone = noS & 0xffff;
    char si[2] = "\0";
    if((f >> 31) & 1) si[0] = '-', si[1] = '\0';
    int res = trans(floatZone);

	// int len = sprintf(buf, "0x%08x", f);
	int len = sprintf(buf, "%s%d.%06u", si, intZone, res);
	return __stdio_fwrite(buf, len, stream);
}

static void modify_vfprintf() {
	char *addr = &_vfprintf_internal;
	char *call = addr + 0x306;
	// printf("call:%x\n", call);
	char *pre = call - 100;
	int offSet = (int)format_FLOAT -  (int)(&_fpmaxtostr);
	// mprotect((void*)((int)pre & 0xfffff000), 4096 * 2, PROT_READ | PROT_WRITE | PROT_EXEC);
	int *off = (int*)(call + 1);
	int originOff = *off;
	*off = originOff + offSet;

	char *fop = call - 10;
	// ff 32                	pushl  (%edx)
	*fop = 0xff;
	*(fop + 1) = 0x32;
	// nop
	*(fop + 2) = nop;
	char *stackSize = fop - 1;
	*stackSize -= 4;

	char *fop1 = fop - 20;
	*fop1 = nop;
	*(fop1 + 1) = nop;
	char *fop2 = fop1 - 4;
	*fop2 = nop;
	*(fop2 + 1)= nop;
	// printf("stack:%x,op:%x,op1:%x,op2:%x\n", stackSize, fop, fop1, fop2);
	/* TODO: Implement this function to hijack the formating of "%f"
	 * argument during the execution of `_vfprintf_internal'. Below
	 * is the code section in _vfprintf_internal() relative to the
	 * hijack.
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = _fpmaxtostr(stream,
				(__fpmax_t)
				(PRINT_INFO_FLAG_VAL(&(ppfs->info),is_long_double)
				 ? *(long double *) *argptr
				 : (long double) (* (double *) *argptr)),
				&ppfs->info, FP_OUT );
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

	/* You should modify the run-time binary to let the code above
	 * call `format_FLOAT' defined in this source file, instead of
	 * `_fpmaxtostr'. When this function returns, the action of the
	 * code above should do the following:
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = format_FLOAT(stream, *(FLOAT *) *argptr);
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

}

static void modify_ppfs_setargs() {
	/* TODO: Implement this function to modify the action of preparing
	 * "%f" arguments for _vfprintf_internal() in _ppfs_setargs().
	 * Below is the code section in _vfprintf_internal() relative to
	 * the modification.
	 */
	// #if 0
	char *ppfs = &_ppfs_setargs;
	// 0xe9
	char *lea = ppfs + 0x71;
	// 801387 - 801355
	int offSet = 0x2D;
	*lea = 0xE9;
	*(lea + 1) = offSet;
	*(lea + 2) = 0;
	*(lea + 3) = 0;
	*(lea + 4) = 0;
	// int *of = lea + 1;
	// *of = offSet;
	// #endif
#if 0
	enum {                          /* C type: */
		PA_INT,                       /* int */
		PA_CHAR,                      /* int, cast to char */
		PA_WCHAR,                     /* wide char */
		PA_STRING,                    /* const char *, a '\0'-terminated string */
		PA_WSTRING,                   /* const wchar_t *, wide character string */
		PA_POINTER,                   /* void * */
		PA_FLOAT,                     /* float */
		PA_DOUBLE,                    /* double */
		__PA_NOARG,                   /* non-glibc -- signals non-arg width or prec */
		PA_LAST
	};

	/* Flag bits that can be set in a type returned by `parse_printf_format'.  */
	/* WARNING -- These differ in value from what glibc uses. */
#define PA_FLAG_MASK		(0xff00)
#define __PA_FLAG_CHAR		(0x0100) /* non-gnu -- to deal with hh */
#define PA_FLAG_SHORT		(0x0200)
#define PA_FLAG_LONG		(0x0400)
#define PA_FLAG_LONG_LONG	(0x0800)
#define PA_FLAG_LONG_DOUBLE	PA_FLAG_LONG_LONG
#define PA_FLAG_PTR		(0x1000) /* TODO -- make dynamic??? */

	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			case (PA_INT|PA_FLAG_LONG):
				GET_VA_ARG(p,ul,unsigned long,ppfs->arg);
				break;
			case PA_CHAR:	/* TODO - be careful */
				/* ... users could use above and really want below!! */
			case (PA_INT|__PA_FLAG_CHAR):/* TODO -- translate this!!! */
			case (PA_INT|PA_FLAG_SHORT):
			case PA_INT:
				GET_VA_ARG(p,u,unsigned int,ppfs->arg);
				break;
			case PA_WCHAR:	/* TODO -- assume int? */
				/* we're assuming wchar_t is at least an int */
				GET_VA_ARG(p,wc,wchar_t,ppfs->arg);
				break;
				/* PA_FLOAT */
			case PA_DOUBLE:
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			case (PA_DOUBLE|PA_FLAG_LONG_DOUBLE):
				GET_VA_ARG(p,ld,long double,ppfs->arg);
				break;
			default:
				/* TODO -- really need to ensure this can't happen */
				assert(ppfs->argtype[i-1] & PA_FLAG_PTR);
			case PA_POINTER:
			case PA_STRING:
			case PA_WSTRING:
				GET_VA_ARG(p,p,void *,ppfs->arg);
				break;
			case __PA_NOARG:
				continue;
		}
		++p;
	}
#endif

	/* You should modify the run-time binary to let the `PA_DOUBLE'
	 * branch execute the code in the `(PA_INT|PA_FLAG_LONG_LONG)'
	 * branch. Comparing to the original `PA_DOUBLE' branch, the
	 * target branch will also prepare a 64-bit argument, without
	 * introducing floating point instructions. When this function
	 * returns, the action of the code above should do the following:
	 */

#if 0
	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
			here:
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			// ......
				/* PA_FLOAT */
			case PA_DOUBLE:
				goto here;
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			// ......
		}
		++p;
	}
#endif

}


void init_FLOAT_vfprintf(void) {
	modify_vfprintf();
	modify_ppfs_setargs();
}
