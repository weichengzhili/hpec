/*
 * mm —— 内核内存管理接口
 * 对应文档: ../06-kernel/kernel-03-内存管理.md
 * 职责: 页分配、slab、vmalloc、内存映射接口。
 */
#ifndef KERNEL_MM_H
#define KERNEL_MM_H

#include <stdint.h>
#include <stddef.h>

/* ---- 页分配（buddy） ---- */
void *mm_alloc_pages(uint32_t order);          /* 2^order 页 */
void  mm_free_pages(void *ptr, uint32_t order);

/* ---- slab 小对象 ---- */
void *mm_kmalloc(size_t size);
void *mm_kzalloc(size_t size);                  /* 清零 */
void  mm_kfree(void *ptr);

/* ---- 虚拟连续（非物理连续） ---- */
void *mm_vmalloc(size_t size);
void  mm_vfree(void *ptr);

/* ---- 内存映射（用户态/DMA） ---- */
int mm_mmap(void *vma, uintptr_t phys, size_t size);
int mm_munmap(void *vma, size_t size);

/* ---- 内存统计/水位 ---- */
uint64_t mm_total_pages(void);
uint64_t mm_free_pages(void);
int mm_watermark_check(void);

/* ---- 内存回收与 OOM ---- */
void mm_reclaim(void);
void mm_oom_notify(uint32_t reason);

#endif /* KERNEL_MM_H */
