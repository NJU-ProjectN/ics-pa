#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  // x86 规范：push 1字节立即数时需要先对其进行符号扩展
  // 在 NEMU 译码中，单个操作数的指令其源数据通常放在 id_dest 中
  if (id_dest->type == OP_TYPE_IMM && id_dest->width == 1) {
    rtl_sext(&t2, &id_dest->val, 1);
    rtl_push(&t2);
  } else {
    rtl_push(&id_dest->val);
  }

  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&t2);
  operand_write(id_dest, &t2);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  if (decoding.is_operand_size_16) { assert(0); }
  
  rtl_mv(&t1, &cpu.esp); // 安全备份当时的 ESP 到临时寄存器 t1
  rtl_push(&cpu.eax);
  rtl_push(&cpu.ecx);
  rtl_push(&cpu.edx);
  rtl_push(&cpu.ebx);
  rtl_push(&t1);        // 压入备份的 ESP
  rtl_push(&cpu.ebp);
  rtl_push(&cpu.esi);
  rtl_push(&cpu.edi);

  print_asm("pusha");
}

make_EHelper(popa) {
  if (decoding.is_operand_size_16) { assert(0); }
  
  rtl_pop(&cpu.edi);
  rtl_pop(&cpu.esi);
  rtl_pop(&cpu.ebp);
  rtl_pop(&t1);        // 弹出原本的 ESP 丢弃到临时寄存器，不覆盖当前的 ESP
  rtl_pop(&cpu.ebx);
  rtl_pop(&cpu.edx);
  rtl_pop(&cpu.ecx);
  rtl_pop(&cpu.eax);

  print_asm("popa");
}

make_EHelper(leave) {
  rtl_mv(&reg_l(R_ESP), &reg_l(R_EBP));
  rtl_pop(&reg_l(R_EBP));
  
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    rtl_sext(&t0, &reg_l(R_EAX), 2);
    rtl_shri(&reg_l(R_EDX), &t0, 16);
  } else {
    rtl_sari(&reg_l(R_EDX), &reg_l(R_EAX), 31);
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    rtl_shli(&reg_l(R_EAX), &reg_l(R_EAX), 24);
    rtl_sari(&reg_l(R_EAX), &reg_l(R_EAX), 8);
    rtl_shri(&reg_l(R_EAX), &reg_l(R_EAX), 16);
  } else {
    rtl_sext(&reg_l(R_EAX), &reg_l(R_EAX), 2);
  }
  
  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t2, &id_src->val, id_src->width);
  operand_write(id_dest, &t2);
  
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  // 借助 t2 中转限制符号宽度，清除源操作数高位残留的脏数据
  rtl_li(&t2, id_src->val); 
  operand_write(id_dest, &t2);
  
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  // lea 获取的是源操作数的有效地址(addr)，而不是其指向的内容(val)
  rtl_li(&t2, id_src->addr);
  operand_write(id_dest, &t2);
  
  print_asm_template2(lea);
}