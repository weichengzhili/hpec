// fifo —— FIFO 与片上存储接口
// 对应文档: ../05-fpga/fpga-05-FIFO与片上存储.md
// 职责: 同步/异步 FIFO 接口声明（满/空/水线流控）。
`timescale 1ns / 1ps

// ---- 同步 FIFO（同时钟读写） ----
module sync_fifo #(
    parameter int WIDTH = 32,
    parameter int DEPTH = 16,
    parameter bit FWFT = 1'b0   // First-Word Fall-Through
)(
    input  logic             clk,
    input  logic             rst_n,
    input  logic             wr_en,
    input  logic [WIDTH-1:0] din,
    output logic             full,
    output logic             almost_full,   // 水线预警
    input  logic             rd_en,
    output logic [WIDTH-1:0] dout,
    output logic             empty,
    output logic             almost_empty
);
endmodule

// ---- 异步 FIFO（跨时钟域，见 cdc_async_fifo） ----
// 接口见 cdc/rtl/cdc_sync.sv 的 cdc_async_fifo

// ---- 片上存储（BRAM 封装） ----
module bram #(
    parameter int WIDTH = 32,
    parameter int DEPTH = 1024
)(
    input  logic             clk,
    input  logic             en,
    input  logic             we,
    input  logic [$clog2(DEPTH)-1:0] addr,
    input  logic [WIDTH-1:0] din,
    output logic [WIDTH-1:0] dout
);
endmodule
