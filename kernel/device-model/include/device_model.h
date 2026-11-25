/*
 * device-model —— 设备模型与 sysfs 接口
 * 对应文档: ../06-kernel/kernel-06-设备模型与sysfs.md
 * 职责: 总线/设备/驱动三层抽象、匹配、sysfs 属性接口。
 */
#ifndef KERNEL_DEVICE_MODEL_H
#define KERNEL_DEVICE_MODEL_H

#include <stdint.h>
#include <stddef.h>

/* ---- 设备/驱动抽象 ---- */
typedef struct device {
    const char *name;
    void *of_node;          /* 设备树节点 */
    void *driver_data;
} device_t;

typedef struct device_driver {
    const char *name;
    const char **compatible; /* 匹配表 */
    int (*probe)(device_t *dev);
    void (*remove)(device_t *dev);
} device_driver_t;

/* ---- 总线注册与匹配 ---- */
int bus_register(const char *bus_name);
int device_register(device_t *dev);
int driver_register(device_driver_t *drv);
int bus_match_driver(device_t *dev, device_driver_t *drv);

/* ---- sysfs 属性 ---- */
typedef struct device_attribute {
    const char *name;
    ssize_t (*show)(device_t *dev, char *buf);
    ssize_t (*store)(device_t *dev, const char *buf, size_t len);
} device_attribute_t;

int sysfs_create_group(device_t *dev, const device_attribute_t *attrs, size_t count);
void sysfs_remove_group(device_t *dev);

/* ---- 热插拔通知 ---- */
int uevent_send(device_t *dev, const char *action);

#endif /* KERNEL_DEVICE_MODEL_H */
