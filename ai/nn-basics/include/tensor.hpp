// tensor —— 神经网络基础接口
// 对应文档: ../07-ai/ai-02-神经网络基础.md
// 职责: 张量数据结构与布局（NCHW/NHWC）定义。
#pragma once

#include <cstdint>
#include <cstddef>

namespace ai {

// ---- 数据类型 ----
enum class dtype {
    uint8 = 0,
    int8,
    float16,
    float32,
    int16,
    int32,
};

// ---- 张量布局 ----
enum class layout {
    nchw = 0,
    nhwc = 1,   // NPU 偏好
};

// ---- 张量描述 ----
struct tensor {
    std::uint32_t dims[4];       // N/C/H/W
    std::uint32_t n_dims;
    dtype         type;
    layout        layout;
    std::size_t   size_bytes;    // 数据大小
    void         *data;          // 数据缓冲（可 dmabuf 零拷贝）
};

// ---- 张量工具 ----
std::size_t tensor_size(const tensor &t);
bool tensor_reshape(tensor &t, std::uint32_t n, std::uint32_t c,
                    std::uint32_t h, std::uint32_t w, dtype type, layout layout);
bool tensor_convert_layout(const tensor &src, tensor &dst);   // NCHW <-> NHWC

// ---- 算子类型（模型结构描述） ----
enum class op_type {
    conv, pool, activation, normalize, fully_connected, attention,
};

} // namespace ai
