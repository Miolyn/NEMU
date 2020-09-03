#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "monitor/monitor.h"
#include "stdlib.h"
#include "string.h"
#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;
void wp_change_info(WP *wp, uint32_t o_v, uint32_t n_v);
void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(bool *success, char *e){
    if (free_ == NULL){
        assert(0);
    }
    char *exp = (char*)malloc(32 * sizeof(char));
    strcpy(exp, e);
    uint32_t res = expr(exp, success);
    if(!*success){
        return NULL;
    }
    if (head == NULL){
        head = free_;
        free_ = free_->next;
        head->next = NULL;
        head->expr = exp;
        head->val = res;
        return head;
    }
    WP* p = free_;
    free_ = free_->next;
    p->next = head;
    head = p;
    head->expr = exp;
    head->val = res;
    return head;
}

bool free_wp(int no){
    WP* q = NULL;
    WP* p = head;
    while(p != NULL){
        if(p->NO == no){
            break;
        }
        q = p;
        p = p->next;
    }
    if (p == NULL){
        return false;
    }
    if (q != NULL){
        q->next = p->next;
    }
    p->next = free_;
    free_ = p;
    if(p == head){
        head = NULL;
    }
    return true;
}

void check_wp(int *state){
    WP *p = head;
    while(p != NULL){
        bool success = true;
        uint32_t v = expr(p->expr, &success);
        if (v != p->val){
            *state = STOP;
            wp_change_info(p, p->val, v);
            p->val = v;
            return;
        }
        p = p->next;
    }
}

void wp_change_info(WP *wp, uint32_t o_v, uint32_t n_v){
    printf("watchpoint trgger, NO:%d,expression:%s,original value:%d,now value:%d\n", wp->NO, wp->expr, o_v, n_v);
}

void wp_info(int eip){
    if (head == NULL){
        printf("no watch point\n");
        return;
    }
    WP *p = head;
    printf("watchpoint info at eip:%d\n", eip);
    while(p != NULL){
        printf("NO:%d,expression:%s,value:%d\n", p->NO, p->expr, p->val);
        p = p->next;
    }
}
