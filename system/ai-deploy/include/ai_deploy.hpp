// ai_deploy —— AI 边缘部署系统接口
// 对应文档: ../08-scenarios/scenario-04-AI边缘部署.md
// 职责: 模型转换/量化、NPU 推理、视觉流水线、MLOps 的系统级接口聚合。
#pragma once

#include <cstdint>
#include <cstddef>

namespace ai_deploy {

// ---- 部署平台接口聚合 ----
struct ai_deploy_system {
    // 模型转换/量化（见 ai/quantization）
    struct convert {
        int (*to_rknn)(const void *model, std::size_t size, void **out, std::size_t *out_size);
        int (*quantize)(const void *model, std::size_t size, void **out, std::size_t *out_size);
    } convert;

    // NPU 推理（见 ai/npu-runtime）
    struct runtime {
        void *(*load)(const void *model, std::size_t size);
        int (*infer)(void *ctx, const void *input, void *output);
        int (*release)(void *ctx);
    } runtime;

    // 视觉流水线（见 ai/vision-pipeline）
    struct vision {
        int (*start)(int streams);
        int (*process)(int stream, const void *frame, void *result);
        int (*stop)(void);
    } vision;

    // MLOps（见 ai/mlops）
    struct mlops {
        int (*deploy)(const char *model, const char *version);
        int (*switch_model)(const char *version);
        int (*rollback)(void);
        int (*report_metric)(const char *metric, double value);
    } mlops;
};

int ai_deploy_init(ai_deploy_system *sys);
int ai_deploy_start(ai_deploy_system *sys);
int ai_deploy_stop(ai_deploy_system *sys);

} // namespace ai_deploy
