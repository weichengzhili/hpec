/*
 * lock —— 内核锁与同步接口
 * 对应文档: ../06-kernel/kernel-05-内核锁与同步.md
 * 职责: 自旋锁、互斥量、读写锁、RCU、原子操作、完成量接口。
 */
#ifndef KERNEL_LOCK_H
#define KERNEL_LOCK_H

#include <stdint.h>
#include <stdbool.h>

/* ---- 自旋锁 ---- */
typedef struct spinlock { uint32_t raw; } spinlock_t;
void spin_lock(spinlock_t *lock);
void spin_unlock(spinlock_t *lock);
void spin_lock_irqsave(spinlock_t *lock, uint64_t *flags);
void spin_unlock_irqrestore(spinlock_t *lock, uint64_t flags);

/* ---- 互斥量 ---- */
typedef struct mutex { void *owner; } mutex_t;
void mutex_init(mutex_t *m);
void mutex_lock(mutex_t *m);
bool mutex_trylock(mutex_t *m);
void mutex_unlock(mutex_t *m);

/* ---- 读写锁 ---- */
typedef struct rwlock { uint32_t raw; } rwlock_t;
void read_lock(rwlock_t *lock);
void read_unlock(rwlock_t *lock);
void write_lock(rwlock_t *lock);
void write_unlock(rwlock_t *lock);

/* ---- RCU ---- */
void rcu_read_lock(void);
void rcu_read_unlock(void);
void synchronize_rcu(void);
void rcu_assign_pointer(void **p, void *v);
void *rcu_dereference(void **p);

/* ---- 原子操作 ---- */
int32_t atomic_add(int32_t *p, int32_t v);
int32_t atomic_cmpxchg(int32_t *p, int32_t expect, int32_t desired);
int32_t atomic_read(const int32_t *p);

/* ---- 完成量 ---- */
typedef struct completion { uint32_t done; } completion_t;
void completion_init(completion_t *c);
void completion_complete(completion_t *c);
void completion_wait(completion_t *c);

#endif /* KERNEL_LOCK_H */
