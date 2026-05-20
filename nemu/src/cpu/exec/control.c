#include "cpu/exec.h"

make_EHelper(jmp) {
  // jmp 是无条件跳转
  decoding.jmp_eip = decoding.seq_eip + id_dest->val;
  decoding.is_jmp = 1;

  print_asm_template1(jmp);
}

make_EHelper(jcc) {
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode); 
  

  print_asm("jcc %x", decoding.seq_eip + id_dest->val); 
  
  if (t2) {
    decoding.jmp_eip = decoding.seq_eip + id_dest->val;
    decoding.is_jmp = 1;
  } else {
    decoding.is_jmp = 0;
  }
}

make_EHelper(jmp_rm) {
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  rtlreg_t return_addr = decoding.seq_eip;
  
  cpu.esp -= 4;
  if (cpu.esp < 0x7000 || cpu.esp > 0x8000) {
    panic("Stack pointer corrupted! ESP: 0x%08x", cpu.esp);
}
  vaddr_write(cpu.esp, 4, return_addr);

  // 2. 计算跳转目标
  // Target = 返回地址 + 译码出的偏移量
  int32_t offset = id_dest->val;
  if (id_dest->width == 1) {
    offset = (int32_t)(int8_t)id_dest->val;
  } else if (id_dest->width == 2) {
    offset = (int32_t)(int16_t)id_dest->val;
  } // 如果是 4 字节，本身就是 32 位完整有符号数，无需处理

  decoding.jmp_eip = decoding.seq_eip + offset;
  decoding.is_jmp = 1;

  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  if (cpu.esp < 0x7000 || cpu.esp > 0x8000) {
    panic("Stack pointer corrupted! ESP: 0x%08x", cpu.esp);
}
  rtlreg_t target_eip = vaddr_read(cpu.esp, 4);
  printf("[RET REAL] Current ESP: 0x%08x, Value in Stack: 0x%08x\n", cpu.esp, target_eip);
  cpu.esp += 4;
  decoding.jmp_eip = target_eip;
  decoding.is_jmp = 1;

  print_asm("ret");
}

make_EHelper(call_rm) {
  rtl_push(&decoding.seq_eip);
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("call *%s", id_dest->str);
}
make_EHelper(ret_imm) {
  rtl_pop(&t0);
  decoding.jmp_eip = t0;
  decoding.is_jmp = 1;
  
  rtl_addi(&cpu.esp, &cpu.esp, id_dest->val);

  print_asm("ret %d", id_dest->val);
}
