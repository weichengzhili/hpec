// cdc_sync —— 跨时钟域 CDC 接口
// 对应文档: ../05-fpga/fpga-04-跨时钟域CDC.md
// 职责: 亚稳态消除（双级同步器）、异步 FIFO 跨域接口。
`timescale 1ns / 1ps

// ---- 单 bit 同步器（双级触发器消除亚稳态） ----
module cdc_bit_sync (
    input  logic clk_dst,     // 目标时钟域
    input  logic rst_n_dst,
    input  logic signal_src,  // 源时钟域信号
    output logic signal_dst   // 同步后的目标域信号
);
    // 实现：两级 FF 串联（见文档）
endmodule

// ---- 多 bit 数据跨域（异步 FIFO，格雷码指针） ----
module cdc_async_fifo #(
    parameter int WIDTH = 32,
    parameter int DEPTH = 16
)(
    // 写侧（源时钟域）
    input  logic             wr_clk,
    input  logic             wr_rst_n,
    input  logic             wr_en,
    input  logic [WIDTH-1:0] wr_data,
    output logic             full,
    // 读侧（目标时钟域）
    input  logic             rd_clk,
    input  logic             rd_rst_n,
    input  logic             rd_en,
    output logic [WIDTH-1:0] rd_data,
    output logic             empty
);
    // 实现：双端口 RAM + 格雷码读写指针同步（见文档）
endmodule

// ---- 复位同步器 ----
module cdc_reset_sync (
    input  logic clk,
    input  logic rst_async,   // 异步复位输入
    output logic rst_sync     // 同步释放的复位
);
endmodule
