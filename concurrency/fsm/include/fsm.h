/*
 * fsm —— 状态机与任务设计接口
 * 对应文档: ../01-c/c-10-状态机与任务设计.md
 * 职责: 表驱动状态机、事件队列、非法转换检测。
 */
#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef int32_t fsm_state_t;
typedef int32_t fsm_event_t;

typedef fsm_state_t (*fsm_action_t)(void *ctx, fsm_event_t ev);

typedef struct {
    fsm_state_t  from;
    fsm_event_t  event;
    fsm_state_t  to;
    fsm_action_t action;
} fsm_transition_t;

typedef struct {
    fsm_state_t             current;
    const fsm_transition_t *table;
    size_t                  table_size;
    void                   *ctx;
    void (*on_entry)(void *ctx, fsm_state_t state);
    void (*on_exit)(void *ctx, fsm_state_t state);
} fsm_t;

void fsm_init(fsm_t *fsm, const fsm_transition_t *table, size_t table_size,
              void *ctx, fsm_state_t initial);
fsm_state_t fsm_dispatch(fsm_t *fsm, fsm_event_t ev);
fsm_state_t fsm_current(const fsm_t *fsm);
bool fsm_can_transition(const fsm_t *fsm, fsm_event_t ev);

typedef struct fsm_event_queue fsm_event_queue_t;
fsm_event_queue_t *fsm_event_queue_create(size_t capacity);
void fsm_event_queue_push(fsm_event_queue_t *q, fsm_event_t ev);
bool fsm_event_queue_pop(fsm_event_queue_t *q, fsm_event_t *ev);

#endif /* FSM_H */
