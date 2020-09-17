#ifndef __ELFV_H__
#define __ELFV_H__

extern int find_variable(char *e, bool *success);
extern int find_func(int eip, char *name);
#endif