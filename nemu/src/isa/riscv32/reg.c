#include <isa.h>
#include <stdio.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

#define REGS_NUM ARRLEN(regs)

/* hhw::note 输出所有寄存器信息，格式为 regName \t\t 八进制 \t\t 十进制 */
void isa_reg_display() {
  printf("pc\t\t0x%x\t\t%d\n", cpu.pc, cpu.pc);
  for (int i = 0; i < REGS_NUM; i ++) {
    // hhw::note 这里 width 参数暂时未用上，随便传一个
    printf("%s\t\t0x%08x\t\t%d\n",reg_name(i, 0), cpu.gpr[i]._32, cpu.gpr[i]._32);
  }
}

word_t isa_reg_str2val(const char *s, bool *success) {
  return 0;
}
