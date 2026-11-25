/*
 * memory-barrier —— volatile 与内存屏障接口
 * 对应文档: ../01-c/c-02-volatile与内存屏障.md
 * 职责: 硬件寄存器 MMIO 访问封装与内存屏障原语。
 */
#ifndef VOLATILE_BARRIER_H
#define VOLATILE_BARRIER_H

#include <stdint.h>
#include <stddef.h>

/* ---- MMIO 寄存器访问（统一封装，禁止直接 *(volatile uint32_t*)addr） ---- */
uint8_t  mmio_read8(uintptr_t addr);
uint16_t mmio_read16(uintptr_t addr);
uint32_t mmio_read32(uintptr_t addr);
uint64_t mmio_read64(uintptr_t addr);

void mmio_write8(uintptr_t addr, uint8_t value);
void mmio_write16(uintptr_t addr, uint16_t value);
void mmio_write32(uintptr_t addr, uint32_t value);
void mmio_write64(uintptr_t addr, uint64_t value);

/* ---- 内存屏障（编译器 + 架构级） ---- */
void mb_compiler(void);   /* 编译器屏障 */
void mb_read(void);       /* 读屏障 */
void mb_write(void);      /* 写屏障 */
void mb_full(void);       /* 完整屏障 */
void mb_isb(void);        /* 指令同步屏障 */
void mb_dsb(void);        /* 数据同步屏障 */

/* ---- 中断安全标志类型（C11 唯一标准保证） ---- */
typedef volatile int mb_sig_atomic_t;

/* ---- 缓存一致性（DMA 前后使用，见 kernel/dma） ---- */
void cache_clean_range(void *addr, size_t size);
void cache_invalidate_range(void *addr, size_t size);
void cache_clean_invalidate_range(void *addr, size_t size);

#endif /* VOLATILE_BARRIER_H */
