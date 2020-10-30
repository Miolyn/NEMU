#include "irq.h"

#include <sys/syscall.h>
void serial_printc(char);
void add_irq_handle(int, void (*)(void));
uint32_t mm_brk(uint32_t);
int fs_ioctl(int, uint32_t, void *);

int fs_open(const char *, int);
int fs_read(int, void*, int);
int fs_write(int, void*, int);
int fs_lseek(int, int, int);
int fs_close(int);

static void sys_brk(TrapFrame *tf) {
	tf->eax = mm_brk(tf->ebx);
}

static void sys_ioctl(TrapFrame *tf) {
	tf->eax = fs_ioctl(tf->ebx, tf->ecx, (void *)tf->edx);
}

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The `add_irq_handle' system call is artificial. We use it to
		 * let user program register its interrupt handlers. But this is
		 * very dangerous in a real operating system. Therefore such a
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;

		case SYS_brk: sys_brk(tf); break;
		case SYS_ioctl: sys_ioctl(tf); break;
		/* TODO: Add more system calls. */
		case SYS_open: {
			tf->eax = fs_open((char*)tf->ebx, tf->ecx);
			break;
		}
		case SYS_read: {
			tf->eax = fs_read(tf->ebx, (char*)tf->ecx, tf->edx);
			break;
		}
		case SYS_write: {
			tf->eax = fs_write(tf->ebx, (void*)tf->ecx, tf->edx);
			// uint32_t fd = tf->ebx;

			// if(fd == 1 || fd == 2){
			// 	#ifdef HAS_DEVICE
			// 	char *buf = (char*)tf->ecx;
			// 	int i;
			// 	for(i = 0; i < tf->edx; i++){
			// 		serial_printc(*buf);
			// 		++buf;
			// 	}
			// 	#else
			// 	asm volatile (".byte 0xd6" : : "a"(2), "c"(tf->ecx), "d"(tf->edx));
			// 	#endif
			// 	tf->eax = tf->edx;
			// } else{
			// 	assert(0);
			// }
			// break;
		}
		case SYS_lseek: {
			tf->eax = fs_lseek(tf->ebx, tf->ecx, tf->edx);
			break;
		}
		case SYS_close: {
			tf->eax = fs_close(tf->ebx);
			break;
		}
		default: panic("Unhandled system call: id = %d, eip = 0x%08x", tf->eax, tf->eip);
	}
}

