#include "cpu/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };
  rtlreg_t t0 = 0, t1 = 0;
  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O: // 对应 O 和 NO
      rtl_get_OF(dest);
      break;
    case CC_B: // 对应 B 和 NB (jc / jnc)
      rtl_get_CF(dest);
      break;
    case CC_E: // 对应 E 和 NE (je / jne)
      rtl_get_ZF(dest);
      break;
    case CC_BE: // 对应 BE 和 NBE (jbe / jnbe)
      // 条件是：CF == 1 或 ZF == 1
      rtl_get_CF(&t0);
      rtl_get_ZF(&t1);
      rtl_or(dest, &t0, &t1);
      break;
    case CC_S: // 对应 S 和 NS
      rtl_get_SF(dest);
      break;
    case CC_L: // 对应 L 和 NL (jl / jnl)
      rtl_get_SF(&t0);
      rtl_get_OF(&t1);
      rtl_xor(dest, &t0, &t1);
      break;
    case CC_LE: // 对应 LE 和 NLE (jle / jnle)
      rtl_get_SF(&t0);
      rtl_get_OF(&t1);
      rtl_xor(&t0, &t0, &t1); // t0 = SF ^ OF
      rtl_get_ZF(&t1);        // t1 = ZF
      rtl_or(dest, &t0, &t1); // dest = (SF ^ OF) | ZF
      break;
      
    case CC_P: panic("n86 does not have PF");
    default: panic("should not reach here");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
}
