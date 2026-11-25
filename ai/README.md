# ai 边缘 AI

> 边缘 AI 推理功能模块（C++ 接口声明）。

| 模块 | 功能 | 接口文件 |
|------|------|----------|
| [边缘AI总览](overview/边缘AI总览.md) | 推理管线、云边协同 | include/ai_pipeline.hpp |
| [神经网络基础](nn-basics/神经网络基础.md) | 张量、算子、网络结构 | include/tensor.hpp |
| [模型量化](quantization/模型量化.md) | INT8/FP16、校准、混合量化 | include/quantization.hpp |
| [NPU推理运行时](npu-runtime/NPU推理运行时.md) | RKNN Runtime、多模型 | include/rknn_runtime.hpp |
| [推理框架选型](framework/推理框架选型.md) | 框架对比、统一引擎 | include/inference_engine.hpp |
| [视觉处理流水线](vision-pipeline/视觉处理流水线.md) | 采集→预处理→推理→后处理 | include/vision_pipeline.hpp |
| [模型部署与MLOps](mlops/模型部署与MLOps.md) | 版本管理、A/B、回滚 | include/model_manager.hpp |
| [性能优化与评测](performance/性能优化与评测.md) | 延迟/吞吐/精度/功耗 | include/benchmark.hpp |
| [边缘AI安全与隐私](security/边缘AI安全与隐私.md) | 模型保护、隐私、对抗 | include/ai_security.hpp |
