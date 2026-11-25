/*
 * realtime —— 实时内核与 PREEMPT_RT 接口
 * 对应文档: ../06-kernel/kernel-14-实时内核与PREEMPT_RT.md
 * 职责: 完全抢占、中断线程化、实时调度、核心隔离接口。
 */
#ifndef KERNEL_REALTIME_H
#define KERNEL_REALTIME_H

#include <stdint.h>
#include <stdbool.h>

/* ---- 抢占模型 ---- */
typedef enum {
    PREEMPT_NONE = 0,
    PREEMPT_VOLUNTARY,
    PREEMPT_FULL,          /* 完全抢占 */
    PREEMPT_RT,            /* 实时补丁 */
} preempt_model_t;

/* ---- 内核抢占配置 ---- */
preempt_model_t preempt_get_model(void);
int preempt_enable_rt(void);
int preempt_disable_rt(void);

/* ---- 中断线程化（实时补丁特性） ---- */
int irq_force_threaded(uint32_t irq, bool threaded);

/* ---- 实时调度（见 scheduler.h） ---- */

/* ---- 核心隔离（isolcpus/nohz） ---- */
int rt_isolate_cpu(uint32_t cpu);
int rt_unisolate_cpu(uint32_t cpu);

/* ---- 高精度时钟（hrtimer） ---- */
int hrtimer_start(uint64_t *timer, uint64_t expires_ns);
int hrtimer_cancel(uint64_t *timer);

/* ---- 延迟测量 ---- */
void latency_measure_start(void);
uint64_t latency_measure_stop_ns(void);

#endif /* KERNEL_REALTIME_H */
