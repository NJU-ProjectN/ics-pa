#include "cpu/exec.h"

make_EHelper(jmp) {
  // 目标地址在译码阶段已由框架计算在 decoding.jmp_eip 中
  decoding.is_jmp = 1;

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  uint8_t subcode = decoding.opcode & 0xf;
  
  // 获取条件码状态，结果存入 t2
  rtl_setcc(&t2, subcode);
  
  // 如果条件满足，is_jmp = 1，NEMU 会自动跳转到 decoding.jmp_eip
  // 如果条件不满足，is_jmp = 0，NEMU 会自动顺序执行到 decoding.seq_eip
  decoding.is_jmp = t2;

  print_asm("j%s %x", get_cc_name(subcode), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  // 间接跳转（如 jmp *%eax），目标地址保存在操作数中
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // 压入返回地址（当前指令的下一条指令地址）
  rtl_push(&decoding.seq_eip);
  decoding.is_jmp = 1;

  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(call_rm) {
  // 间接调用（如 call *%eax）
  rtl_push(&decoding.seq_eip);
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("call *%s", id_dest->str);
}

make_EHelper(ret) {
  // 弹出返回地址到临时寄存器，并赋给 jmp_eip
  rtl_pop(&t2);
  decoding.jmp_eip = t2;
  decoding.is_jmp = 1;

  print_asm("ret");
}

make_EHelper(ret_imm) {
  // 弹出返回地址
  rtl_pop(&t2);
  decoding.jmp_eip = t2;
  decoding.is_jmp = 1;
  
  // 释放栈空间（平栈）：ESP <- ESP + imm
  rtl_addi(&cpu.esp, &cpu.esp, id_dest->val);

  print_asm("ret %d", id_dest->val);
}