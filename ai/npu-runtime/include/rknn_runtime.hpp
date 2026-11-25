// rknn_runtime —— NPU 推理运行时接口
// 对应文档: ../07-ai/ai-04-NPU推理运行时.md
// 职责: RKNN Runtime 的 C++ 封装（加载/推理/释放，多模型管理）。
#pragma once

#include <cstdint>
#include <cstddef>

namespace ai {

// ---- 运行时句柄 ----
using rknn_context = void *;

// ---- 张量描述（见 tensor.hpp） ----
struct tensor;
struct tensor_attr {
    std::uint32_t dims[4];
    std::uint32_t n_dims;
    std::uint32_t type;      // 对应 dtype
    std::uint32_t layout;    // 对应 layout
    std::size_t   size;
};

// ---- 运行时接口 ----
class rknn_runtime {
public:
    // 加载 .rknn 模型（可多模型）
    int load(const void *model_data, std::size_t model_size, rknn_context *ctx);
    int unload(rknn_context ctx);

    // 输入输出张量属性
    int get_input_attr(rknn_context ctx, int index, tensor_attr *attr);
    int get_output_attr(rknn_context ctx, int index, tensor_attr *attr);

    // 设置输入/输出（零拷贝 DMA）
    int set_input(rknn_context ctx, int index, const tensor &t);
    int set_output(rknn_context ctx, int index, const tensor &t);

    // 推理
    int inference(rknn_context ctx);

    // 模型完整性校验
    int verify_model(const void *model_data, std::size_t model_size);
};

} // namespace ai
