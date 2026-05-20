#include "cpu/exec.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
rtl_add(&cpu.eip, &decoding.seq_eip, &id_dest->val);

  print_asm_template1(jmp);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint8_t subcode = decoding.opcode & 0xf;
  bool invert = subcode & 0x1;
  rtl_setcc(&t2, subcode >> 1);
  if (invert) {
    rtl_xori(&t2, &t2, 1); // 或者是 t2 = !t2; 
  }
  if (t2) {
    rtl_add(&cpu.eip, &decoding.seq_eip, &id_dest->val);
  }

  print_asm("j%s %x", get_cc_name(subcode >> 1), decoding.seq_eip + id_dest->val);
}

make_EHelper(jmp_rm) {
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // 1. 如果 AM 把栈初始化在了 1MB 以下的非法区域 (如 0x7c00)，依然帮它拉回安全区
  if (cpu.esp < 0x100000) {
    cpu.esp = 0x00800000;
  }

  vaddr_t next_ip = decoding.seq_eip;
  
  // 2. 现场读取 4 字节相对偏移量
  int32_t offset = instr_fetch(&next_ip, 4); 
  rtlreg_t return_addr = next_ip;

  // 3. 更新 ESP 寄存器
  cpu.esp -= 4;
  
  // 🟢 核心修正：严格按照 (地址, 长度, 数据) 的顺序传参！
  vaddr_write(cpu.esp, 4, return_addr); 

  // 4. 更新跳转状态
  decoding.jmp_eip = return_addr + offset;
  decoding.is_jmp = 1;
  decoding.seq_eip = next_ip;

  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  // 1. 从当前的栈顶稳稳读出返回地址
  rtlreg_t target_eip = vaddr_read(cpu.esp, 4);

  // 2. 栈指针恢复（Pop 动作）
  cpu.esp += 4;

  // 3. 🟢 暴力破局：直接强行改写 CPU 的 eip 和译码跟踪指针，不给框架任何留校查看的机会！
  cpu.eip = target_eip;
  decoding.jmp_eip = target_eip;
  decoding.is_jmp = 1;
  decoding.seq_eip = target_eip; // 全线锁死目标地址

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
