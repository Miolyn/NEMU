#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "monitor/elfv.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

typedef struct {
    swaddr_t prev_ebp;
    swaddr_t ret_addr;
    uint32_t args[4];
} PartOfStackFrame;


/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
    int n = 1;
    if (args != NULL){
        n = atoi(args);
    }
    cpu_exec(n);
    return 0;
}   

static int cmd_info(char *args){
    if (args == NULL){
        return 0;
    }
    if (strcmp(args, "r") == 0){
        char *regName[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
        int i;
        for (i = R_EAX; i <= R_EDI; i++){
            printf("%s: %x\n", regName[i], reg_l(i));
        }
    } else if (strcmp(args, "w") == 0){
        wp_info(cpu.eip);
    }
    
    return 0;
}

static int cmd_p(char *args){
    bool success = true;
    uint32_t res = expr(args, &success);
    if (!success){
        printf("error expression\n");
        return 0;
    }
    printf("%d\n", res);
    printf("hex:0x%x\n", res);
    return 0;
}

static int cmd_x(char *args){
    if (args == NULL){
        printf("n needed, please enter the cmd like [x 10 0x100000]\n");
        return 0;
    }
    char *nStr = strtok(args, " ");
    if (nStr == NULL){
        printf("expr needed, please enter the cmd like [x 10 0x100000]\n");
        return 0;
    }
    int n = atoi(nStr);
    if (n <= 0){
        return 0;
    }
    char *exprStr = nStr + strlen(nStr) + 1;
    bool success = true;
    uint32_t uAddr = expr(exprStr, &success);
    if (!success){
        printf("error expression\n");
        return 0;
    }
    int i;
    for (i = 0; i < n; i++){
        uint32_t addrI = uAddr + i * 4;
        uint32_t val = swaddr_read(addrI, 4);
        printf("(+0x%x)addr:0x%x,val:0x%x\n", i * 4, addrI, val);
        int j;
        for(j = 0; j < 4; j++){
            val = swaddr_read(addrI + j, 1);
            printf("%c", val);
        }
        printf("\n");
    }
    return 0;
}

static int cmd_w(char *args){
    bool success = true;
    WP *wp = new_wp(&success, args);
    if (!success){
        printf("expression error\n");
        return 0;
    }
    printf("set watchpoint no:%d\n", wp->NO);
    return 0;
}

static int cmd_d(char *args){
    if (args == NULL){
        printf("watchpoint no needed, please enter the cmd like [d 0]\n");
        return 0;
    }
    int n = atoi(args);
    bool suc = free_wp(n);
    if(suc){
        printf("free watchpoint no:%d success\n", n);
        return 0;
    }
    printf("free watchpoint no:%d error\n", n);
    return 0;
}

static int cmd_pt(char *args){
    if (args == NULL){
        printf("n needed\n");
        return 0;
    }
    int n = atoi(args);
    int i = 0;
    for(i = 0; i < n; i++){
        printf("+0x%x($esp)=0x%x\n", i * 4, swaddr_read(cpu.esp + i * 4, 4));
    }
    return 0;
}

static int cmd_bt(char *args){
    char name[255];
    int esp = cpu.esp, ebp = cpu.ebp, eip = cpu.eip, i;
    while(ebp){
        int res = find_func(eip, name);
        if(res){
            return 0;
        }
        printf("-----------------------------\n");
        printf("in function [%s]\n", name);
        int addr = ebp + 8;
        for(i = 0; i < 4; i++){
            printf("arg%d: addr:0x%x, val:0x%x\n", i + 1, addr + i * 4,swr4(addr + i * 4));
        }
        printf("-----------------------------\n");
        esp = ebp - 8;
        eip = swr4(ebp - 4);
        ebp = swr4(ebp);
        printf("eip:%x\n", eip);
    }
    return 0;
}

// function test2
static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },

	/* TODO: Add more commands */
    {"si", "run by step, argument n needed", cmd_si },
    {"/", "run by step, argument n needed", cmd_si },
    {"info", "print the info of register or watchpoint", cmd_info },
    {"x", "print n value of the memory addr", cmd_x },
    {"p", "calculate expression, support negative number and *addr, $register only support lowercase reg name", cmd_p },
    {"w", "add watch point", cmd_w},
    {"d", "delete watch point", cmd_d},
    {"pt", "print esp", cmd_pt},
    {"bt", "print the stack link", cmd_bt},

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
