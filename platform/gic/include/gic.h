/*
 * gic —— RK3588 中断与 GIC 接口
 * 对应文档: ../04-rk3588/rk3588-05-中断与GIC.md
 * 职责: GIC-600 (GICv3) 中断控制器接口声明。
 */
#ifndef RK3588_GIC_H
#define RK3588_GIC_H

#include <stdint.h>

/* ---- 中断类型（GICv3） ---- */
typedef enum {
    GIC_IRQ_SGI = 0,    /* 软件生成中断（核间 IPI）0-15 */
    GIC_IRQ_PPI,        /* 私有外设中断 16-31 */
    GIC_IRQ_SPI,        /* 共享外设中断 32+ */
    GIC_IRQ_LPI,        /* 局部外设中断（ITS） */
} gic_irq_type_t;

/* ---- 中断优先级 ---- */
typedef enum {
    GIC_PRIO_HIGHEST = 0,
    GIC_PRIO_HIGH    = 64,
    GIC_PRIO_MEDIUM  = 128,
    GIC_PRIO_LOW     = 192,
    GIC_PRIO_LOWEST  = 255,
} gic_priority_t;

/* ---- GIC 操作 ---- */
int gic_init(void);
int gic_enable_irq(uint32_t irq);
int gic_disable_irq(uint32_t irq);
int gic_set_priority(uint32_t irq, gic_priority_t prio);
int gic_set_target_cpu(uint32_t irq, uint32_t cpu_mask);

/* ---- 核间通信（SGI/IPI） ---- */
int gic_send_sgi(uint32_t cpu, uint32_t sgi_id);

/* ---- 中断处理注册（与内核 request_irq 对应） ---- */
typedef void (*gic_irq_handler_t)(uint32_t irq, void *arg);
int gic_register_handler(uint32_t irq, gic_irq_handler_t h, void *arg);

/* ---- 结束中断处理（EOI） ---- */
void gic_end_of_interrupt(uint32_t irq);

#endif /* RK3588_GIC_H */
