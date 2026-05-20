#include "nemu.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
  if (addr >= PMEM_SIZE) {
    printf("\n[NEMU FATAL MEMORY READ]\n");
    printf("访问非法物理地址: 0x%08x (len = %d)\n", addr, len);
    printf("此时 CPU 状态:\n");
    printf("  EIP = 0x%08x\n", cpu.eip);
    printf("  ESP = 0x%08x\n", cpu.esp);
    printf("  EBP = 0x%08x\n", cpu.ebp);
    printf("  EAX = 0x%08x, EBX = 0x%08x\n", cpu.eax, cpu.ebx);
    printf("  ECX = 0x%08x, EDX = 0x%08x\n", cpu.ecx, cpu.edx);
    printf("  ESI = 0x%08x, EDI = 0x%08x\n", cpu.esi, cpu.edi);
    
    // 强制挂起，让我们看清输出后再 assert
    assert(addr < PMEM_SIZE);
  }
  if (len == 1) {
    return pmem_rw(addr, uint8_t);
  } else if (len == 2) {
    return pmem_rw(addr, uint16_t);
  } else if (len == 4) {
    return pmem_rw(addr, uint32_t);
  } else {
    Assert(0, "Invalid internal read length: %d", len);
  }
  return 0;
}

void paddr_write(paddr_t addr, int len, uint32_t data) {
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
  if (addr >= 0xff000000) {
    addr = addr & 0xffff; // 0xfffffff0 & 0xffff = 0xfff0
  }
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, uint32_t data) {
  if (addr >= 0xff000000) {
    addr = addr & 0xffff;
  }
  paddr_write(addr, len, data);
}
