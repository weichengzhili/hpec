// model_manager —— 模型部署与 MLOps 接口
// 对应文档: ../07-ai/ai-07-模型部署与MLOps.md
// 职责: 模型版本管理、下发、A/B 切换、回滚、监控接口。
#pragma once

#include <cstdint>
#include <cstddef>

namespace ai {

// ---- 模型元数据 ----
struct model_meta {
    char        name[64];
    char        version[32];
    char        checksum[64];   // 完整性校验
    std::size_t size;
    float       accuracy;
    std::uint64_t created_at;
};

// ---- 模型槽位（A/B） ----
enum class model_slot { a = 0, b = 1 };

// ---- 模型管理器 ----
class model_manager {
public:
    // 模型注册/查询
    int register_model(const model_meta &meta, const void *data, std::size_t size);
    int query_model(const char *name, model_meta *meta);

    // 下发与校验
    int download_model(const char *url, const char *name, const char *version);
    int verify_model(const char *name, const char *version);   // 签名+哈希校验

    // A/B 原子切换与回滚
    int switch_model(const char *name, const char *version, model_slot target);
    int rollback(const char *name);

    // 加载到推理运行时
    int load_to_engine(const char *name, void *engine);

    // 监控（精度/性能指标上报）
    int report_metric(const char *name, const char *metric, double value);
};

} // namespace ai
