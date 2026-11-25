/*
 * platform-driver —— 平台驱动与设备树接口
 * 对应文档: ../06-kernel/kernel-08-平台驱动与设备树.md
 * 职责: platform 驱动注册、compatible 匹配、资源获取接口。
 */
#ifndef KERNEL_PLATFORM_H
#define KERNEL_PLATFORM_H

#include <stdint.h>
#include <stddef.h>

/* ---- 平台驱动 ---- */
typedef struct {
    const char *name;
    const char **of_match_table;  /* compatible 列表 */
    int (*probe)(void *dev);
    int (*remove)(void *dev);
} platform_driver_t;

int platform_driver_register(platform_driver_t *drv);
void platform_driver_unregister(platform_driver_t *drv);

/* ---- 资源类型 ---- */
typedef enum {
    RES_IRQ = 0,
    RES_MEM,
    RES_CLOCK,
    RES_PINCTRL,
} resource_type_t;

/* ---- 资源获取（devm 自动管理） ---- */
int devm_get_irq(void *dev, uint32_t index, uint32_t *irq);
uintptr_t devm_get_mmio(void *dev, uint32_t index, size_t *size);
void *devm_get_clock(void *dev, uint32_t index);
void *devm_get_pinctrl(void *dev, const char *state);

/* ---- 设备树解析 ---- */
int of_device_is_available(void *node);
const void *of_get_property(void *node, const char *name, int *len);
uint32_t of_get_interrupt(void *node, uint32_t index);

#endif /* KERNEL_PLATFORM_H */
