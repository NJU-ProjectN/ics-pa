#include "cpu/exec.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  decoding.is_jmp = 1;

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  decoding.is_jmp = t2;

  print_asm("j%s %x", get_cc_name(subcode), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
if (cpu.esp < 0x100000) {
    printf("[NEMU HOTFIX] Detected invalid ESP: 0x%08x. Force resetting ESP to safe zone 0x00800000!\n", cpu.esp);
    cpu.esp = 0x00800000;
  }

  printf("[FATAL DEBUG] Entering CALL. cpu.eip = 0x%08x, cpu.esp = 0x%08x\n", cpu.eip, cpu.esp);

  vaddr_t next_ip = decoding.seq_eip;
  
  // 1. 读取偏移量
  int32_t offset = instr_fetch(&next_ip, 4); 

  rtlreg_t return_addr = next_ip;

  // 2. 更新 ESP 并写入
  cpu.esp -= 4;
  printf("[FATAL DEBUG] Attempting to write return_addr(0x%08x) to stack ESP = 0x%08x\n", return_addr, cpu.esp);
  
  vaddr_write(cpu.esp, return_addr, 4); 
  printf("[FATAL DEBUG] Stack Write Success!\n");

  // 3. 更新跳转状态
  decoding.jmp_eip = return_addr + offset;
  decoding.is_jmp = 1;
  decoding.seq_eip = next_ip;

  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  printf("[DEBUG RET] Before Pop, Current ESP: 0x%08x, Memory Value at ESP: 0x%08x\n", 
          cpu.esp, vaddr_read(cpu.esp, 4));

  rtl_pop(&t0);
  printf("[DEBUG RET] After Pop, Popped Target EIP: 0x%08x\n", t0);
  decoding.jmp_eip = t0;
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
