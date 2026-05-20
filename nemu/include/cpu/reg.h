#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
    union {
        // 这一层 union 包含 gpr 数组，用于下标访问
        union {
            uint32_t _32;
            uint16_t _16;
            uint8_t _8[4];
        } gpr[8];

        // 这一层 struct 包含具名寄存器，用于直接访问
        struct {
            uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
        };
    };
    vaddr_t eip;
    uint32_t eflags;
} CPU_state;

extern CPU_state cpu;

#define R_EAX 0
#define R_ECX 1
#define R_EDX 2
#define R_EBX 3
#define R_ESP 4
#define R_EBP 5
#define R_ESI 6
#define R_EDI 7

static inline int check_reg_index(int index) {
  assert(index >= 0 && index < 8);
  return index;
}
// 提供一个访问器，确保译码器永远访问的是同一块物理内存
static inline rtlreg_t* reg_l(int index) {
  // 显式指向联合体内部的 ._32 成员，这样返回的指针就是 uint32_t *
  return &cpu.gpr[check_reg_index(index)]._32;
}




#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

static inline void set_reg_l(int i, uint32_t val) {
  cpu.gpr[check_reg_index(i)]._32 = val;
}
static inline void set_reg_w(int i, uint32_t val) {
  int r = check_reg_index(i);
  cpu.gpr[r]._32 =
      (cpu.gpr[r]._32 & 0xffff0000) |
      (val & 0xffff);
}
static inline void set_reg_b(int i, uint32_t val) {
  int r = check_reg_index(i & 0x3);
  int off = (i >> 2) * 8;

  cpu.gpr[r]._32 =
      (cpu.gpr[r]._32 & ~(0xff << off)) |
      ((val & 0xff) << off);
}

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

static inline const char* reg_name(int index, int width) {
  assert(index >= 0 && index < 8);
  switch (width) {
    case 4: return regsl[index];
    case 1: return regsb[index];
    case 2: return regsw[index];
    default: assert(0);
  }
}

#endif
