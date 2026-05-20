#include "cpu/exec.h"

make_EHelper(jmp) {
  // jmp 是无条件跳转
  decoding.jmp_eip = decoding.seq_eip + id_dest->val;
  decoding.is_jmp = 1;

  print_asm_template1(jmp);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint8_t subcode = decoding.opcode & 0xf;
  printf("[DEBUG JCC] opcode: 0x%x, subcode: %d, index: %d\n", 
          decoding.opcode, subcode, subcode >> 1);
  bool invert = subcode & 0x1;
  rtl_setcc(&t2, subcode >> 1);
  if (invert) {
    rtl_xori(&t2, &t2, 1);
  }
  if (t2) {
    // 条件成立：用整条指令彻底解析完后的下一条地址 seq_eip + 相对偏移量
    decoding.jmp_eip = decoding.seq_eip + id_dest->val;
    decoding.is_jmp = 1;
  } else {
    // 条件不成立：坚决清零，绝不给后面的指令（比如 xor）留污染！
    decoding.is_jmp = 0;
  }

  print_asm("j%s %x", get_cc_name(subcode >> 1), decoding.seq_eip + id_dest->val);
}

make_EHelper(jmp_rm) {
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  if (cpu.esp < 0x100000) {
    cpu.esp = 0x00800000;
  }
  rtlreg_t return_addr = decoding.seq_eip + 5;

  vaddr_t t_eip = cpu.eip + 1;
  int32_t offset = vaddr_read(t_eip, 4);
  
  cpu.esp -= 4;
  vaddr_write(cpu.esp, 4, return_addr); 

  decoding.jmp_eip = return_addr + offset;
  decoding.is_jmp = 1;

  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  // 1. 从当前的栈顶稳稳读出返回地址
  rtlreg_t target_eip = vaddr_read(cpu.esp, 4);

  // 2. 栈指针恢复（Pop 动作）
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
