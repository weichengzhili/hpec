/*
 * memory-model —— 内存模型与存储布局接口
 * 对应文档: ../01-c/c-01-内存模型与存储布局.md
 * 职责: 存储类、内存段、对齐、端序、ABI 相关声明与静态断言。
 */
#ifndef MEMORY_MODEL_H
#define MEMORY_MODEL_H

#include <stddef.h>
#include <stdint.h>

/* ---- 链接脚本导出符号（由启动代码引用，见 boot/startup） ---- */
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __data_load_start__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __heap_start__;
extern uint32_t __heap_end__;
extern uint32_t __stack_top__;

/* ---- 对齐 ---- */
#define MM_ALIGN_UP(n, a)   (((n) + ((a) - 1u)) & ~((a) - 1u))
#define MM_ALIGN_DOWN(n, a) ((n) & ~((a) - 1u))

/* ---- 编译期静态断言（防结构体布局/类型位宽假设漂移） ---- */
#define MM_STATIC_ASSERT(expr, msg) _Static_assert((expr), msg)

/* ---- 端序检测与转换 ---- */
#define MM_LITTLE_ENDIAN 1u
#define MM_BIG_ENDIAN    0u

/* 启动自检：.bss 是否清零、.data 是否拷贝完整 */
void mm_startup_check(void);

/* 栈水位检测（防栈溢出） */
void mm_stack_watermark_init(void *stack_low, size_t stack_size);
size_t mm_stack_watermark_usage(void);

#endif /* MEMORY_MODEL_H */
