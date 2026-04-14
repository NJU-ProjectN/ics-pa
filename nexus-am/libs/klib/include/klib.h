/*
 * Static runtime library for a system software on AbstractMachine
 */

#ifndef __KLIB_H__
#define __KLIB_H__

#include <am.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL  ((void*)0)
#endif

void *kalloc(size_t);
void kfree(void*);

// string.h
void* memset(void* v, int c, size_t n);
void* memcpy(void* dst, const void* src, size_t n);
void* memmove(void* dst, const void* src, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
size_t strlen(const char* s);
char* strcat(char* dst, const char* src);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strtok(char* s,const char* delim);
char *strstr(const char *, const char *);
const char *strchr(const char *s, int c);

// stdlib.h
int atoi(const char* nptr);
int abs(int x);
unsigned long time();
void srand(unsigned int seed);
int rand();

// stdio.h
int printf(const char* fmt, ...);
int sprintf(char* out, const char* format, ...);
int snprintf(char* s, size_t n, const char* format, ...);
int vsprintf(char *str, const char *format, va_list ap);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
int sscanf(const char *str, const char *format, ...);

void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

#define printk printf

// assert.h
#ifdef NDEBUG
  #define assert(ignore) ((void)0)
#else
  #define assert(cond) \
    do { \
      if (!(cond)) { \
        printk("Assertion fail at %s:%d\n", __FILE__, __LINE__); \
        _halt(1); \
      } \
    } while (0)
#endif

// math.h
float sqrtf(float);
float fabsf(float);
float floorf(float);
float ceilf(float);
float sinf(float);
float cosf(float);
float powf(float, float);
float fmodf(float, float);
float sqrtf(float);

// types.h
int toupper(int);
int tolower(int);

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);

#ifdef __cplusplus
}
#endif

#endif
