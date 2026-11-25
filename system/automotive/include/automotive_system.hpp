// automotive_system —— 汽车域控系统接口
// 对应文档: ../08-scenarios/scenario-01-汽车域控.md
// 职责: 座舱/ADAS/车身域控的系统级接口聚合（感知/控制/网络/OTA/安全）。
#pragma once

#include <cstdint>

namespace auto_sys {

// ---- 功能安全等级 ----
enum class asil { qm = 0, a, b, c, d };

// ---- 域控子系统接口聚合 ----
struct automotive_system {
    // 感知（NPU 舱内/环视，见 ai/vision-pipeline）
    struct perception {
        int (*init)(void);
        int (*detect)(const void *frame, void *result);
    } perception;

    // 实时控制（PREEMPT_RT，见 kernel/realtime）
    struct control {
        int (*start)(asil level);
        int (*set_task_priority)(int task, int priority);
    } control;

    // 车载网络（TSN/CAN-FD）
    struct vehicle_net {
        int (*init_tsn)(void);
        int (*send_can)(uint32_t id, const void *data, int len);
        int (*recv_can)(uint32_t *id, void *data, int *len);
    } net;

    // 显示（GPU 多路，见 platform/gpu）
    struct display {
        int (*show_cluster)(const void *surface);
        int (*show_ivt)(const void *surface);
    } display;

    // OTA（A/B 升级，见 platform/uboot）
    struct ota {
        int (*download)(const char *url);
        int (*install)(void);
        int (*rollback)(void);
    } ota;

    // 功能安全监控（安全 MCU）
    struct safety {
        int (*watchdog_kick)(void);
        int (*report_status)(int status);
    } safety;
};

// ---- 系统初始化 ----
int automotive_init(automotive_system *sys);
int automotive_start(automotive_system *sys);
int automotive_stop(automotive_system *sys);

} // namespace auto_sys
