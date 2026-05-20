#include "cpu/exec.h"

make_EHelper(add) {
  // 1. 用独立且绝对安全的局部 C 变量把最原始的源数据、目的数据和结果扣下来
  // 这样无论底层 RTL 宏怎么倒腾，我们的原始数值绝对不会变！
  uint32_t original_dest = id_dest->val;
  uint32_t original_src = id_src->val;

  // 2. 执行核心加法
  rtl_add(&t2, &id_dest->val, &id_src->val);
  
  // 3. 立即用局部变量接住相加后的真实结果，防止被后面的写回操作干扰
  uint32_t result = t2; 

  // 4. 将结果安全写回寄存器或内存
  operand_write(id_dest, &t2);

  // 5. 刷新 ZF 和 SF
  rtl_update_ZFSF(&result, id_dest->width);

  // 6. 用最纯粹、没有 RTL 指针冲突的纯逻辑计算 CF 和 OF
  // 计算 CF: 如果结果小于任何一个原始加数，说明无符号溢出
  rtlreg_t cf_val = (result < original_dest) ? 1 : 0;
  rtl_set_CF(&cf_val);

  // 计算 OF: 如果 (src1 和 src2 符号相同) 且 (结果与 src1 符号不同)，说明有符号溢出
  // 我们直接在 C 语言层面完成这个位运算，绕开所有垃圾中间寄存器
  uint32_t of_val = ((original_dest ^ original_src) == 0) && ((original_dest ^ result) < 0);
  
  // 如果上面的位运算觉得不直观，可以用更标准的 x86 传统判定法：
  // of_val = ((original_dest ^ result) & (original_src ^ result)) >> (id_dest->width * 8 - 1);
  rtlreg_t of_reg = (of_val & 1);
  rtl_set_OF(&of_reg);

  print_asm_template2(add);
}

make_EHelper(sub) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  
  // 1. 严格在写回前算好所有的标志位，防止 destination 被覆盖
  rtl_update_ZFSF(&t2, id_dest->width);
  
  // CF: 始终通过比较源操作数的大小来判定无符号借位
  rtl_sltu(&t3, &id_dest->val, &id_src->val);
  rtl_set_CF(&t3);
  
  // OF: 减法溢出标志标准 RTL 运算
  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  // 2. 标志位锁死后，再执行写回
  operand_write(id_dest, &t2);

  print_asm_template2(sub);
}

make_EHelper(cmp) {
  // 1. 提取原始值
  uint32_t dest_val = id_dest->val;
  uint32_t src_val  = id_src->val;

  // 💥【核心清洗】：在计算任何标志位之前，根据位宽把 dest 和 src 的高位全部清零！
  uint32_t mask = 0xffffffff;
  if (id_dest->width == 1)      mask = 0xff;
  else if (id_dest->width == 2) mask = 0xffff;

  dest_val &= mask;
  src_val  &= mask;

  // 2. 用清洗后的干净数据进行 C 语言原生计算
  uint32_t res_val = (dest_val - src_val) & mask;

  // 3. 将干净的差值送入 t2，更新 ZF 和 SF
  rtl_li(&t2, res_val);
  rtl_update_ZFSF(&t2, id_dest->width);
  
  // 4. CF (无符号借位): 此时 dest_val 和 src_val 已经完美截断，直接比较绝无高位干扰
  uint32_t cf_bit = (dest_val < src_val) ? 1 : 0;
  rtl_li(&t3, cf_bit);
  rtl_set_CF(&t3);
  
  // 5. OF (有符号溢出判断): 
  // 减法溢出：(dest ^ src) & (dest ^ res)
  uint32_t overflow = (dest_val ^ src_val) & (dest_val ^ res_val);
  uint32_t of_bit = 0;

  // 根据位宽精确提取对应的最高符号位
  if (id_dest->width == 1)      of_bit = (overflow >> 7) & 0x1;
  else if (id_dest->width == 2) of_bit = (overflow >> 15) & 0x1;
  else if (id_dest->width == 4) of_bit = (overflow >> 31) & 0x1;

  rtl_li(&t0, of_bit);
  rtl_set_OF(&t0);

  print_asm_template2(cmp);
}

make_EHelper(inc) {
  rtl_mv(&t1, &id_dest->val);
  rtl_addi(&t2, &t1, 1);
  rtl_mv(&t0, &t1);
  rtl_not(&t0);
  rtl_and(&t0, &t0, &t2);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  rtl_update_ZF(&t2, id_dest->width);
  rtl_update_SF(&t2, id_dest->width);

  operand_write(id_dest, &t2);
  print_asm_template1(inc);
}

make_EHelper(dec) {
rtl_mv(&t1, &id_dest->val);

  rtl_subi(&t2, &t1, 1);

  rtl_mv(&t0, &t2);
  rtl_not(&t0);
  rtl_and(&t0, &t1, &t0);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  rtl_update_ZF(&t2, id_dest->width);
  rtl_update_SF(&t2, id_dest->width);

  operand_write(id_dest, &t2);
  print_asm_template1(dec);
}

make_EHelper(neg) {
  rtl_sub(&t2, &tzero, &id_dest->val);
  operand_write(id_dest, &t2);

  print_asm_template1(neg);
}

make_EHelper(adc) {
  // 1. 获取当前的进位 CF
  rtl_get_CF(&t1); // t1 = CF

  // 2. 第一阶段加法：t2 = dest + src
  rtl_add(&t2, &id_dest->val, &id_src->val);
  // 检查第一阶段是否溢出/进位
  rtl_sltu(&t3, &t2, &id_dest->val); // t3 = (t2 < dest) ? 1 : 0 (第一阶段的进位)

  // 3. 第二阶段加法：把 CF 加进去，得到最终结果存入 t2
  rtl_add(&t2, &t2, &t1);
  // 检查第二阶段是否由于加了 CF 再次产生进位
  rtl_sltu(&t0, &t2, &t1); 

  // 4. 将最终结果写入目标寄存器或内存
  operand_write(id_dest, &t2);

  // 5. 更新零标志位（ZF）和符号标志位（SF）
  rtl_update_ZFSF(&t2, id_dest->width);

  // 6. 更新进位标志位（CF）：两阶段任何一阶段产生进位，最终 CF 就是 1
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  // 7. 更新溢出标志位（OF）：两源操作数符号相同，但与最终结果符号不同，则溢出
  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_not(&t0); // 如果 dest 和 src 符号相同，t0 的最高位为 1
  rtl_xor(&t1, &id_dest->val, &t2); // 如果 dest 和 最终结果 符号不同，t1 的最高位为 1
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width); // 取出最高位
  rtl_set_OF(&t0);

  print_asm_template2(adc);
}

make_EHelper(sbb) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  rtl_sltu(&t3, &id_dest->val, &t2);
  rtl_get_CF(&t1);
  rtl_sub(&t2, &t2, &t1);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_sltu(&t0, &id_dest->val, &t2);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(sbb);
}

make_EHelper(mul) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_mul(&t0, &t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr_w(R_AX, &t1);
      break;
    case 2:
      rtl_sr_w(R_AX, &t1);
      rtl_shri(&t1, &t1, 16);
      rtl_sr_w(R_DX, &t1);
      break;
    case 4:
      rtl_sr_l(R_EDX, &t1);
      rtl_sr_l(R_EAX, &t0);
      break;
    default: assert(0);
  }

  print_asm_template1(mul);
}
make_EHelper(imul) {
  // 1. 拿到目标操作数（dest，这里是 %edx）和源操作数（src，这里是 %ecx）的值
  // 2. 直接做 32 位乘法（C 语言会自动做低位截断）
  uint32_t result = id_dest->val * id_src->val;

  // 3. 将结果写回目标寄存器（dest）
  operand_write(id_dest, &result);

  print_asm_template2(imul);
}

// imul with one operand
make_EHelper(imul1) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_imul(&t0, &t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr_w(R_AX, &t1);
      break;
    case 2:
      rtl_sr_w(R_AX, &t1);
      rtl_shri(&t1, &t1, 16);
      rtl_sr_w(R_DX, &t1);
      break;
    case 4:
      rtl_sr_l(R_EDX, &t1); 
      rtl_sr_l(R_EAX, &t0);
      break;
    default: assert(0);
  }

  print_asm_template1(imul);
}

// imul with two operands
make_EHelper(imul2) {
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_imul(&t0, &t1, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);

  print_asm_template2(imul);
}

// imul with three operands
make_EHelper(imul3) {
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_src2->val, &id_src2->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_imul(&t0, &t1, &id_src2->val, &id_src->val);
  operand_write(id_dest, &t0);

  print_asm_template3(imul);
}

make_EHelper(div) {
  switch (id_dest->width) {
    case 1:
      rtl_li(&t1, 0);
      rtl_lr_w(&t0, R_AX);
      break;
    case 2:
      rtl_lr_w(&t0, R_AX);
      rtl_lr_w(&t1, R_DX);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_li(&t1, 0);
      break;
    case 4:
      rtl_lr_l(&t0, R_EAX);
      rtl_lr_l(&t1, R_EDX);
      break;
    default: assert(0);
  }

  rtl_div(&t2, &t3, &t1, &t0, &id_dest->val);

  rtl_sr(R_EAX, id_dest->width, &t2);
  if (id_dest->width == 1) {
    rtl_sr_b(R_AH, &t3);
  }
  else {
    rtl_sr(R_EDX, id_dest->width, &t3);
  }

  print_asm_template1(div);
}

make_EHelper(idiv) {
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  switch (id_dest->width) {
    case 1:
      rtl_lr_w(&t0, R_AX);
      rtl_sext(&t0, &t0, 2);
      rtl_msb(&t1, &t0, 4);
      rtl_sub(&t1, &tzero, &t1);
      break;
    case 2:
      rtl_lr_w(&t0, R_AX);
      rtl_lr_w(&t1, R_DX);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_msb(&t1, &t0, 4);
      rtl_sub(&t1, &tzero, &t1);
      break;
    case 4:
      rtl_lr_l(&t0, R_EAX);
      rtl_lr_l(&t1, R_EDX);
      break;
    default: assert(0);
  }

  rtl_idiv(&t2, &t3, &t1, &t0, &id_dest->val);

  rtl_sr(R_EAX, id_dest->width, &t2);
  if (id_dest->width == 1) {
    rtl_sr_b(R_AH, &t3);
  }
  else {
    rtl_sr(R_EDX, id_dest->width, &t3);
  }

  print_asm_template1(idiv);
}
