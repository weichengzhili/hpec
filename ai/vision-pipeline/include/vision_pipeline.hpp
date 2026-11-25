// vision_pipeline —— 视觉处理流水线接口
// 对应文档: ../07-ai/ai-06-视觉处理流水线.md
// 职责: 采集→预处理→推理→后处理端到端流水线接口。
#pragma once

#include <cstdint>
#include <cstddef>

namespace ai {

// ---- 检测结果 ----
struct detection_box {
    float x, y, w, h;          // 归一化坐标
    float confidence;
    int   class_id;
};

struct vision_result {
    detection_box *boxes;
    std::size_t    count;
};

// ---- 流水线阶段回调 ----
struct pipeline_stages {
    // 预处理（缩放/颜色转换/归一化）
    int (*preprocess)(const void *frame, int frame_fd, void *input_tensor);
    // 后处理（解码/NMS/跟踪）
    int (*postprocess)(const void *output_tensor, vision_result *result);
};

// ---- 视觉流水线 ----
class vision_pipeline {
public:
    struct config {
        std::uint32_t width;
        std::uint32_t height;
        std::uint32_t num_streams;   // 多路
        bool          zero_copy;     // dmabuf 零拷贝
    };

    int init(const config &cfg, const pipeline_stages &stages);
    int start();
    int stop();

    // 处理一帧（阻塞）
    int process_frame(const void *frame, int frame_fd, vision_result *result);

    // 处理一帧（异步）
    int process_frame_async(const void *frame, int frame_fd);
    int get_result(vision_result *result, std::uint32_t timeout_ms);

    ~vision_pipeline();
};

} // namespace ai
