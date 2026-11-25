/*
 * kernel-arch —— Linux 内核架构总览接口
 * 对应文档: ../06-kernel/kernel-01-内核架构总览.md
 * 职责: 内核子系统入口、模块接口、系统调用约定。
 */
#ifndef KERNEL_ARCH_H
#define KERNEL_ARCH_H

#include <stdint.h>

/* ---- 内核模块生命周期 ---- */
typedef int (*module_init_fn)(void);
typedef void (*module_exit_fn)(void);

#define KERNEL_MODULE_INIT(name, init_fn, exit_fn) /* 模块注册占位 */

/* ---- 模块注册接口 ---- */
int kernel_module_register(const char *name, module_init_fn init, module_exit_fn exit);
void kernel_module_unregister(const char *name);

/* ---- 导出符号（模块间接口） ---- */
#define KERNEL_EXPORT_SYMBOL(sym) /* 符号导出占位 */

/* ---- 内核版本与信息 ---- */
const char *kernel_version(void);
uint32_t kernel_api_version(void);

/* ---- 系统调用约定（用户态入口） ---- */
int kernel_syscall_register(uint32_t nr, void *handler);

#endif /* KERNEL_ARCH_H */
