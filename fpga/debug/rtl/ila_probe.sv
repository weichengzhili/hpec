// ila_probe —— 硬件调试接口
// 对应文档: ../05-fpga/fpga-12-硬件调试.md
// 职责: 片内逻辑分析仪（ILA）探点接口约定。
`timescale 1ns / 1ps

// ILA 探点封装：将关键信号引出供片内逻辑分析仪抓取
module ila_probe #(
    parameter int PROBE_WIDTH = 64
)(
    input  logic             clk,           // ILA 采样时钟
    input  logic [PROBE_WIDTH-1:0] probe,  // 被观测信号
    input  logic             trigger       // 触发条件
);
    // 生产版本需移除/禁用（见文档安全设计）
    // 例化厂商 ILA IP：设置触发条件、采集深度
endmodule

// ---- 调试约定 ----
// 1. 探点选取关键信号（状态机状态、握手、中断）。
// 2. 调试逻辑资源开销可控，生产版本裁剪。
// 3. JTAG 调试链路，生产禁用防逆向。
