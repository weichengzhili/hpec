/*
 * dma-engine —— RK3588 DMA 引擎接口
 * 对应文档: ../04-rk3588/rk3588-06-DMA引擎.md
 * 职责: DMA 通道、描述符、缓存一致性接口声明。
 */
#ifndef RK3588_DMA_H
#define RK3588_DMA_H

#include <stdint.h>
#include <stddef.h>

/* ---- DMA 传输方向（决定 cache 操作） ---- */
typedef enum {
    DMA_TO_DEVICE = 0,     /* 内存 -> 外设 */
    DMA_FROM_DEVICE,       /* 外设 -> 内存 */
    DMA_BIDIRECTIONAL,     /* 双向 */
} dma_dir_t;

/* ---- DMA 缓冲分配/映射 ---- */
void *dma_alloc_coherent(size_t size, uintptr_t *dma_handle);
void  dma_free_coherent(size_t size, void *cpu_addr, uintptr_t dma_handle);

/* ---- 流式映射 ---- */
uintptr_t dma_map_single(void *cpu_addr, size_t size, dma_dir_t dir);
void dma_unmap_single(uintptr_t dma_handle, size_t size, dma_dir_t dir);
void dma_sync_single_for_cpu(uintptr_t dma_handle, size_t size, dma_dir_t dir);
void dma_sync_single_for_device(uintptr_t dma_handle, size_t size, dma_dir_t dir);

/* ---- 传输描述符（scatter-gather） ---- */
typedef struct {
    uintptr_t src;
    uintptr_t dst;
    size_t    len;
} dma_sg_entry_t;

/* ---- 传输接口 ---- */
typedef void (*dma_complete_cb_t)(void *arg);
int dma_transfer(uint32_t channel, const dma_sg_entry_t *sg, size_t sg_count,
                 dma_complete_cb_t cb, void *arg);
int dma_wait_complete(uint32_t channel, uint32_t timeout_ms);

#endif /* RK3588_DMA_H */
