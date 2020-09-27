#include "nemu.h"

#define ENTRY_START 0x100000
// #define ENTRY_START 0x0

extern uint8_t entry [];
extern uint32_t entry_len;
extern char *exec_file;

void load_elf_tables(int, char *[]);
void init_regex();
void init_wp_pool();
void init_ddr3();

FILE *log_fp = NULL;

static void init_log() {
	log_fp = fopen("log.txt", "w");
	Assert(log_fp, "Can not open 'log.txt'");
}

static void welcome() {
	printf("Welcome to NEMU!\nThe executable is %s.\nFor help, type \"help\"\n",
			exec_file);
}

void init_monitor(int argc, char *argv[]) {
	/* Perform some global initialization */

	/* Open the log file. */
	init_log();

	/* Load the string table and symbol table from the ELF file for future use. */
	load_elf_tables(argc, argv);

	/* Compile the regular expressions. */
	init_regex();

	/* Initialize the watchpoint pool. */
	init_wp_pool();

	/* Display welcome message. */
	welcome();
}

#ifdef USE_RAMDISK
static void init_ramdisk() {
	int ret;
	const int ramdisk_max_size = 0xa0000;
	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);
	// set fp to the end of the file stream
	fseek(fp, 0, SEEK_END);
	// get the len of the file
	size_t file_size = ftell(fp);
	Assert(file_size < ramdisk_max_size, "file size(%zd) too large", file_size);
	// set fp to the start of the file stream
	fseek(fp, 0, SEEK_SET);
	// hwa_to_va(p) ((void *)(hw_mem + (unsigned)p))
	// read the exec file to the hardware mem
	ret = fread(hwa_to_va(0), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}
#endif

static void load_entry() {
	int ret;
	FILE *fp = fopen("entry", "rb");
	Assert(fp, "Can not open 'entry'");

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	// load the entry file at 0x100000
	ret = fread(hwa_to_va(ENTRY_START), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}

static void init_register(){
	cpu.eip = ENTRY_START;
	cpu.ef = 0x2;
	cpu.cr0.protect_enable = 1;
	int i;
	for(i = R_CS; i <= R_GS; i++){
		cpu.sRegs[i].selector.val = 0;
	}
	cpu.cs.seg_limit = 0xffffffff;
	cpu.ds.seg_limit = 0xffffffff;
	cpu.ss.seg_limit = 0xffffffff;
	cpu.gdtr.base_addr = 0;
	cpu.gdtr.table_limit = 0x3ff;
}

void restart() {
	/* Perform some initialization to restart a program */
#ifdef USE_RAMDISK
	/* Read the file with name `argv[1]' into ramdisk. */
	init_ramdisk();
#endif
	init_cache();
	/* Read the entry code into memory. */
	load_entry();

	/* Set the initial instruction pointer. */
	// cpu.eip = ENTRY_START;
	init_register();

	/* Initialize DRAM. */
	init_ddr3();
}
