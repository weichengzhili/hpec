/*
 * resource-scheduling —— 内存资源调度接口
 * 对应文档: ../资源调度.md
 * 职责: 内存资源配额、预留、优先级准入、回收与归属审计。
 */
#ifndef RESOURCE_SCHEDULER_H
#define RESOURCE_SCHEDULER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MEM_RS_OK                 0
#define MEM_RS_ERR_INVAL         -1
#define MEM_RS_ERR_NOT_FOUND     -2
#define MEM_RS_ERR_QUOTA         -3
#define MEM_RS_ERR_CAPACITY      -4
#define MEM_RS_ERR_RECLAIM       -5
#define MEM_RS_ERR_TOKEN         -6
#define MEM_RS_ERR_BUSY          -7

#define MEM_RS_TOKEN_MAGIC       0x4D525354U /* "MRST" */

/* 资源/令牌标志 */
#define MEM_RS_F_NONE            0x00000000U
#define MEM_RS_F_RECLAIMABLE     0x00000001U
#define MEM_RS_F_CRITICAL        0x00000002U
#define MEM_RS_F_ZERO_INIT       0x00000004U

typedef enum {
    MEM_RS_POLICY_FIXED = 0,
    MEM_RS_POLICY_PRIORITY,
    MEM_RS_POLICY_WEIGHTED
} mem_rs_policy_t;

typedef enum {
    MEM_RS_ON_PRESSURE_REJECT = 0,
    MEM_RS_ON_PRESSURE_RECLAIM,
    MEM_RS_ON_PRESSURE_DEGRADE
} mem_rs_pressure_t;

typedef void *(*mem_rs_alloc_fn)(void *ctx, size_t bytes, uint32_t flags);
typedef int   (*mem_rs_free_fn)(void *ctx, void *handle, size_t bytes, uint32_t flags);
typedef size_t(*mem_rs_reclaim_fn)(void *ctx, uint16_t victim_id, size_t want_bytes);
typedef int   (*mem_rs_lock_fn)(void *ctx);
typedef void  (*mem_rs_unlock_fn)(void *ctx);

typedef struct {
    uint16_t          id;
    const char       *name;
    size_t            capacity;
    size_t            safety_margin;
    size_t            used;
    size_t            high_water;
    void             *alloc_ctx;
    mem_rs_alloc_fn   alloc;
    mem_rs_free_fn    free;
    mem_rs_reclaim_fn reclaim;
} mem_rs_resource_t;

typedef struct {
    uint16_t          id;
    const char       *name;
    uint8_t           priority;
    uint8_t           enabled;
    size_t            reserved;
    size_t            quota;
    size_t            used;
    size_t            high_water;
    mem_rs_pressure_t on_pressure;
} mem_rs_consumer_t;

typedef struct {
    uint32_t magic;
    uint16_t resource_id;
    uint16_t consumer_id;
    size_t   bytes;
    uint32_t flags;
    void    *handle;
} mem_rs_token_t;

typedef struct {
    size_t capacity;
    size_t used;
    size_t high_water;
    size_t reserved_total;
    size_t available;
} mem_rs_stats_t;

typedef struct {
    mem_rs_resource_t *resources;
    size_t             resource_count;
    mem_rs_consumer_t *consumers;
    size_t             consumer_count;
    mem_rs_policy_t    policy;
    void              *lock_ctx;
    mem_rs_lock_fn     lock;
    mem_rs_unlock_fn   unlock;
} mem_rs_t;

int    mem_rs_init(mem_rs_t *sched,
                   mem_rs_resource_t *resources, size_t resource_count,
                   mem_rs_consumer_t *consumers, size_t consumer_count,
                   mem_rs_policy_t policy);

int    mem_rs_set_quota(mem_rs_t *sched, uint16_t consumer_id,
                        size_t reserved, size_t quota,
                        mem_rs_pressure_t on_pressure);

int    mem_rs_acquire(mem_rs_t *sched, uint16_t resource_id,
                      uint16_t consumer_id, size_t bytes,
                      uint32_t flags, mem_rs_token_t *token);

int    mem_rs_release(mem_rs_t *sched, mem_rs_token_t *token);

int    mem_rs_reclaim(mem_rs_t *sched, uint16_t resource_id, size_t want_bytes);

size_t mem_rs_available(const mem_rs_t *sched,
                        uint16_t resource_id, uint16_t consumer_id);

int    mem_rs_stats(const mem_rs_t *sched, uint16_t resource_id,
                    mem_rs_stats_t *stats);

int    mem_rs_verify(const mem_rs_t *sched);

#ifdef __cplusplus
}
#endif

#endif /* RESOURCE_SCHEDULER_H */
