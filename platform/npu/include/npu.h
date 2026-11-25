/*
 * npu —— RK3588 NPU 与 AI 加速接口
 * 对应文档: ../04-rk3588/rk3588-07-NPU与AI加速.md
 * 职责: RKNN 运行时接口声明（与 ai/npu-runtime 联动）。
 */
#ifndef RK3588_NPU_H
#define RK3588_NPU_H

#include <stdint.h>
#include <stddef.h>

/* ---- 张量数据类型 ---- */
typedef enum {
    NPU_TENSOR_UINT8 = 0,
    NPU_TENSOR_INT8,
    NPU_TENSOR_FLOAT16,
    NPU_TENSOR_FLOAT32,
    NPU_TENSOR_INT16,
    NPU_TENSOR_INT32,
} npu_tensor_type_t;

/* ---- 张量布局 ---- */
typedef enum {
    NPU_LAYOUT_NCHW = 0,
    NPU_LAYOUT_NHWC,
} npu_layout_t;

/* ---- 张量描述 ---- */
typedef struct {
    uint32_t dims[4];
    uint32_t n_dims;
    npu_tensor_type_t type;
    npu_layout_t layout;
    size_t   size;
    void    *buf;          /* 零拷贝 DMA 缓冲 */
} npu_tensor_t;

/* ---- 模型句柄 ---- */
typedef struct npu_ctx npu_ctx_t;

/* ---- RKNN 运行时接口（对应 ai/npu-runtime） ---- */
npu_ctx_t *npu_init(const void *model_data, size_t model_size);
int npu_get_input_count(npu_ctx_t *ctx);
int npu_get_output_count(npu_ctx_t *ctx);
int npu_get_input_attr(npu_ctx_t *ctx, int index, npu_tensor_t *attr);
int npu_get_output_attr(npu_ctx_t *ctx, int index, npu_tensor_t *attr);
int npu_set_input(npu_ctx_t *ctx, int index, const npu_tensor_t *tensor);
int npu_set_output(npu_ctx_t *ctx, int index, const npu_tensor_t *tensor);
int npu_inference(npu_ctx_t *ctx);
int npu_destroy(npu_ctx_t *ctx);

/* ---- 模型完整性校验 ---- */
int npu_model_verify(const void *model_data, size_t model_size);

#endif /* RK3588_NPU_H */
