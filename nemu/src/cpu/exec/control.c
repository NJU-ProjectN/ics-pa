#include "cpu/exec.h"

make_EHelper(jmp) {
  // jmp 是无条件跳转
  decoding.jmp_eip = decoding.seq_eip + id_dest->val;
  decoding.is_jmp = 1;

  print_asm_template1(jmp);
}

make_EHelper(jcc) {
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode); // 不要 >> 1 ！！！
  
  // 打印如果报错可以暂时不管，核心是跳对！
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
  vaddr_write(cpu.esp, 4, return_addr);

  // 2. 计算跳转目标
  // Target = 返回地址 + 译码出的偏移量
  decoding.jmp_eip = decoding.seq_eip + id_dest->val;
  decoding.is_jmp = 1;

  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  printf("[RET DEBUG] Target EIP pushed to jmp_eip: 0x%08x\n", decoding.jmp_eip);
  rtlreg_t target_eip = vaddr_read(cpu.esp, 4);
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
