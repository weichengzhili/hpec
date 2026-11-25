// inference_engine —— 推理框架选型接口
// 对应文档: ../07-ai/ai-05-推理框架选型.md
// 职责: 统一推理引擎抽象（框架可替换，屏蔽 RKNN/ONNX Runtime/NCNN 差异）。
#pragma once

#include <cstdint>
#include <cstddef>

namespace ai {

// ---- 框架类型 ----
enum class engine_type {
    rknn = 0,       // RK3588 NPU 最佳性能
    onnx_runtime,   // 通用多后端
    ncnn,           // 轻量 CPU
    tflite,
};

// ---- 引擎能力 ----
struct engine_info {
    engine_type type;
    const char *name;
    const char *version;
    bool has_npu_accel;     // 是否支持 NPU 加速
    bool has_gpu_accel;     // 是否支持 GPU 加速
};

// ---- 统一推理引擎接口 ----
struct inference_engine {
    virtual int init(const engine_info &info) = 0;
    virtual int load_model(const void *model, std::size_t size) = 0;
    virtual int infer(const void *input, std::size_t input_size,
                      void *output, std::size_t *output_size) = 0;
    virtual int get_info(engine_info *info) = 0;
    virtual int release() = 0;
    virtual ~inference_engine() = default;
};

// ---- 引擎工厂（按框架选型） ----
inference_engine *create_engine(engine_type type);
void destroy_engine(inference_engine *engine);

// ---- 基准对比（见 performance） ----
int engine_benchmark(engine_type type, const void *model, std::size_t size,
                     double *latency_ms, double *throughput_fps);

} // namespace ai
