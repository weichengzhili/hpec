// dut_if —— 验证与 UVM 接口
// 对应文档: ../05-fpga/fpga-11-验证与UVM.md
// 职责: 测试平台与 DUT 的连接接口、断言、UVM 组件约定。
`timescale 1ns / 1ps

// ---- 验证接口（连接 DUT 与验证平台） ----
interface dut_if (
    input logic clk,
    input logic rst_n
);
    // 数据与控制信号
    logic             din_valid;
    logic [31:0]      din;
    logic             dout_valid;
    logic [31:0]      dout;
    logic             busy;

    // ---- 时钟块（驱动/采样时序约定） ----
    clocking cb @(posedge clk);
        output din_valid, din;
        input  dout_valid, dout, busy;
    endclocking

    // ---- 断言（协议/时序违例监控） ----
    property data_valid_stable;
        @(posedge clk) din_valid |-> din inside {[0:32'hFFFFFFFF]};
    endproperty
    assert property (data_valid_stable);

    modport tb (clocking cb, output rst_n);
    modport dut (input din_valid, din, output dout_valid, dout, busy);
endinterface

// ---- UVM 组件约定（driver/monitor/scoreboard/sequence） ----
// 验证平台通过 dut_if 连接 DUT，约束随机激励 + 覆盖率模型（见文档）
