/*
 * startup —— 链接脚本与启动流程接口
 * 对应文档: ../01-c/c-06-链接脚本与启动流程.md
 * 职责: 启动入口、段初始化钩子、看门狗/时钟启动配置。
 */
#ifndef STARTUP_H
#define STARTUP_H

#include <stdint.h>

/* ---- 启动入口（由复位向量跳转） ---- */
void _start(void);

/* ---- 段初始化（.data 拷贝、.bss 清零） ---- */
void startup_copy_data(void);
void startup_zero_bss(void);
void startup_init_c_runtime(void);

/* ---- 板级/系统启动钩子 ---- */
void system_init(void);
void board_init(void);
void clock_init(void);
void watchdog_init(void);
void interrupt_init(void);

/* ---- 镜像完整性校验 ---- */
int  startup_image_crc_check(void);
int  startup_image_signature_check(void);

#endif /* STARTUP_H */
