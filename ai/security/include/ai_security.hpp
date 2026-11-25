// ai_security —— 边缘 AI 安全与隐私接口
// 对应文档: ../07-ai/ai-09-边缘AI安全与隐私.md
// 职责: 模型保护（加密/签名）、数据隐私、对抗防护接口。
#pragma once

#include <cstdint>
#include <cstddef>

namespace ai {

// ---- 模型保护 ----
int ai_model_encrypt(const void *model, std::size_t size, const void *key,
                     void **out, std::size_t *out_size);
int ai_model_decrypt(const void *model, std::size_t size, const void *key,
                     void **out, std::size_t *out_size);
int ai_model_sign(const void *model, std::size_t size, void *signature, std::size_t *sig_size);
int ai_model_verify(const void *model, std::size_t size,
                    const void *signature, std::size_t sig_size);   // 0 = 通过

// ---- 数据隐私 ----
int ai_data_anonymize(void *data, std::size_t size);   // 脱敏（人脸/车牌模糊）
int ai_data_minimize(void *data, std::size_t *size);   // 最小化（裁剪敏感字段）

// ---- 对抗攻击防护 ----
int ai_input_validate(const void *input, std::size_t size, bool *valid);   // 输入校验
int ai_adversarial_detect(const void *input, std::size_t size, float *score);

// ---- 供应链审计 ----
int ai_model_source_verify(const void *model, std::size_t size, const char *source_meta);

} // namespace ai
