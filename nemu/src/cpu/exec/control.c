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
  // the target address is calculated at the decode stage
  rtl_push(&decoding.jmp_eip);


  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  rtl_pop(&decoding.jmp_eip);
  decoding.is_jmp = 1;

  print_asm("ret");
}

make_EHelper(call_rm) {
  rtl_push(&decoding.jmp_eip);
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("call *%s", id_dest->str);
}

make_EHelper(ret_imm) {
  rtl_pop(&decoding.jmp_eip);
  rtl_addi(&cpu.esp, &cpu.esp, id_dest->val);
  decoding.is_jmp = 1;

  print_asm("ret %d", id_dest->val);
}

make_EHelper(xor_rm2r) {
  rtl_xor(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);

  print_asm("xor %s, %s", id_dest->str, id_src->str);
}

make_EHelper(xor_i2rm) {
  rtl_xor(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);

  print_asm("xor %s, %s", id_dest->str, id_src->str);
}

make_EHelper(push) {
  rtl_push(&id_dest->val);

  print_asm("push %s", id_dest->str);
}

make_EHelper(pop) {
  rtl_pop(&t2);
  operand_write(id_dest, &t2);

  print_asm("pop %s", id_dest->str);
}

make_EHelper(sub_rm2r) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);

  print_asm("sub %s, %s", id_dest->str, id_src->str);
}

make_EHelper(sub_i2rm) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);

  print_asm("sub %s, %s", id_dest->str, id_src->str);
}

