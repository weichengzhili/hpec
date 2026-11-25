// module_template —— Verilog/SystemVerilog 编码规范模块模板
// 对应文档: ../05-fpga/fpga-02-Verilog-SystemVerilog.md
// 职责: 可综合 RTL 风格约定（时序 always_ff、组合 always_comb、参数化）。
`timescale 1ns / 1ps

module module_template #(
    parameter int WIDTH = 32,
    parameter bit SYNC_RESET = 1'b1   // 复位策略统一
)(
    input  logic             clk,
    input  logic             rst_n,     // 低有效复位
    input  logic [WIDTH-1:0] din,
    input  logic             din_valid,
    output logic [WIDTH-1:0] dout,
    output logic             dout_valid
);
    // 规范约定：
    // 1. 时序逻辑用 always_ff + 非阻塞赋值 <=
    // 2. 组合逻辑用 always_comb + 阻塞赋值 =
    // 3. 状态机用 enum 编码，覆盖 default 分支
    // 4. 禁止在同一 always 中混用阻塞/非阻塞赋值

    // ---- 状态机（enum 编码示例） ----
    typedef enum logic [1:0] {
        ST_IDLE = 2'b00,
        ST_BUSY = 2'b01,
        ST_DONE = 2'b10
    } state_t;

    state_t state_q, state_d;
    // 实现待填充
endmodule
