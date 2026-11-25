/*
 * memory-pool —— 内存管理与内存池接口
 * 对应文档: ../01-c/c-05-内存管理与内存池.md
 * 职责: 固定块内存池，确定性 O(1) 分配/释放，高水位统计。
 */
#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void    *memory;        /* 池首地址 */
    size_t   block_size;    /* 块大小（含块头） */
    uint32_t block_count;   /* 块数量 */
    void    *free_list;     /* 空闲块链表头 */
    uint32_t used;          /* 当前占用块数 */
    uint32_t high_water;    /* 峰值占用块数 */
} mem_pool_t;

/* ---- 生命周期 ---- */
void mem_pool_init(mem_pool_t *pool, void *memory, size_t block_size, uint32_t block_count);

/* ---- 分配/释放（O(1)） ---- */
void *mem_pool_alloc(mem_pool_t *pool);
void  mem_pool_free(mem_pool_t *pool, void *ptr);

/* ---- 统计/审计 ---- */
uint32_t mem_pool_usage(const mem_pool_t *pool);
uint32_t mem_pool_high_water(const mem_pool_t *pool);
size_t   mem_pool_free_count(const mem_pool_t *pool);

/* ---- 调试/校验 ---- */
int mem_pool_verify(const mem_pool_t *pool);

#endif /* MEMORY_POOL_H */
