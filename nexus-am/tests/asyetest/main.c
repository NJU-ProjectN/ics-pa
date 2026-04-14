#include <am.h>
#include <klib.h>

_RegSet* handler(_Event ev, _RegSet *regs) {
  switch (ev.event) {
    case _EVENT_IRQ_TIME:
      printf(".");
      break;
    case _EVENT_IRQ_IODEV: 
      while (1) {
        int key = _read_key();
        if (key == _KEY_NONE) break;
        if (key & 0x8000) {
          printf("[D:%d]", key ^ 0x8000);
        } else {
          int ntraps = REG4(regs);
          REG4(regs) = 0;
          printf("[U:%d]/[TRAPS:%d]", key, ntraps);
        }
      }
      break;
    case _EVENT_TRAP:
      REG4(regs) ++;
      break;
  }
  REG1(regs) = 0;
  return regs;
}

int main(){
  _ioe_init();
  _asye_init(handler);
  assert(!_istatus(1));
  _make(_heap, main, 0);
  while (1) _trap();
  return 0;
}
