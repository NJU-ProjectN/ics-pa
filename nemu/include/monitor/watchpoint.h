#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expr[256];
  uint32_t value;
  /* TODO: Add more members if necessary */

} WP;
void wp_display();
WP* new_wp();
void free_wp(WP *wp);
int delete_wp(int no);
#endif
