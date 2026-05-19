#include "nemu.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
  return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
}

void paddr_write(paddr_t addr, int len, uint32_t data) {
  // 🟢 借用完美的 pmem_rw 宏进行安全的边界检查与直写！
  // 根据不同的长度，强转成对应类型的指针并直接赋值
  if (len == 1) {
    pmem_rw(addr, uint8_t) = (uint8_t)data;
  } else if (len == 2) {
    pmem_rw(addr, uint16_t) = (uint16_t)data;
  } else if (len == 4) {
    pmem_rw(addr, uint32_t) = (uint32_t)data;
  } else {
    Assert(0, "Invalid internal write length: %d", len);
  }
}

uint32_t vaddr_read(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, uint32_t data) {
  paddr_write(addr, len, data);
}
