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
    // push 返回地址
    printf("DEBUG: Call: pushing ret addr 0x%08x\n", decoding.seq_eip);
    int32_t offset = (int32_t)id_dest->val;
    // seq_eip 应该是 call 指令的下一条指令地址 (0x10000f)
    // 目标地址应该是 0x10000f + offset = 0x10001c
    printf("DEBUG: CALL calc: seq_eip=0x%08x, offset=0x%08x, target=0x%08x\n", 
            decoding.seq_eip, offset, decoding.seq_eip + offset);
    rtlreg_t ret_addr = decoding.seq_eip; 
    rtl_push(&ret_addr);

    decoding.jmp_eip = decoding.seq_eip + (int32_t)id_dest->val;
    decoding.is_jmp = 1;

    print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
    rtlreg_t target_eip;
    rtl_pop(&target_eip);   // pop 返回地址
    printf("[DEBUG] RET: Popping target_eip = 0x%08x, current_esp = 0x%08x\n", target_eip, cpu.esp);
    
    // 如果弹出的值不符合代码段范围（通常是 0x00100000 左右），直接报错
    if (target_eip < 0x00100000) { 
        panic("RET 弹出了非法地址！检查谁压入了该值");
    }
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
