/*
 * The Nexus Abstract Architecture
 * Minmal architectural-independent library for implementing system software
 *
 * Please refer to the AM specification
 */
#ifndef __AM_H__
#define __AM_H__

#include <stdint.h>
#include <sys/types.h>
#include <arch.h>

#ifndef NULL
# define NULL ((void *)0)
#endif

#define MAX_CPU 8

typedef struct _Area {
  void *start, *end;
} _Area; 

#define _KEYS(_) \
  _(ESCAPE) _(F1) _(F2) _(F3) _(F4) _(F5) _(F6) _(F7) _(F8) _(F9) _(F10) _(F11) _(F12) \
  _(GRAVE) _(1) _(2) _(3) _(4) _(5) _(6) _(7) _(8) _(9) _(0) _(MINUS) _(EQUALS) _(BACKSPACE) \
  _(TAB) _(Q) _(W) _(E) _(R) _(T) _(Y) _(U) _(I) _(O) _(P) _(LEFTBRACKET) _(RIGHTBRACKET) _(BACKSLASH) \
  _(CAPSLOCK) _(A) _(S) _(D) _(F) _(G) _(H) _(J) _(K) _(L) _(SEMICOLON) _(APOSTROPHE) _(RETURN) \
  _(LSHIFT) _(Z) _(X) _(C) _(V) _(B) _(N) _(M) _(COMMA) _(PERIOD) _(SLASH) _(RSHIFT) \
  _(LCTRL) _(APPLICATION) _(LALT) _(SPACE) _(RALT) _(RCTRL) \
  _(UP) _(DOWN) _(LEFT) _(RIGHT) _(INSERT) _(DELETE) _(HOME) _(END) _(PAGEUP) _(PAGEDOWN)

#define _KEY_NAME(k) _KEY_##k,

enum {
  _KEY_NONE = 0,
  _KEYS(_KEY_NAME)
};

#define _EVENTS(_) \
  _(IRQ_TIME) _(IRQ_IODEV) \
  _(ERROR) _(PAGE_FAULT) _(BUS_ERROR) _(NUMERIC) \
  _(TRAP) _(SYSCALL)

#define _EVENT_NAME(ev) _EVENT_##ev,

enum {
  _EVENT_NULL = 0,
  _EVENTS(_EVENT_NAME)
};

typedef struct _RegSet _RegSet;

typedef struct _Event {
  int event;
  intptr_t cause;
} _Event;

typedef struct _Screen {
  int width, height;
} _Screen;

typedef struct _Protect {
  _Area area; 
  void *ptr;
} _Protect;

#ifdef __cplusplus
extern "C" {
#endif

// =======================================================================
// [0] Turing Machine: code execution & a heap memory
// =======================================================================

void _putc(char ch);
void _halt(int code);
extern _Area _heap;

// =======================================================================
// [1] IO Extension (IOE)
// =======================================================================

void _ioe_init();
unsigned long _uptime();
int _read_key();
void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h);
void _draw_sync();
extern _Screen _screen;

// =======================================================================
// [2] Asynchronous Extension (ASYE)
// =======================================================================

void _asye_init(_RegSet* (*l)(_Event ev, _RegSet *regs));
_RegSet *_make(_Area kstack, void *entry, void *arg);
void _trap();
int _istatus(int enable);

// =======================================================================
// [3] Protection Extension (PTE)
// =======================================================================

void _pte_init(void*(*palloc)(), void (*pfree)(void*));
void _protect(_Protect *p);
void _release(_Protect *p);
void _map(_Protect *p, void *va, void *pa);
void _unmap(_Protect *p, void *va);
void _switch(_Protect *p);
_RegSet *_umake(_Protect *p, _Area ustack, _Area kstack, void *entry, char *const argv[], char *const envp[]);

// =======================================================================
// [4] Multi-Processor Extension (MPE)
// =======================================================================

void _mpe_init(void (*entry)());
int _cpu();
intptr_t _atomic_xchg(volatile intptr_t *addr, intptr_t newval);
void _barrier();
extern int _NR_CPU;

#ifdef __cplusplus
}
#endif

#endif

