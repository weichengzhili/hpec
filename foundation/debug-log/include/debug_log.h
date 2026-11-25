/*
 * debug-log —— 调试与工具链接口
 * 对应文档: ../01-c/c-09-调试与工具链.md
 * 职责: 日志分级接口、断言、崩溃转储、环形缓冲追踪。
 */
#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    LOG_FATAL = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_TRACE,
    LOG_LEVEL_COUNT
} log_level_t;

void log_set_level(log_level_t level);
log_level_t log_get_level(void);
void log_write(log_level_t level, const char *tag, const char *fmt, ...);

void debug_assert_fail(const char *file, int line, const char *cond);
#define DEBUG_ASSERT(cond) \
    ((void)((cond) ? 0 : debug_assert_fail(__FILE__, __LINE__, #cond)))

typedef struct {
    uint32_t pc;
    uint32_t lr;
    uint32_t sp;
    uint32_t regs[16];
} crash_frame_t;

void debug_crash_dump(const crash_frame_t *frame);
void debug_crash_store_to_nvm(const crash_frame_t *frame);

void trace_event(uint32_t event_id, uint32_t arg0, uint32_t arg1);
void trace_dump(void);

#endif /* DEBUG_LOG_H */
