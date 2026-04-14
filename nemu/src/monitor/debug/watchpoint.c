#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}
WP* new_wp() {
  if (free_ == NULL) {
    return NULL;
  }
  WP *wp = free_;
  free_ = free_->next;
  wp->next = head;
  head = wp;
  return wp;
}
void free_wp(WP *wp) {
  if (wp == NULL) {
    return;
  }
  WP *prev = NULL, *curr = head;
  while (curr != NULL) {
    if (curr == wp) {
      if (prev == NULL) {
        head = curr->next;
      } else {
        prev->next = curr->next;
      }
      curr->next = free_;
      free_ = curr;
      return;
    }
    prev = curr;
    curr = curr->next;
  }
}
int delete_wp(int no) {
  WP *prev = NULL, *curr = head;
  while (curr != NULL) {
    if (curr->NO == no) {
      if (prev == NULL) {
        head = curr->next;
      } else {
        prev->next = curr->next;
      }
      curr->next = free_;
      free_ = curr;
      return 0;
    }
    prev = curr;
    curr = curr->next;
  }
  return -1;
}
void wp_display(){
  WP *wp = head;
  while (wp != NULL) {
    printf("Watchpoint %d: %s = 0x%08x\n", wp->NO, wp->expr, wp->value);
    wp = wp->next;
  }
}
//qwq
//Why i need add useless code to make the commit updated?
//again
/* TODO: Implement the functionality of watchpoint */


