#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
    char expr[1024];
    uint32_t val;

} WP;
extern WP* new_wp(bool *success, char *e);
extern bool free_wp(int no);
extern void check_wp(int *state);
extern void wp_info(int eip);
#endif
