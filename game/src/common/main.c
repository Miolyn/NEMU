#include "game-common.h"
#include "x86.h"
#include "FLOAT.h"

#include <unistd.h>

void init_timer();
void main_loop();

/* Invoke `system call' number SYSNO, passing it the remaining arguments.
   This is completely system-dependent, and not often useful.

   In Unix, `syscall' sets `errno' for all errors and most calls return -1
   for errors; in many systems you cannot pass arguments or get return
   values for all system calls (`pipe', `fork', and `getppid' typically
   among them).

   In Mach, all system calls take normal arguments and always return an
   error code (zero for success).  */
void add_irq_handle(int irq, void *handler) {
	// .globl irq0;     irq0:  pushl $0;  pushl $1000; jmp asm_do_irq
	syscall(0, irq, handler);
}

int
main(void) {
	init_timer();

	add_irq_handle(0, timer_event);
	add_irq_handle(0, keyboard_event);
	init_FLOAT_vfprintf();

	Log("game start!");

	main_loop();

	assert(0); /* main_loop是死循环，永远无法返回这里 */

	return 0;
}
