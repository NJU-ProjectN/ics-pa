#include "cpu/exec.h"

make_EHelper(mov) {
  rtl_mv(&t2, &id_src->val);
  operand_write(id_dest, &t2);
  
  print_asm_template2(mov);
}

make_EHelper(push) {
    rtlreg_t val;

    if (id_src->type == OP_TYPE_MEM) {
        rtl_lm(&val, &id_src->addr, id_src->width);
    }
    else {
        val = id_src->val;
    }

    rtl_push(&val);

    print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&t0);
  operand_write(id_dest, &t0);

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  if (decoding.is_operand_size_16) {
    uint16_t temp_esp = cpu.esp;
    
    rtl_lr_w(&t0, R_AX);  rtl_push(&t0);
    rtl_lr_w(&t0, R_CX);  rtl_push(&t0);
    rtl_lr_w(&t0, R_DX);  rtl_push(&t0);
    rtl_lr_w(&t0, R_BX);  rtl_push(&t0);
    
    t0 = temp_esp;        rtl_push(&t0); // 压入旧的 esp
    
    rtl_lr_w(&t0, R_BP);  rtl_push(&t0);
    rtl_lr_w(&t0, R_SI);  rtl_push(&t0);
    rtl_lr_w(&t0, R_DI);  rtl_push(&t0);
  } else {
    uint32_t temp_esp = cpu.esp;
    rtl_push(&cpu.eax);
    rtl_push(&cpu.ecx);
    rtl_push(&cpu.edx);
    rtl_push(&cpu.ebx);
    rtl_push(&temp_esp);
    rtl_push(&cpu.ebp);
    rtl_push(&cpu.esi);
    rtl_push(&cpu.edi);
  }

  print_asm("pusha");
}

make_EHelper(popa) {
  if (decoding.is_operand_size_16) {
    rtl_pop(&t0); rtl_sr_w(R_DI, &t0);
    rtl_pop(&t0); rtl_sr_w(R_SI, &t0);
    rtl_pop(&t0); rtl_sr_w(R_BP, &t0);
    
    rtl_pop(&t0); 
    
    rtl_pop(&t0); rtl_sr_w(R_BX, &t0);
    rtl_pop(&t0); rtl_sr_w(R_DX, &t0);
    rtl_pop(&t0); rtl_sr_w(R_CX, &t0);
    rtl_pop(&t0); rtl_sr_w(R_AX, &t0);
  } else {
    rtl_pop(&cpu.edi);
    rtl_pop(&cpu.esi);
    rtl_pop(&cpu.ebp);
    rtl_pop(&t0);
    rtl_pop(&cpu.ebx);
    rtl_pop(&cpu.edx);
    rtl_pop(&cpu.ecx);
    rtl_pop(&cpu.eax);
  }

  print_asm("popa");
}

make_EHelper(leave) {
  rtl_mv(&cpu.esp, &cpu.ebp);
  rtl_pop(&cpu.ebp);

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    rtl_lr_w(&t0, R_AX);
    rtl_msb(&t1, &t0, 2); 
    if (t1) {
      t2 = 0xffff;
      rtl_sr_w(R_DX, &t2);
    } else {
      t2 = 0;
      rtl_sr_w(R_DX, &t2);
    }
  }
  else {
   rtl_lr_l(&t0, R_EAX);
    rtl_msb(&t1, &t0, 4); 
    if (t1) {
      t2 = 0xffffffff;
      rtl_sr_l(R_EDX, &t2);
    } else {
      t2 = 0;
      rtl_sr_l(R_EDX, &t2);
    }
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
  rtl_lr_b(&t0, R_AL);
  rtl_sext(&t0, &t0, 1);
  rtl_sr_w(R_AX, &t0);
  }
  else {
  rtl_lr_w(&t0, R_AX);
  rtl_sext(&t0, &t0, 2);
  rtl_sr_l(R_EAX, &t0);
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

  uint32_t val = id_src->val;
  if (id_src->width == 1) {
    val = (uint8_t)val;  
  } else if (id_src->width == 2) {
    val = (uint16_t)val; 
  }

  operand_write(id_dest, &val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  printf("[DEBUG LEA] addr: 0x%x\n", id_src->addr);
  operand_write(id_dest, &id_src->addr);
  
  print_asm_template2(lea);
}
