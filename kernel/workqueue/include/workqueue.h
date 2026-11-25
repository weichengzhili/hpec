/*
 * workqueue —— 工作队列与内核线程接口
 * 对应文档: ../06-kernel/kernel-11-工作队列与内核线程.md
 * 职责: workqueue、专用 workqueue、kthread 接口。
 */
#ifndef KERNEL_WORKQUEUE_H
#define KERNEL_WORKQUEUE_H

#include <stdint.h>
#include <stdbool.h>

/* ---- 工作项 ---- */
typedef void (*work_fn_t)(void *arg);

typedef struct work_struct {
    work_fn_t fn;
    void *arg;
    uint32_t state;
} work_struct_t;

/* ---- 系统 workqueue ---- */
void schedule_work(work_struct_t *work);
void schedule_delayed_work(work_struct_t *work, uint32_t delay_ms);
bool flush_work(work_struct_t *work);
bool cancel_work(work_struct_t *work);

/* ---- 专用 workqueue（实时场景可设优先级） ---- */
typedef struct workqueue workqueue_t;
workqueue_t *alloc_workqueue(const char *name, uint32_t flags, int32_t nice);
void queue_work(workqueue_t *wq, work_struct_t *work);
void destroy_workqueue(workqueue_t *wq);

/* ---- 内核线程 ---- */
typedef int (*thread_fn_t)(void *arg);
void *kthread_run(thread_fn_t fn, void *arg, const char *name);
int kthread_stop(void *thread);
bool kthread_should_stop(void);

#endif /* KERNEL_WORKQUEUE_H */
