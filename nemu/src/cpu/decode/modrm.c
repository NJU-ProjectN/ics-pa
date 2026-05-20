#include "cpu/exec.h"
#include "cpu/rtl.h"

void load_addr(vaddr_t *eip, ModR_M *m, Operand *rm) {
  assert(m->mod != 3);

  int base_reg  = -1;
  int index_reg = -1;
  int scale     = 0;

  int disp_size = 0;
  int32_t disp  = 0;

  /*
   * Step 1:
   * parse ModR/M and SIB
   */

  if (m->R_M == R_ESP) {
    // has SIB
    SIB s;
    s.val = instr_fetch(eip, 1);

    scale = s.ss;

    // index = 4 means no index
    if (s.index != R_ESP) {
      index_reg = s.index;
    }

    /*
     * SIB special case:
     * mod = 0 && base = 5
     * => disp32 only, no base register
     */
    if (m->mod == 0 && s.base == R_EBP) {
      base_reg = -1;
      disp_size = 4;
    }
    else {
      base_reg = s.base;
    }
  }
  else {
    /*
     * normal ModR/M
     */
    if (m->mod == 0 && m->R_M == R_EBP) {
      // disp32 only
      base_reg = -1;
      disp_size = 4;
    }
    else {
      base_reg = m->R_M;
    }
  }

  /*
   * Step 2:
   * determine displacement size
   */

  if (m->mod == 1) {
    disp_size = 1;
  }
  else if (m->mod == 2) {
    disp_size = 4;
  }

  /*
   * Step 3:
   * fetch displacement
   */

  if (disp_size != 0) {
    if (disp_size == 1) {
      // 强迫 instr_fetch 出来的 uint32_t 先变成真正的有符号 8 位数，
      // 再由编译器安全地符号扩展到 32 位的 int32_t
      disp = (int8_t)instr_fetch(eip, 1);
    }
    else if (disp_size == 4) {
      disp = (int32_t)instr_fetch(eip, 4);
    }
  }

  /*
   * Step 4:
   * calculate effective address
   */

  rtl_li(&rm->addr, disp);

  if (base_reg != -1) {
    rtl_add(&rm->addr, &rm->addr, &reg_l(base_reg));
  }

  if (index_reg != -1) {
    rtlreg_t t;
    rtl_shli(&t, &reg_l(index_reg), scale);
    rtl_add(&rm->addr, &rm->addr, &t);
  }

#ifdef DEBUG
  char disp_buf[32]  = "";
  char base_buf[16]  = "";
  char index_buf[32] = "";

  if (disp_size != 0 || (base_reg == -1 && index_reg == -1)) {
    sprintf(disp_buf, "%#x", disp);
  }

  if (base_reg != -1) {
    sprintf(base_buf, "%%%s", reg_name(base_reg, 4));
  }

  if (index_reg != -1) {
    sprintf(index_buf,
            ",%%%s,%d",
            reg_name(index_reg, 4),
            1 << scale);
  }

  if (base_reg == -1 && index_reg == -1) {
    sprintf(rm->str, "%s", disp_buf);
  }
  else {
    sprintf(rm->str,
            "%s(%s%s)",
            disp_buf,
            base_buf,
            index_buf);
  }
#endif

  rm->type = OP_TYPE_MEM;
}

void read_ModR_M(vaddr_t *eip, Operand *rm, bool load_rm_val, Operand *reg, bool load_reg_val) {
  if (rm != NULL && rm->width == 0) rm->width = decoding.is_operand_size_16 ? 2 : 4;
  if (reg != NULL && reg->width == 0) reg->width = decoding.is_operand_size_16 ? 2 : 4;
  ModR_M m;
  m.val = instr_fetch(eip, 1);
  decoding.ext_opcode = m.opcode;
  if (reg != NULL) {
    reg->type = OP_TYPE_REG;
    reg->reg = m.reg;
    if (load_reg_val) {
      rtl_lr(&reg->val, reg->reg, reg->width);
    }

#ifdef DEBUG
    snprintf(reg->str, OP_STR_SIZE, "%%%s", reg_name(reg->reg, reg->width));
#endif
  }

  if (m.mod == 3) {
    rm->type = OP_TYPE_REG;
    rm->reg = m.R_M;
    if (load_rm_val) {
      rtl_lr(&rm->val, m.R_M, rm->width);
    }

#ifdef DEBUG
    sprintf(rm->str, "%%%s", reg_name(m.R_M, rm->width));
#endif
  }
  else {
    load_addr(eip, &m, rm);
    if (load_rm_val) {
      rtl_lm(&rm->val, &rm->addr, rm->width);
    }
  }
}
