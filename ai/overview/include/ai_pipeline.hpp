// ai_pipeline —— 边缘 AI 总览接口
// 对应文档: ../07-ai/ai-01-边缘AI总览.md
// 职责: 端到端推理管线（采集→预处理→推理→后处理）统一接口。
#pragma once

#include <cstdint>
#include <cstddef>

namespace ai {

// ---- 推理结果状态 ----
enum class status {
    ok = 0,
    error = -1,
    timeout = -2,
    busy = -3,
};

// ---- 推理管线接口（统一入口） ----
struct pipeline {
    // 加载模型（见 model_manager）
    virtual int load_model(const void *model, std::size_t size) = 0;

    // 单帧推理（输入 → 输出）
    virtual status infer(const void *input, std::size_t input_size,
                         void *output, std::size_t *output_size) = 0;

    // 异步推理（低延迟场景）
    virtual status infer_async(const void *input, std::size_t input_size) = 0;
    virtual status wait_result(void *output, std::size_t *output_size, std::uint32_t timeout_ms) = 0;

    virtual ~pipeline() = default;
};

// ---- 管线配置 ----
struct pipeline_config {
    std::uint32_t num_streams;      // 多路并发
    bool          zero_copy;        // dmabuf 零拷贝
    std::uint32_t timeout_ms;       // 推理超时
};

// ---- 管线工厂 ----
pipeline *create_pipeline(const pipeline_config &cfg);
void destroy_pipeline(pipeline *p);

} // namespace ai
