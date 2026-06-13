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

  if (t2) {
    decoding.jmp_eip = decoding.seq_eip + id_dest->val;
    decoding.is_jmp = 1;
  } else {
    decoding.is_jmp = 0;
  }

  print_asm("jcc %x", decoding.is_jmp ? decoding.jmp_eip : decoding.seq_eip);
}

make_EHelper(jmp_rm) {
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
    rtlreg_t ret_addr = decoding.seq_eip; 
    rtl_push(&ret_addr);                       // 压入返回地址

    decoding.jmp_eip = decoding.seq_eip + (int32_t)id_dest->val;
    decoding.is_jmp = 1;

    print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
    rtlreg_t target_eip;
    rtl_pop(&target_eip);
    
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
