#include "common.h"
#include <sys/ioctl.h>
#include "string.h"

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;


typedef struct{
	bool opened;
	uint32_t offset;
} Fstate;
enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] __attribute__((used)) = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

static Fstate fStates[NR_FILES + 3];


int fs_ioctl(int fd, uint32_t request, void *p) {
	assert(request == TCGETS);
	return (fd >= 0 && fd <= 2 ? 0 : -1);
}

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* TODO: implement a simplified file system here. */

// The open() system call opens the file specified by pathname. 
// If the specified file does not exist, it may optionally (if O_CREAT is specified in flags) be created by open().
// The return value of open() is a file descriptor, a small, nonnegative integer that is used in subsequent system calls 
int fs_open(const char *pathname, int flags){
	int i;
	for(i = 0; i < NR_FILES; i++){
		if (strcmp(pathname, file_table[i].name) == 0){
			fStates[i + 3].opened = true;
			fStates[i + 3].offset = 0;
			return i + 3;
		}
	}
	assert(0);
	return -1;
}
void serial_printc(char);
// read() attempts to read up to count bytes from file descriptor fd into the buffer starting at buf
// On  files  that support seeking, the read operation commences at the file offset, 
// and the file offset is incremented by the number of bytes read.  If the file offset is at or past
// the end of file, no bytes are read, and read() returns zero.
int fs_read(int fd, void *buf, int len){
	assert(fd > 2);
	assert(fStates[fd].opened);
	int id = fd - 3;
	if(fStates[fd].offset >= file_table[id].size) return 0;
	int l = len;
	if(fStates[id].offset + l >= file_table[id].size){
		l = file_table[id].size - fStates[fd].offset - 1;
	}
	if (l == 0) return 0;
	memset(buf + l, 0, len - l);
	ide_read(buf, file_table[id].disk_offset + fStates[fd].offset, l);
	fStates[fd].offset += l;
	return l;
}

// write() writes up to count bytes from the buffer starting at buf to the file referred to by the file descriptor fd.

int fs_write(int fd, void *buf, int len){
	assert(fd <= 2);
	#ifdef HAS_DEVICE
	// char *buf = (char*)buf;
	int i;
	char *b = (char*)buf
	for(i = 0; i < len; i++){
		serial_printc(*(b));
		++b;
	}
	#else
	asm volatile (".byte 0xd6" : : "a"(2), "c"(b), "d"(len));
	#endif
	return len;
}

// lseek() repositions the file offset of the open file description 
// associated with the file descriptor fd to the argument offset according to the directive whence as follows:
/*
       SEEK_SET
              The file offset is set to offset bytes.

       SEEK_CUR
              The file offset is set to its current location plus offset bytes.

       SEEK_END
              The file offset is set to the size of the file plus offset bytes.
*/
int fs_lseek(int fd, int offset, int whence){
	switch (whence){
		case SEEK_SET: {
			fStates[fd].offset = offset;
			return offset;
		} 
		case SEEK_CUR: {
			fStates[fd].offset += offset;
			return fStates[fd].offset;
		}
		case SEEK_END: {
			fStates[fd].offset = file_table[fd - 3].size + offset;
			return fStates[fd].offset;
		}
	}
	return -1;
}

int fs_close(int fd){
	assert(fd < NR_FILES + 3);
	fStates[fd].opened = false;
	fStates[fd].offset = 0;
	return 0;
}