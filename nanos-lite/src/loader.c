#include "common.h"

#define DEFAULT_ENTRY ((void *)0x4000000)

uintptr_t loader(_Protect *as, const char *filename) {
  TODO();
  return (uintptr_t)DEFAULT_ENTRY;
}
