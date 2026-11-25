/*
 * misra —— MISRA-C 与功能安全编码接口
 * 对应文档: ../01-c/c-07-MISRA-C与功能安全编码.md
 * 职责: 功能安全相关的类型约束、防御宏、安全返回码。
 */
#ifndef MISRA_SAFETY_H
#define MISRA_SAFETY_H

#include <stdint.h>

typedef uint8_t  u8_t;
typedef int8_t   s8_t;
typedef uint16_t u16_t;
typedef int16_t  s16_t;
typedef uint32_t u32_t;
typedef int32_t  s32_t;
typedef uint64_t u64_t;
typedef int64_t  s64_t;
typedef uintptr_t addr_t;

typedef enum {
    SAFETY_OK        = 0,
    SAFETY_ERR_PARAM = -1,
    SAFETY_ERR_STATE = -2,
    SAFETY_ERR_FAULT = -3,
    SAFETY_ERR_FATAL = -4,
} safety_ret_t;

#ifdef SAFETY_ASSERT_ENABLE
#define SAFETY_ASSERT(cond) ((void)((cond) ? 0 : safety_assert_fail(#cond)))
#else
#define SAFETY_ASSERT(cond) ((void)0)
#endif

void safety_assert_fail(const char *cond);
void safety_watchdog_kick(void);
void safety_monitor_report(safety_ret_t status);

#endif /* MISRA_SAFETY_H */
