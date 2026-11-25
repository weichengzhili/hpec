/*
 * scheduler —— 进程调度接口
 * 对应文档: ../06-kernel/kernel-02-进程调度.md
 * 职责: 调度策略、优先级、亲和性、实时任务隔离接口。
 */
#ifndef KERNEL_SCHED_H
#define KERNEL_SCHED_H

#include <stdint.h>

/* ---- 调度策略 ---- */
typedef enum {
    SCHED_NORMAL = 0,     /* CFS */
    SCHED_FIFO,           /* 实时 FIFO */
    SCHED_RR,             /* 实时轮转 */
    SCHED_DEADLINE,       /* 截止时间调度 */
} sched_policy_t;

/* ---- 调度参数 ---- */
typedef struct {
    sched_policy_t policy;
    int32_t priority;      /* 实时优先级 1-99 */
    int32_t nice;          /* CFS nice 值 -20..19 */
    uint64_t runtime_ns;   /* deadline 运行时间 */
    uint64_t deadline_ns;
    uint64_t period_ns;
} sched_param_t;

/* ---- 调度控制 ---- */
int sched_set_policy(uint32_t tid, const sched_param_t *param);
int sched_get_policy(uint32_t tid, sched_param_t *param);

/* ---- CPU 亲和性 ---- */
int sched_set_affinity(uint32_t tid, uint64_t cpu_mask);
uint64_t sched_get_affinity(uint32_t tid);

/* ---- 实时任务隔离（isolcpus/cpuset） ---- */
int sched_isolate_cpu(uint32_t cpu);
int sched_release_cpu(uint32_t cpu);

/* ---- 优先级反转防护（见 lock.h 优先级继承） ---- */
int sched_priority_ceil(uint32_t mutex_id, int32_t ceiling);

#endif /* KERNEL_SCHED_H */
