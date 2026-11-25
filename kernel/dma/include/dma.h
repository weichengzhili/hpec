/*
 * dma —— DMA 与缓存一致性接口
 * 对应文档: ../06-kernel/kernel-12-DMA与缓存一致性.md
 * 职责: DMA API、一致性/流式映射、IOMMU/SMMU 接口。
 */
#ifndef KERNEL_DMA_H
#define KERNEL_DMA_H

#include <stdint.h>
#include <stddef.h>

/* ---- DMA 方向 ---- */
typedef enum {
    DMA_TO_DEVICE = 0,
    DMA_FROM_DEVICE,
    DMA_BIDIRECTIONAL,
} dma_dir_t;

/* ---- 一致性映射 ---- */
void *dma_alloc_coherent(void *dev, size_t size, uintptr_t *handle);
void  dma_free_coherent(void *dev, size_t size, void *cpu_addr, uintptr_t handle);

/* ---- 流式映射 ---- */
uintptr_t dma_map_single(void *dev, void *cpu_addr, size_t size, dma_dir_t dir);
void dma_unmap_single(void *dev, uintptr_t handle, size_t size, dma_dir_t dir);
void dma_sync_for_cpu(void *dev, uintptr_t handle, size_t size, dma_dir_t dir);
void dma_sync_for_device(void *dev, uintptr_t handle, size_t size, dma_dir_t dir);

/* ---- scatter-gather ---- */
typedef struct dma_sg {
    uintptr_t addr;
    size_t    len;
} dma_sg_t;

int dma_map_sg(void *dev, dma_sg_t *sg, int nents, dma_dir_t dir);
void dma_unmap_sg(void *dev, dma_sg_t *sg, int nents, dma_dir_t dir);

/* ---- IOMMU/SMMU（设备 DMA 隔离） ---- */
int iommu_attach_device(void *iommu, void *dev);
int iommu_detach_device(void *iommu, void *dev);
int iommu_map(void *iommu, uintptr_t iova, uintptr_t phys, size_t size);
int iommu_unmap(void *iommu, uintptr_t iova, size_t size);

#endif /* KERNEL_DMA_H */
