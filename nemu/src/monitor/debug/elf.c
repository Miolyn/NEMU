#include "common.h"
#include "monitor/elfv.h"
#include <stdlib.h>
#include <elf.h>

char *exec_file = NULL;

static char *strtab = NULL;
static Elf32_Sym *symtab = NULL;
static int nr_symtab_entry;

void load_elf_tables(int argc, char *argv[]) {
	int ret;
	Assert(argc == 2, "run NEMU with format 'nemu [program]'");
	exec_file = argv[1];

	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);
	uint8_t buf[sizeof(Elf32_Ehdr)];
	ret = fread(buf, sizeof(Elf32_Ehdr), 1, fp);
	assert(ret == 1);

	/* The first several bytes contain the ELF header. */
	Elf32_Ehdr *elf = (void *)buf;
	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	/* Check ELF header */
	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version


	/* Load symbol table and string table for future use */

	/* Load section header table */
	uint32_t sh_size = elf->e_shentsize * elf->e_shnum;
	// Elf32_Shdr  节描述符。 ELF文件节表
	Elf32_Shdr *sh = malloc(sh_size);
	// 设置流 stream 的文件位置为给定的偏移 offset，参数 offset 意味着从给定的 whence 位置查找的字节数。
	// stream -- 这是指向 FILE 对象的指针，该 FILE 对象标识了流。
	// offset -- 这是相对 whence 的偏移量，以字节为单位。
	// SEEK_SET	文件的开头
	fseek(fp, elf->e_shoff, SEEK_SET);
	// 从给定流 stream 读取数据到 ptr 所指向的数组中。
	ret = fread(sh, sh_size, 1, fp);
	assert(ret == 1);

	/* Load section header string table */
	char *shstrtab = malloc(sh[elf->e_shstrndx].sh_size);
	fseek(fp, sh[elf->e_shstrndx].sh_offset, SEEK_SET);
	ret = fread(shstrtab, sh[elf->e_shstrndx].sh_size, 1, fp);
	assert(ret == 1);

	int i;
	for(i = 0; i < elf->e_shnum; i ++) {
		// symbol table
		if(sh[i].sh_type == SHT_SYMTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".symtab") == 0) {
			/* Load symbol table from exec_file */
			symtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(symtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
			nr_symtab_entry = sh[i].sh_size / sizeof(symtab[0]);
		}
		else if(sh[i].sh_type == SHT_STRTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".strtab") == 0) {
			/* Load string table from exec_file */
			strtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(strtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
		}
	}

	free(sh);
	free(shstrtab);

	assert(strtab != NULL && symtab != NULL);

	fclose(fp);
}

int find_variable(char *e, bool *success){
	int i, offSet;
	for(i = 0; i < nr_symtab_entry; i++){
		if((symtab[i].st_info & 0xf) == STT_OBJECT){
			offSet = symtab[i].st_name;
			if(strncmp(e, strtab + offSet, strlen(e)) == 0){
				return symtab[i].st_value;
			}
		}
	}
	*success = 0;
	return 0;
}

int find_func(int eip, char *name){
	int i, baseAddr, size, offSet;
	printf("nr%d\n", nr_symtab_entry);
	for(i = 0; i < nr_symtab_entry; i++){
		if((symtab[i].st_info & 0xf) == STT_FUNC){
			baseAddr = symtab[i].st_value;
			size = symtab[i].st_size;
			offSet = symtab[i].st_name;
			if(eip >= baseAddr && eip <= baseAddr + size){
				int len = symtab[i + 1].st_name - symtab[i].st_name - 1;
				printf("i:%d, i+1l:%x, il:%x\n", i,symtab[i + 1].st_name, symtab[i].st_name);
				printf("len:%d\n", len);
				for(i = 0; strtab[offSet + i]; i++){
					printf("%c\n", strtab[offSet + i]);
				}
				// strncpy(name, strtab + offSet, len);
				name[len] = '\0';
				return 0;
			}
		}
	}
	return -1;
}