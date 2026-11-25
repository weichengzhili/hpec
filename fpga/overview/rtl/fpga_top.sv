// fpga_top —— FPGA 开发总览顶层接口
// 对应文档: ../05-fpga/fpga-01-FPGA开发总览.md
// 职责: 顶层模块接口声明（时钟/复位/主要业务接口）。
`timescale 1ns / 1ps

module fpga_top #(
    parameter int DATA_WIDTH = 32
)(
    // ---- 时钟与复位 ----
    input  logic                    clk,
    input  logic                    rst_n,

    // ---- 高速串行（SerDes） ----
    input  logic                    serdes_rx_p,
    input  logic                    serdes_rx_n,
    output logic                    serdes_tx_p,
    output logic                    serdes_tx_n,

    // ---- AXI 接口（连 RK3588，见 axi_if.sv） ----
    axi_if.slave                    axi_s,

    // ---- 实时接口（编码器/EtherCAT） ----
    input  logic                    enc_a,
    input  logic                    enc_b,
    input  logic                    enc_z,
    output logic                    ethercat_irq,

    // ---- 通用 GPIO ----
    inout  logic [15:0]             gpio
);
    // 实现待填充（见文档架构设计）
endmodule
