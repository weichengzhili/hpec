// benchmark —— 性能优化与评测接口
// 对应文档: ../07-ai/ai-08-性能优化与评测.md
// 职责: 延迟/吞吐/精度/功耗/内存指标评测接口。
#pragma once

#include <cstdint>
#include <cstddef>

namespace ai {

// ---- 评测指标 ----
struct benchmark_result {
    double latency_ms;        // 单帧端到端延迟
    double inference_ms;      // 纯推理延迟
    double throughput_fps;    // 吞吐
    double accuracy;          // 精度（mAP/准确率）
    double power_w;           // 功耗
    std::size_t peak_memory;  // 峰值内存
    std::size_t resident_memory; // 常驻内存
};

// ---- 评测配置 ----
struct benchmark_config {
    std::uint32_t warmup_runs;
    std::uint32_t measure_runs;
    bool          measure_power;
    bool          measure_memory;
};

// ---- 基准测试接口 ----
class benchmark {
public:
    // 跑基准
    int run(const void *model, std::size_t size, const benchmark_config &cfg,
            benchmark_result *result);

    // 精度评测
    int evaluate_accuracy(const void *model, std::size_t size,
                          const void *eval_data, std::size_t eval_size,
                          double *accuracy);

    // 生成报告
    int generate_report(const benchmark_result *result, char *report, std::size_t *len);
};

} // namespace ai
