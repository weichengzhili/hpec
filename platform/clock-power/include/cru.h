/*
 * clock-power —— RK3588 时钟与电源管理接口
 * 对应文档: ../04-rk3588/rk3588-03-时钟与电源管理.md
 * 职责: 时钟树（CRU）、功耗域（PMU）、DVFS 接口声明。
 */
#ifndef RK3588_CRU_H
#define RK3588_CRU_H

#include <stdint.h>

/* ---- 时钟 ID（时钟树节点标识） ---- */
typedef enum {
    CLK_CPU_BIG = 0,
    CLK_CPU_LITTLE,
    CLK_GPU,
    CLK_NPU,
    CLK_DDR,
    CLK_BUS,
    CLK_UART,
    CLK_SPI,
    CLK_I2C,
    CLK_MAX
} clk_id_t;

/* ---- 功耗域 ---- */
typedef enum {
    PD_CPU_BIG = 0,
    PD_CPU_LITTLE,
    PD_GPU,
    PD_NPU,
    PD_VPU,
    PD_VO,        /* 视频输出 */
    PD_MAX
} power_domain_t;

/* ---- 时钟操作 ---- */
int cru_clk_enable(clk_id_t id);
int cru_clk_disable(clk_id_t id);
int cru_clk_set_rate(clk_id_t id, uint32_t rate_hz);
uint32_t cru_clk_get_rate(clk_id_t id);

/* ---- 功耗域操作 ---- */
int pmu_pd_power_on(power_domain_t pd);
int pmu_pd_power_off(power_domain_t pd);
int pmu_pd_is_on(power_domain_t pd);

/* ---- DVFS（OPP 表） ---- */
typedef struct {
    uint32_t freq_hz;
    uint32_t volt_mv;
} opp_point_t;

int dvfs_set_opp(clk_id_t id, uint32_t freq_hz);
int dvfs_get_opp_table(clk_id_t id, const opp_point_t **table, uint32_t *count);

#endif /* RK3588_CRU_H */
