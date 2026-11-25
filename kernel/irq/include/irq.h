/*
 * irq —— 中断与下半部接口
 * 对应文档: ../06-kernel/kernel-04-中断与下半部.md
 * 职责: 中断注册、上半部/下半部（softirq/tasklet/threaded）接口。
 */
#ifndef KERNEL_IRQ_H
#define KERNEL_IRQ_H

#include <stdint.h>
#include <stdbool.h>

/* ---- 中断处理函数（上半部） ---- */
typedef uint32_t (*irq_handler_t)(uint32_t irq, void *dev_id);

/* ---- 线程化处理函数（下半部） ---- */
typedef uint32_t (*irq_thread_fn_t)(uint32_t irq, void *dev_id);

/* ---- 中断注册 ---- */
int irq_request(uint32_t irq, irq_handler_t handler, uint32_t flags, const char *name, void *dev_id);
int irq_request_threaded(uint32_t irq, irq_handler_t handler,
                         irq_thread_fn_t thread_fn, uint32_t flags, const char *name, void *dev_id);
void irq_free(uint32_t irq, void *dev_id);

/* ---- 中断控制 ---- */
int irq_enable(uint32_t irq);
int irq_disable(uint32_t irq);
int irq_set_affinity(uint32_t irq, uint64_t cpu_mask);

/* ---- 下半部：softirq / tasklet ---- */
void softirq_raise(uint32_t softirq);
void tasklet_schedule(void (*fn)(uintptr_t), uintptr_t data);

/* ---- 下半部：workqueue（见 workqueue.h） ---- */

/* ---- 中断统计 ---- */
uint64_t irq_count(uint32_t irq);

#endif /* KERNEL_IRQ_H */
