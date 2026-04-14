SECTIONS {
  . = 0x100000;
  .text : {
    *(entry)
    *(.text*)
  }
  etext = .;
  _etext = .;
  .rodata : {
    *(.rodata*)
  }
  .data : {
    *(.data*)
  }
  edata = .;
  _edata = .;
  .bss : {
    *(.bss*)
  }
  end = .;
  _end = .;
  _heap_start = ALIGN(4096);
  _stack_pointer = 0x7c00;
  _heap_end = 0x8000000;
}
