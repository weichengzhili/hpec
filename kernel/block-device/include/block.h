/*
 * block-device —— 块设备驱动接口
 * 对应文档: ../06-kernel/kernel-09-块设备驱动.md
 * 职责: 请求队列（blk-mq）、块设备注册、IO 接口。
 */
#ifndef KERNEL_BLOCK_H
#define KERNEL_BLOCK_H

#include <stdint.h>
#include <stddef.h>

/* ---- IO 请求 ---- */
typedef enum {
    BIO_READ = 0,
    BIO_WRITE,
    BIO_FLUSH,
    BIO_DISCARD,
} bio_op_t;

typedef struct {
    bio_op_t op;
    uint64_t sector;
    uint32_t sector_count;
    void    *buffer;
} bio_request_t;

/* ---- 请求队列 ---- */
typedef struct request_queue request_queue_t;

request_queue_t *blk_mq_init_queue(uint32_t max_depth);
void blk_mq_destroy_queue(request_queue_t *q);

/* ---- 块设备注册 ---- */
typedef struct gendisk {
    const char *name;
    uint32_t major;
    uint32_t first_minor;
    uint64_t capacity_sectors;
    uint32_t sector_size;
    request_queue_t *queue;
} gendisk_t;

int block_device_register(gendisk_t *disk);
void block_device_unregister(gendisk_t *disk);

/* ---- 请求处理（驱动回调） ---- */
typedef void (*bio_handler_t)(bio_request_t *req, void *arg);
void block_queue_request_handler(request_queue_t *q, bio_handler_t handler, void *arg);
void block_complete_request(bio_request_t *req, int error);

#endif /* KERNEL_BLOCK_H */
