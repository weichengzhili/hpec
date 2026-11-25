/*
 * soc-arch —— RK3588 SoC 架构总览接口
 * 对应文档: ../04-rk3588/rk3588-01-SoC架构总览.md
 * 职责: 内存映射基址、子系统基址、资源分配约定。
 */
#ifndef RK3588_SOC_H
#define RK3588_SOC_H

#include <stdint.h>

/* ---- 外设总线基址（MMIO 统一编址，见文档） ---- */
#define RK3588_GIC_BASE       0xFE000000u   /* GIC-600 */
#define RK3588_CRU_BASE       0xFD7C0000u   /* 时钟控制单元 */
#define RK3588_PMU_BASE       0xFD8D8000u   /* 电源管理单元 */
#define RK3588_GPIO0_BASE     0xFD8A0000u
#define RK3588_UART_BASE      0xFEB50000u
#define RK3588_DMA_BASE       0xFED10000u
#define RK3588_NPU_BASE       0xFDAB0000u
#define RK3588_GPU_BASE       0xFB000000u
#define RK3588_VPU_BASE       0xFDB50000u
#define RK3588_ISP_BASE       0xFDCB0000u
#define RK3588_CSI_BASE       0xFDCB0000u
#define RK3588_PCIE_BASE      0xFE150000u
#define RK3588_USB3_BASE      0xFC000000u

/* ---- CPU 集群（big.LITTLE） ---- */
#define RK3588_CPU_BIG_CORES   4   /* Cortex-A76 */
#define RK3588_CPU_LITTLE_CORES 4  /* Cortex-A55 */
#define RK3588_CPU_TOTAL_CORES 8

/* ---- 中断线分配（SPI 编号约定，见 gic.h） ---- */
typedef enum {
    RK3588_IRQ_UART0  = 32,
    RK3588_IRQ_DMA    = 40,
    RK3588_IRQ_NPU    = 80,
    RK3588_IRQ_GPU    = 120,
} rk3588_irq_t;

/* ---- 资源分配查询 ---- */
uint32_t soc_peripheral_base(uint32_t peripheral_id);
const char *soc_name(void);
uint32_t soc_cpu_count(void);

#endif /* RK3588_SOC_H */
