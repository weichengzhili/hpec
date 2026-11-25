/*
 * interrupt-critical —— 中断安全与临界区接口
 * 对应文档: ../01-c/c-04-中断安全与临界区.md
 * 职责: 临界区进入/退出、原子操作、SPSC 无锁队列。
 */
#ifndef INTERRUPT_CRITICAL_H
#define INTERRUPT_CRITICAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---- 临界区（底层映射为关中断/锁） ---- */
typedef uint32_t critical_state_t;
critical_state_t critical_enter(void);
void critical_exit(critical_state_t state);

/* ---- 原子操作 ---- */
int32_t atomic_add_i32(volatile int32_t *p, int32_t v);
int32_t atomic_fetch_i32(volatile int32_t *p);
int32_t atomic_cmpxchg_i32(volatile int32_t *p, int32_t expect, int32_t desired);
uint32_t atomic_add_u32(volatile uint32_t *p, uint32_t v);

/* ---- SPSC 无锁环形队列（ISR 写 / 任务读） ---- */
typedef struct {
    volatile uint32_t head;
    volatile uint32_t tail;
    uint32_t mask;
    uint8_t  *buf;
    uint32_t item_size;
} spsc_queue_t;

void spsc_queue_init(spsc_queue_t *q, void *buf, uint32_t item_size, uint32_t capacity_pow2);
bool spsc_push(spsc_queue_t *q, const void *item);
bool spsc_pop(spsc_queue_t *q, void *item);
uint32_t spsc_count(const spsc_queue_t *q);
bool spsc_empty(const spsc_queue_t *q);
bool spsc_full(const spsc_queue_t *q);

#endif /* INTERRUPT_CRITICAL_H */
