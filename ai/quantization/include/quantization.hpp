// quantization —— 模型量化接口
// 对应文档: ../07-ai/ai-03-模型量化.md
// 职责: FP32 → INT8/FP16 量化、校准、混合量化接口。
#pragma once

#include <cstdint>
#include <cstddef>

namespace ai {

// ---- 量化类型 ----
enum class quant_type {
    none = 0,      // FP32
    fp16,
    int8,          // NPU 高效
    mixed,         // 混合量化（敏感层高精度）
};

// ---- 量化配置 ----
struct quant_config {
    quant_type type = quant_type::int8;
    const void *calib_data = nullptr;      // 校准数据集
    std::size_t calib_size = 0;
    float       accuracy_target = 0.99f;   // 精度目标（相对 FP32）
};

// ---- 量化结果统计 ----
struct quant_result {
    std::size_t orig_size;      // 原始模型大小
    std::size_t quant_size;     // 量化后大小
    float       accuracy;       // 量化后精度
    bool        acceptable;     // 是否达标
};

// ---- 量化接口 ----
int quantize_model(const void *fp32_model, std::size_t fp32_size,
                   const quant_config &cfg, void **out_model, std::size_t *out_size);

int quant_evaluate_accuracy(const void *model, std::size_t size,
                            const void *eval_data, std::size_t eval_size,
                            float *accuracy);

// ---- 混合量化（指定敏感层保持高精度） ----
int quant_set_sensitive_layers(const char *const *layer_names, std::size_t count);

} // namespace ai
