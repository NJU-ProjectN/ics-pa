#include <am.h>
#include <x86.h>

static _RegSet* (*H)(_Event, _RegSet*) = NULL;

void vecsys();
void vecnull();

_RegSet* irq_handle(_RegSet *tf) {
  _RegSet *next = tf;
  if (H) {
    _Event ev;
    switch (tf->irq) {
      case 0x80: ev.event = _EVENT_SYSCALL; break;
      default: ev.event = _EVENT_ERROR; break;
    }

    next = H(ev, tf);
    if (next == NULL) {
      next = tf;
    }
  }

  return next;
}

static GateDesc idt[NR_IRQ];

void _asye_init(_RegSet*(*h)(_Event, _RegSet*)) {
  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), vecnull, DPL_KERN);
  }

  // -------------------- system call --------------------------
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), vecsys, DPL_USER);

  set_idt(idt, sizeof(idt));

  // register event handler
  H = h;
}

_RegSet *_make(_Area stack, void *entry, void *arg) {
  return NULL;
}

void _trap() {
}

int _istatus(int enable) {
  return 0;
}
