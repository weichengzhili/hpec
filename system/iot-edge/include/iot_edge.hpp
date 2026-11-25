// iot_edge —— IoT 边缘节点系统接口
// 对应文档: ../08-scenarios/scenario-03-IoT边缘节点.md
// 职责: 边缘推理、传感器融合、无线连接、设备管理的系统级接口聚合。
#pragma once

#include <cstdint>
#include <cstddef>

namespace iot {

// ---- 无线连接类型 ----
enum class radio { wifi6 = 0, five_g, ethernet, lora };

// ---- IoT 边缘系统接口聚合 ----
struct iot_edge_system {
    // 边缘推理（见 ai/overview）
    struct infer {
        int (*load_model)(const void *model, std::size_t size);
        int (*infer)(const void *input, std::size_t in_len, void *out, std::size_t *out_len);
    } infer;

    // 传感器融合（多源采集）
    struct sensor {
        int (*read_all)(void *samples, std::size_t *count);
        int (*fuse)(const void *samples, std::size_t count, void *result);
    } sensor;

    // 无线连接（见 platform/io）
    struct radio_if {
        int (*connect)(radio type, const char *config);
        int (*send)(const void *data, std::size_t len);
        int (*recv)(void *data, std::size_t *len);
        int (*status)(radio type);
    } radio;

    // 功耗管理（见 platform/clock-power）
    struct power {
        int (*set_mode)(int mode);      // 0=全速 1=降频 2=待机
        int (*get_power)(double *w);
    } power;

    // 设备管理（注册/上报/OTA）
    struct device_mgmt {
        int (*register_device)(const char *id, const char *credential);
        int (*report_state)(const char *state);
        int (*ota_update)(const char *url);
    } mgmt;
};

int iot_edge_init(iot_edge_system *sys);
int iot_edge_start(iot_edge_system *sys);
int iot_edge_stop(iot_edge_system *sys);

} // namespace iot
