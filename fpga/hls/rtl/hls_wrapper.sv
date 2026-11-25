// hls_wrapper —— 高层次综合 HLS 接口
// 对应文档: ../05-fpga/fpga-07-高层次综合HLS.md
// 职责: HLS 导出 IP 的封装接口（AXI-Stream + 控制），供集成。
`timescale 1ns / 1ps

// HLS 导出的算法模块通常具备：
//   - AXI-Stream 数据输入/输出
//   - AXI-Lite 控制寄存器（启动/状态）
//   - ap_start/ap_done/ap_idle 控制握手
module hls_wrapper #(
    parameter int DATA_WIDTH = 32
)(
    input  logic             clk,
    input  logic             rst_n,

    // ---- 控制握手（HLS 约定） ----
    input  logic             ap_start,
    output logic             ap_done,
    output logic             ap_idle,
    output logic             ap_ready,

    // ---- 数据流（AXI-Stream） ----
    axi_stream_if.slave      data_in,
    axi_stream_if.master     data_out,

    // ---- 配置（AXI-Lite） ----
    axi_lite_if.slave        ctrl
);
    // 例化 HLS 导出的 RTL IP（见文档综合流程）
endmodule
