#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

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
    if (head == NULL){
        head = free_;
        free_ = free_->next;
        head->next = NULL;
        return head;
    }
    WP* p = free_;
    free_ = free_->next;
    p->next = head;
    head = p;
    head->expr = e;
    head->val = expr(e, success);
    return head;
}

void free_wp(int no){
    WP* q = NULL;
    WP* p = head;
    while(p != NULL){
        if(p->NO == no){
            break;
        }
        q = p;
        p = p->next;
    }
    q->next = p->next;
    p->next = free_;
    free_ = p;
}
