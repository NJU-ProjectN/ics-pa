#include "cpu/exec.h"

make_EHelper(real);

make_EHelper(operand_size) {
  decoding.is_operand_size_16 = true;
  print_asm("operand_size");
  //exec_real(eip + 1);
  //decoding.is_operand_size_16 = false;
}
