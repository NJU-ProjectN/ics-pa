#include "common.h" 
#include "device/port-io.h"

extern uint8_t *pio_space;

/* http://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming */

#define SERIAL_PORT 0x3F8
#define CH_OFFSET 0
#define LSR_OFFSET 5		/* line status register */

static uint8_t *serial_port_base;

void serial_io_handler(ioaddr_t addr, int len, bool is_write) {
  if (is_write) {
    assert(len == 1);
    if (addr == SERIAL_PORT + CH_OFFSET) {
      char c = serial_port_base[CH_OFFSET];
      /* We bind the serial port with the host stdout in NEMU. */
      putc(c, stdout);
      if (c == '\n') {
        fflush(stdout);
      }
    }
  }
}

void init_serial() {
  serial_port_base = add_pio_map(SERIAL_PORT, 8, serial_io_handler);
  serial_port_base[LSR_OFFSET] = 0x20; /* the status is always free */
}
static uint8_t key_buffer = 0; // 存放按键码
static bool has_key = false;   // 状态标志
 void i8042_io_handler(ioaddr_t addr, int len, bool is_write) {
  if (!is_write) {
    if (addr == 0x60) {
      // CPU 读取数据端口 (IN)
      *(uint32_t *)(pio_space + addr) = key_buffer;
      has_key = false; // 读取后重置状态
    } else if (addr == 0x64) {
      // CPU 读取状态端口 (IN)
      // bit 0 为 1 表示有数据
      *(uint32_t *)(pio_space + addr) = has_key ? 0x1 : 0x0;
    }
  }
}