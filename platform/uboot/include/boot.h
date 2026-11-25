/*
 * uboot —— RK3588 启动流程与 U-Boot 接口
 * 对应文档: ../04-rk3588/rk3588-02-启动流程与U-Boot.md
 * 职责: 多级启动阶段定义、分区布局、安全启动校验接口。
 */
#ifndef RK3588_BOOT_H
#define RK3588_BOOT_H

#include <stdint.h>

/* ---- 启动阶段（信任链，见文档） ---- */
typedef enum {
    BOOT_STAGE_ROM = 0,     /* BootROM */
    BOOT_STAGE_SPL,         /* TPL/SPL */
    BOOT_STAGE_UBOOT,       /* U-Boot */
    BOOT_STAGE_KERNEL,      /* Linux 内核 */
    BOOT_STAGE_ROOTFS,      /* 根文件系统 */
} boot_stage_t;

/* ---- A/B 分区槽位 ---- */
typedef enum {
    BOOT_SLOT_A = 0,
    BOOT_SLOT_B = 1,
} boot_slot_t;

/* ---- 分区布局标识 ---- */
typedef struct {
    const char *idbloader;
    const char *misc;
    const char *boot_a;
    const char *boot_b;
    const char *system_a;
    const char *system_b;
    const char *vendor;
    const char *userdata;
} boot_partition_layout_t;

/* ---- 安全启动（信任链校验） ---- */
int boot_verify_image(boot_stage_t stage, const void *image, uint32_t size);
int boot_get_current_slot(boot_slot_t *slot);
int boot_switch_slot(boot_slot_t slot);      /* A/B 切换 */
int boot_enter_recovery(void);               /* 失败进入恢复模式 */

/* ---- 启动参数 ---- */
const char *boot_get_kernel_cmdline(void);
void boot_set_dtb_load_addr(uintptr_t addr);

#endif /* RK3588_BOOT_H */
