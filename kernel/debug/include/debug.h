/*
 * debug —— 内核调试接口
 * 对应文档: ../06-kernel/kernel-13-内核调试.md
 * 职责: printk/dynamic debug、ftrace、kprobe、panic 分析接口。
 */
#ifndef KERNEL_DEBUG_H
#define KERNEL_DEBUG_H

#include <stdint.h>

/* ---- 日志（printk 分级） ---- */
void printk_log(int level, const char *fmt, ...);

/* ---- dynamic debug（生产零开销） ---- */
int dynamic_debug_enable(const char *pattern);
int dynamic_debug_disable(const char *pattern);

/* ---- ftrace 追踪 ---- */
int ftrace_function_start(const char *func);
int ftrace_function_stop(void);
void ftrace_trace_printk(const char *fmt, ...);

/* ---- kprobe 动态插桩 ---- */
typedef int (*kprobe_handler_t)(void *regs);
int kprobe_register(const char *func, kprobe_handler_t pre, kprobe_handler_t post);
void kprobe_unregister(const char *func);

/* ---- panic/oops ---- */
void panic_dump_registers(void);
void panic_store_stack(void);
void kernel_oops_report(uintptr_t pc, uintptr_t lr);

/* ---- 内存错误检测（KASAN，调试版） ---- */
int kasan_check_range(uintptr_t addr, size_t size);

#endif /* KERNEL_DEBUG_H */
