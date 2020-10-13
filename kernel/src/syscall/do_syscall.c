#include "irq.h"

#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
uint32_t mm_brk(uint32_t);
int fs_ioctl(int, uint32_t, void *);

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
		case SYS_write: {
			// ssize_t write(int fd, const void *buf, size_t count);
			// write() writes up to count bytes from the buffer starting at buf to the file referred to by the file descriptor fd.
			/*
RETURN VALUE
       On success, the number of bytes written is returned (zero indicates nothing was written). 
	    It is not an error if this number is smaller than the number of  bytes  requested;  this
       may happen for example because the disk device was filled.  See also NOTES.

       On error, -1 is returned, and errno is set appropriately.

       If  count  is zero and fd refers to a regular file, 
	   then write() may return a failure status if one of the errors below is detected. 
	    If no errors are detected, or error detection
       is not performed, 0 will be returned without causing any other effect.  
	   If count is zero and fd refers to a file other than a regular file, the results are not specified.
			*/
			tf->ebp = tf->ebp;

		}
		default: panic("Unhandled system call: id = %d, eip = 0x%08x", tf->eax, tf->eip);
	}
}

