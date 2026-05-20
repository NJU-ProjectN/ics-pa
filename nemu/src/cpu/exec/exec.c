#include "cpu/exec.h"
#include "all-instr.h"

make_EHelper(call);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(ret);
make_EHelper(sub);
make_EHelper(xor);
make_EHelper(cmp);
make_EHelper(lea);
make_EHelper(add);
make_EHelper(and);
make_EHelper(or);
make_EHelper(test);
make_EHelper(inc);
make_EHelper(dec);
make_EHelper(neg);
make_EHelper(adc);
make_EHelper(sbb);
make_EHelper(mul);
make_EHelper(imul);
make_EHelper(nop);
make_EHelper(setcc);
make_EHelper(jcc);
make_EHelper(jmp);
make_EHelper(jmp_rm);
make_EHelper(call_rm);
make_EHelper(not);
make_EHelper(movzx);
make_EHelper(shl);
make_EHelper(shr);
make_EHelper(sar);

typedef struct {
  DHelper decode;
  EHelper execute;
  int width;
} opcode_entry;

#define IDEXW(id, ex, w)   {concat(decode_, id), concat(exec_, ex), w}
#define IDEX(id, ex)       IDEXW(id, ex, 0)
#define EXW(ex, w)         {NULL, concat(exec_, ex), w}
#define EX(ex)             EXW(ex, 0)
#define EMPTY              EX(inv)

static inline void set_width(int width) {
  if (width == 0) {
    width = decoding.is_operand_size_16 ? 2 : 4;
  }
  decoding.src.width = decoding.dest.width = decoding.src2.width = width;
}

/* Instruction Decode and EXecute */
static inline void idex(vaddr_t *eip, opcode_entry *e) {
  /* eip is pointing to the byte next to opcode */
  if (e->decode)
    e->decode(eip);
  e->execute(eip);
}

static make_EHelper(2byte_esc);

#define make_group(name, item0, item1, item2, item3, item4, item5, item6, item7) \
  static opcode_entry concat(opcode_table_, name) [8] = { \
    /* 0x00 */	item0, item1, item2, item3, \
    /* 0x04 */	item4, item5, item6, item7  \
  }; \
static make_EHelper(name) { \
  idex(eip, &concat(opcode_table_, name)[decoding.ext_opcode]); \
}

/* 0x80, 0x81, 0x83 */
make_group(gp1,
    EX(add), EMPTY, EMPTY, EMPTY,
    EX(and), EX(sub), EX(xor), EX(cmp))

  /* 0xc0, 0xc1, 0xd0, 0xd1, 0xd2, 0xd3 */
make_group(gp2,
    EMPTY, EMPTY, EMPTY, EMPTY,
    EX(shl), EX(shr), EMPTY, EX(sar))

  /* 0xf6, 0xf7 */
make_group(gp3,
    EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY)

  /* 0xfe */
make_group(gp4,
    EMPTY, EX(dec), EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY)

  /* 0xff */
make_group(gp5,
    EX(inc), EX(dec), EMPTY, EMPTY,
    EMPTY, EMPTY, EX(push), EMPTY)

  /* 0x0f 0x01*/
make_group(gp7,
    EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY)

/* TODO: Add more instructions!!! */

opcode_entry opcode_table [512] = {
  /* 0x00 */	EMPTY, EMPTY, EMPTY, IDEX(E2G, add),
  /* 0x04 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x08 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x0c */	EMPTY, EMPTY, EMPTY, EX(2byte_esc),
  /* 0x10 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x14 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x18 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x1c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x20 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x24 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x28 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x2c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x30 */   EMPTY, IDEX(E2G, xor), EMPTY, EMPTY,
  /* 0x34 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x38 */	EMPTY, EMPTY, EMPTY, IDEX(G2E, cmp),
  /* 0x3c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x40 */   IDEX(r, inc), IDEX(r, inc), IDEX(r, inc), IDEX(r, inc),
  /* 0x44 */   IDEX(r, inc), IDEX(r, inc), IDEX(r, inc), IDEX(r, inc),
  /* 0x48 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x4c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x50 */ IDEX(r, push), IDEX(r, push), IDEX(r, push), IDEX(r, push),
  /* 0x54 */ IDEX(r, push), IDEX(r, push), IDEX(r, push), IDEX(r, push),
  /* 0x58 */ IDEX(r, pop),  IDEX(r, pop),  IDEX(r, pop),  IDEX(r, pop),
  /* 0x5c */ IDEX(r, pop),  IDEX(r, pop),  IDEX(r, pop),  IDEX(r, pop),
  /* 0x60 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x64 */	EMPTY, EMPTY, EX(operand_size), EMPTY,
  /* 0x68 */   IDEX(I, push), EMPTY, IDEXW(I, push, 1), EMPTY,
  /* 0x6c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x70 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x74 */	IDEXW(J, jcc, 1), IDEXW(J, jcc, 1), EMPTY, EMPTY,
  /* 0x78 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x7c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x80 */	IDEXW(I2E, gp1, 1), IDEX(I2E, gp1), EMPTY, IDEX(SI2E, gp1),
  /* 0x84 */	IDEX(G2E, test), IDEX(G2E, test), EMPTY, EMPTY,
  /* 0x88 */	IDEXW(mov_G2E, mov, 1), IDEX(mov_G2E, mov), IDEXW(mov_E2G, mov, 1), IDEX(G2E, mov),
  /* 0x8c */	EMPTY, IDEX(E2G, lea), EMPTY, EMPTY,
  /* 0x90 */	IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc),
  /* 0x94 */	IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc),
  /* 0x98 */	IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc),
  /* 0x9c */	IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc),
  /* 0xa0 */	IDEXW(O2a, mov, 1), IDEX(O2a, mov), IDEXW(a2O, mov, 1), IDEX(a2O, mov),
  /* 0xa4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xa8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xac */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xb0 */	IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1),
  /* 0xb4 */	IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1),
  /* 0xb8 */	IDEX(mov_I2r, mov), IDEX(mov_I2r, mov), IDEX(mov_I2r, mov), IDEX(mov_I2r, mov),
  /* 0xbc */	IDEX(mov_I2r, mov), IDEX(mov_I2r, mov), IDEX(mov_I2r, mov), IDEX(mov_I2r, mov),
  /* 0xc0 */ IDEXW(gp2_Ib2E, gp2, 1), IDEX(gp2_Ib2E, gp2), EMPTY, EX(ret),
  /* 0xc4 */	EMPTY, EMPTY, IDEXW(mov_I2E, mov, 1), IDEX(I2E, mov),
  /* 0xc8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xcc */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xd0 */	IDEXW(gp2_1_E, gp2, 1), IDEX(gp2_1_E, gp2), IDEXW(gp2_cl2E, gp2, 1), IDEX(gp2_cl2E, gp2),
  /* 0xd4 */	EMPTY, EMPTY, EX(nemu_trap), EMPTY,
  /* 0xd8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xdc */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe8 */	IDEXW(J, call, 4), IDEXW(J, jmp, 4), EMPTY, EMPTY,
  /* 0xec */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xf0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xf4 */	EMPTY, EMPTY, IDEXW(E, gp3, 1), IDEX(E, gp3),
  /* 0xf8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xfc */	EMPTY, EMPTY, IDEXW(E, gp4, 1), IDEX(E, gp5),

  /*2 byte_opcode_table */

  /* 0x00 */	EMPTY, IDEX(gp7_E, gp7), EMPTY, EMPTY,
  /* 0x04 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x08 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x0c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x10 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x14 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x18 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x1c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x20 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x24 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x28 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x2c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x30 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x34 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x38 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x3c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x40 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x44 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x48 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x4c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x50 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x54 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x58 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x5c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x60 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x64 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x68 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x6c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x70 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x74 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x78 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x7c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x80 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x84 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x88 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x8c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x90 */	IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc),
  /* 0x94 */	IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc),
  /* 0x98 */	IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc),
  /* 0x9c */	IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc), IDEX(E, setcc),
  /* 0xa0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xa4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xa8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xac */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xb0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xb4 */	EMPTY, EMPTY, IDEX(E2G, movzx), EMPTY,
  /* 0xb8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xbc */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xc0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xc4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xc8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xcc */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xd0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xd4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xd8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xdc */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xec */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xf0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xf4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xf8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xfc */	EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(2byte_esc) {
  uint32_t opcode = instr_fetch(eip, 1) | 0x100;
  //printf("2-byte Opcode is: 0x%02x\n", opcode);
  decoding.opcode = opcode;
  set_width(opcode_table[opcode].width);
  idex(eip, &opcode_table[opcode]);
}

make_EHelper(real) {
  vaddr_t temp_eip = *eip;
  uint8_t first_byte = instr_fetch(eip, 1); 
  *eip = temp_eip;

  uint32_t opcode = instr_fetch(eip, 1);
  decoding.opcode = opcode; 
  set_width(opcode_table[opcode].width);
  idex(eip, &opcode_table[opcode]);

  printf("[LEN_CHECK] EIP: 0x%08x | Opcode: 0x%02x | Next_EIP: 0x%08x | Advanced: %d bytes\n", 
       temp_eip, first_byte, *eip, (int)(*eip - temp_eip));
}

static inline void update_eip(void) {
  if (decoding.is_jmp) {
    cpu.eip = decoding.jmp_eip;
  } else {
    cpu.eip = decoding.seq_eip;
  }
}

void exec_wrapper(bool print_flag) {
  
  decoding.is_jmp = 0;
  decoding.jmp_eip = 0;

#ifdef DEBUG
  decoding.p = decoding.asm_buf;
  decoding.p += sprintf(decoding.p, "%8x:   ", cpu.eip);
#endif
  decoding.seq_eip = cpu.eip;
  exec_real(&decoding.seq_eip);

#ifdef DEBUG
  int instr_len = decoding.seq_eip - cpu.eip;
  sprintf(decoding.p, "%*.s", 50 - (12 + 3 * instr_len), "");
  strncat(decoding.asm_buf, decoding.assembly, 80);
  Log_write("%s\n", decoding.asm_buf);
  if (print_flag) {
    puts(decoding.asm_buf);
  }
#endif

#ifdef DIFF_TEST
  uint32_t eip = cpu.eip;
#endif
#ifdef DEBUG
  printf("[TRACE] EIP = 0x%08x, ESP = 0x%08x, ASM: %s\n", 
          decoding.is_jmp ? decoding.jmp_eip : decoding.seq_eip, cpu.esp, decoding.assembly);
  #else
  printf("[TRACE] EIP = 0x%08x, ESP = 0x%08x\n", cpu.eip, cpu.esp);
  #endif
  update_eip();
  
#ifdef DIFF_TEST
  void difftest_step(uint32_t);
  difftest_step(eip);
#endif
}
