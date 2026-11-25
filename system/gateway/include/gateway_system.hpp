// gateway_system —— 工业网关系统接口
// 对应文档: ../08-scenarios/scenario-02-工业网关.md
// 职责: 多协议采集、协议转换、边缘计算、上云的系统级接口聚合。
#pragma once

#include <cstdint>
#include <cstddef>

namespace gw {

// ---- 工业协议类型 ----
enum class protocol {
    modbus_rtu = 0,
    modbus_tcp,
    opc_ua,
    profinet,
    ethercat,
    mqtt,
};

// ---- 网关系统接口聚合 ----
struct gateway_system {
    // 现场采集（串口/网口/CAN，见 platform/io + driver/hal）
    struct field_io {
        int (*open)(int port);
        int (*read)(int port, void *buf, std::size_t *len);
        int (*write)(int port, const void *buf, std::size_t len);
    } field_io;

    // 协议处理（见 concurrency/fsm）
    struct protocol_stack {
        int (*init)(protocol proto);
        int (*parse)(const void *frame, std::size_t len, void *payload);
        int (*encode)(const void *payload, void *frame, std::size_t *len);
    } proto;

    // 边缘计算（过滤/聚合，见 ai 或规则引擎）
    struct edge_compute {
        int (*filter)(const void *data, std::size_t len, void *out, std::size_t *out_len);
        int (*aggregate)(const void *data, std::size_t len, void *out);
    } compute;

    // 上云（MQTT/OPC-UA，见 kernel/net-device）
    struct cloud {
        int (*connect)(const char *endpoint);
        int (*publish)(const char *topic, const void *data, std::size_t len);
        int (*subscribe)(const char *topic);
    } cloud;

    // 断点续传（本地缓存）
    struct buffer {
        int (*store)(const void *data, std::size_t len);
        int (*flush)(void);
    } buffer;
};

int gateway_init(gateway_system *sys);
int gateway_start(gateway_system *sys);
int gateway_stop(gateway_system *sys);

} // namespace gw
