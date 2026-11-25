// robot_system —— 工业机器人系统接口
// 对应文档: ../08-scenarios/scenario-05-工业机器人.md
// 职责: 运动控制、实时总线、视觉伺服、功能安全的系统级接口聚合。
#pragma once

#include <cstdint>
#include <cstddef>

namespace robot {

// ---- 控制环路周期（见文档） ----
enum class control_cycle {
    current_loop = 0,   // µs 级（FPGA/驱动器）
    velocity_loop,      // 快（FPGA）
    position_loop,      // ms 级（RK3588/FPGA）
    trajectory,         // 慢（RK3588）
};

// ---- 机器人系统接口聚合 ----
struct robot_system {
    // 轨迹规划/运动学（见 concurrency/fsm + kernel/realtime）
    struct motion {
        int (*plan)(const double *target, double *trajectory);
        int (*ik_solve)(const double *pose, double *joints);
        int (*interpolate)(const double *trajectory, double *setpoints);
    } motion;

    // 实时总线（EtherCAT，见 fpga/ps-pl-soc）
    struct bus {
        int (*init_master)(void);
        int (*send_command)(int axis, const void *cmd);
        int (*recv_feedback)(int axis, void *fb);
        int (*sync)(void);
    } bus;

    // 编码器/伺服（见 fpga/dsp）
    struct servo {
        int (*read_encoder)(int axis, double *pos, double *vel);
        int (*write_current)(int axis, double current);
    } servo;

    // 视觉伺服（见 ai/vision-pipeline）
    struct vision {
        int (*detect_target)(const void *frame, double *pose);
        int (*servo_correct)(const double *pose, double *correction);
    } vision;

    // 功能安全（安全 IO/急停，独立回路）
    struct safety {
        int (*estop)(void);
        int (*safety_limit)(int axis, double max_pos, double max_vel);
        int (*monitor)(void);
    } safety;
};

int robot_init(robot_system *sys);
int robot_start(robot_system *sys);
int robot_stop(robot_system *sys);
int robot_emergency_stop(robot_system *sys);

} // namespace robot
