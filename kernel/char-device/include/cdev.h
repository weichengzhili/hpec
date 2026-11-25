/*
 * char-device —— 字符设备驱动接口
 * 对应文档: ../06-kernel/kernel-07-字符设备驱动.md
 * 职责: 字符设备注册与 file_operations 接口。
 */
#ifndef KERNEL_CDEV_H
#define KERNEL_CDEV_H

#include <stdint.h>
#include <stddef.h>

/* ---- 字符设备操作 ---- */
typedef struct {
    int (*open)(void *inode, void *file);
    int (*release)(void *inode, void *file);
    ssize_t (*read)(void *file, char *buf, size_t len, uint64_t off);
    ssize_t (*write)(void *file, const char *buf, size_t len, uint64_t off);
    int (*ioctl)(void *file, uint32_t cmd, uintptr_t arg);
    int (*mmap)(void *file, void *vma);
    unsigned int (*poll)(void *file, void *wait);
} cdev_ops_t;

/* ---- 注册/注销 ---- */
int cdev_register(const char *name, const cdev_ops_t *ops, uint32_t *major, uint32_t *minor);
void cdev_unregister(const char *name, uint32_t major, uint32_t minor);

/* ---- 用户态数据拷贝 ---- */
int copy_to_user(void *to, const void *from, size_t len);
int copy_from_user(void *to, const void *from, size_t len);

/* ---- 等待队列（poll/阻塞） ---- */
void wait_queue_sleep(void *wait);
void wait_queue_wake(void *wait);

#endif /* KERNEL_CDEV_H */
