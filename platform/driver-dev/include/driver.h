/*
 * driver-dev —— RK3588 驱动开发接口
 * 对应文档: ../04-rk3588/rk3588-13-驱动开发.md
 * 职责: platform 驱动与字符设备驱动的标准结构接口声明。
 */
#ifndef RK3588_DRIVER_H
#define RK3588_DRIVER_H

#include <stdint.h>
#include <stddef.h>

/* ---- 平台驱动操作 ---- */
typedef struct {
    int (*probe)(void *dev);
    int (*remove)(void *dev);
} platform_driver_ops_t;

/* ---- 字符设备操作（file_operations） ---- */
typedef struct {
    int  (*open)(void *inode, void *file);
    int  (*release)(void *inode, void *file);
    ssize_t (*read)(void *file, char *buf, size_t len);
    ssize_t (*write)(void *file, const char *buf, size_t len);
    int  (*ioctl)(void *file, uint32_t cmd, uintptr_t arg);
    int  (*mmap)(void *file, void *vma);
    unsigned int (*poll)(void *file, void *wait);
} file_operations_t;

/* ---- 驱动注册 ---- */
int driver_register_platform(const char *compatible, const platform_driver_ops_t *ops);
int driver_register_char(const char *name, const file_operations_t *fops, uint32_t *major);
void driver_unregister(const char *name);

/* ---- 设备资源获取（与 soc-arch/gic/cru 联动） ---- */
int driver_get_irq(void *dev, uint32_t index, uint32_t *irq);
int driver_get_clock(void *dev, uint32_t index, void **clk);
uintptr_t driver_get_mmio_base(void *dev, uint32_t index);
int driver_enable_pm_runtime(void *dev);   /* pm_runtime 自动管理 */

#endif /* RK3588_DRIVER_H */
