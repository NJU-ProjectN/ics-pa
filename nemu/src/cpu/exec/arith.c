#include "cpu/exec.h"

make_EHelper(add) {
  rtl_add(&t2, &id_dest->val, &id_src->val);
  rtl_sltu(&t3, &t2, &id_dest->val);

  rtl_update_ZFSF(&t2, id_dest->width);
  rtl_set_CF(&t3);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_not(&t0);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  // 先算标志位，最后写回
  operand_write(id_dest, &t2);
  print_asm_template2(add);
}

make_EHelper(sub) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  rtl_sltu(&t3, &id_dest->val, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);
  rtl_set_CF(&t3);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  operand_write(id_dest, &t2);
  print_asm_template2(sub);
}

make_EHelper(cmp) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  rtl_sltu(&t3, &id_dest->val, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);
  rtl_set_CF(&t3);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  // cmp 不写回目标寄存器
  print_asm_template2(cmp);
}

make_EHelper(inc) {
  rtl_addi(&t2, &id_dest->val, 1);

  rtl_update_ZFSF(&t2, id_dest->width);
  // 注意：x86 规定 inc 指令不影响 CF，所以这里删除了所有 CF 的处理

  rtl_xori(&t0, &id_dest->val, 1);
  rtl_not(&t0);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  operand_write(id_dest, &t2);
  print_asm_template1(inc);
}

make_EHelper(dec) {
  rtl_subi(&t2, &id_dest->val, 1);

  rtl_update_ZFSF(&t2, id_dest->width);
  // 注意：x86 规定 dec 指令不影响 CF

  rtl_xori(&t0, &id_dest->val, 1);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  operand_write(id_dest, &t2);
  print_asm_template1(dec);
}

make_EHelper(neg) {
  // CF: 如果 dest 为 0 则 CF=0, 否则 CF=1
  rtl_eq0(&t0, &id_dest->val);
  rtl_xori(&t0, &t0, 1); // 0变1, 1变0
  rtl_set_CF(&t0);

  rtl_sub(&t2, &tzero, &id_dest->val);

  rtl_update_ZFSF(&t2, id_dest->width);

  // 计算 OF: 只有最小负数求补码才会溢出 (0 - dest)
  rtl_xor(&t0, &tzero, &id_dest->val);
  rtl_xor(&t1, &tzero, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  operand_write(id_dest, &t2);
  print_asm_template1(neg);
}

make_EHelper(adc) {
  rtl_add(&t2, &id_dest->val, &id_src->val);
  rtl_sltu(&t3, &t2, &id_dest->val); // 第一次加法的进位
  
  rtl_get_CF(&t1);
  rtl_add(&t0, &t2, &t1); // t0 存放最终结果
  
  rtl_sltu(&t1, &t0, &t2); // 第二次加法的进位基准应该是 t2
  rtl_or(&t3, &t3, &t1);
  rtl_set_CF(&t3);

  rtl_update_ZFSF(&t0, id_dest->width);

  rtl_xor(&t3, &id_dest->val, &id_src->val);
  rtl_not(&t3);
  rtl_xor(&t1, &id_dest->val, &t0);
  rtl_and(&t3, &t3, &t1);
  rtl_msb(&t3, &t3, id_dest->width);
  rtl_set_OF(&t3);

  operand_write(id_dest, &t0);
  print_asm_template2(adc);
}

make_EHelper(sbb) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  rtl_sltu(&t3, &id_dest->val, &t2); // 第一次减法的借位
  
  rtl_get_CF(&t1);
  rtl_sub(&t0, &t2, &t1); // t0 存放最终结果
  
  rtl_sltu(&t1, &t2, &t0); // 第二次减法的借位基准应该是 t2
  rtl_or(&t3, &t3, &t1);
  rtl_set_CF(&t3);

  rtl_update_ZFSF(&t0, id_dest->width);

  rtl_xor(&t3, &id_dest->val, &id_src->val);
  rtl_xor(&t1, &id_dest->val, &t0);
  rtl_and(&t3, &t3, &t1);
  rtl_msb(&t3, &t3, id_dest->width);
  rtl_set_OF(&t3);

  operand_write(id_dest, &t0);
  print_asm_template2(sbb);
}

make_EHelper(mul) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_mul(&t0, &t1, &id_dest->val, &t0); // t0是低位, t1是高位

  switch (id_dest->width) {
    case 1:
      rtl_sr_w(R_AX, &t0); // 1字节乘法结果在 AX 中
      break;
    case 2:
      rtl_sr_w(R_AX, &t0);
      rtl_sr_w(R_DX, &t1);
      break;
    case 4:
      rtl_sr_l(R_EAX, &t0); // 低位进 EAX
      rtl_sr_l(R_EDX, &t1); // 高位进 EDX （修复了颠倒的问题）
      break;
    default: assert(0);
  }

  print_asm_template1(mul);
}

make_EHelper(imul1) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_imul(&t0, &t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr_w(R_AX, &t0);
      break;
    case 2:
      rtl_sr_w(R_AX, &t0);
      rtl_sr_w(R_DX, &t1);
      break;
    case 4:
      rtl_sr_l(R_EAX, &t0); // 修复了颠倒的问题
      rtl_sr_l(R_EDX, &t1);
      break;
    default: assert(0);
  }

  print_asm_template1(imul);
}

make_EHelper(imul2) {
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_imul(&t0, &t1, &id_dest->val, &id_src->val); // t0为低位
  operand_write(id_dest, &t0);                     // 2操作数和3操作数只取低位结果

  print_asm_template2(imul);
}

make_EHelper(imul3) {
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_src2->val, &id_src2->val, id_src2->width);

  rtl_imul(&t0, &t1, &id_src2->val, &id_src->val);
  operand_write(id_dest, &t0);

  print_asm_template3(imul);
}

make_EHelper(div) {
  switch (id_dest->width) {
    case 1:
      rtl_lr_w(&t0, R_AX);   // 被除数在 AX
      rtl_li(&t1, 0);        // 高位清零
      break;
    case 2:
      rtl_lr_w(&t0, R_AX);
      rtl_lr_w(&t1, R_DX);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);  // 拼接成 32 位被除数在 t0
      rtl_li(&t1, 0);        // 传给 rtl_div 的高位部分清零
      break;
    case 4:
      rtl_lr_l(&t0, R_EAX);  // 低 32 位在 EAX
      rtl_lr_l(&t1, R_EDX);  // 高 32 位在 EDX
      break;
    default: assert(0);
  }

  rtl_div(&t2, &t3, &t1, &t0, &id_dest->val); // t2商, t3余数

  rtl_sr(R_EAX, id_dest->width, &t2);
  if (id_dest->width == 1) {
    rtl_sr_b(R_AH, &t3);
  } else {
    rtl_sr(R_EDX, id_dest->width, &t3);
  }

  print_asm_template1(div);
}

make_EHelper(idiv) {
  switch (id_dest->width) {
    case 1:
      rtl_lr_w(&t0, R_AX);
      rtl_sext(&t0, &t0, 2); // 将 AX 符号扩展到 32 位存入 t0
      rtl_li(&t1, 0);        // 此时被除数已完整在 t0，高位传 0 即可
      break;
    case 2:
      rtl_lr_w(&t0, R_AX);
      rtl_lr_w(&t1, R_DX);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);  // 拼接 DX:AX 到 t0
      rtl_li(&t1, 0);        // 传给 rtl_idiv 的高位传 0 
      break;
    case 4:
      rtl_lr_l(&t0, R_EAX);  // 有符号 64 位除法，低位 EAX
      rtl_lr_l(&t1, R_EDX);  // 高位 EDX
      break;
    default: assert(0);
  }

  rtl_idiv(&t2, &t3, &t1, &t0, &id_dest->val);

  rtl_sr(R_EAX, id_dest->width, &t2);
  if (id_dest->width == 1) {
    rtl_sr_b(R_AH, &t3);
  } else {
    rtl_sr(R_EDX, id_dest->width, &t3);
  }

  print_asm_template1(idiv);
}