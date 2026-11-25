// serdes_if —— 高速串行接口
// 对应文档: ../05-fpga/fpga-09-高速串行接口.md
// 职责: PCIe/以太网等 SerDes 收发器接口封装。
`timescale 1ns / 1ps

// ---- SerDes 收发器接口 ----
module serdes_if #(
    parameter int LANES = 4,
    parameter int DATA_WIDTH = 32
)(
    input  logic             ref_clk_p,
    input  logic             ref_clk_n,

    // ---- 高速差分收发 ----
    input  logic [LANES-1:0] rx_p,
    input  logic [LANES-1:0] rx_n,
    output logic [LANES-1:0] tx_p,
    output logic [LANES-1:0] tx_n,

    // ---- 链路状态 ----
    output logic             link_up,
    output logic             lane_aligned,
    output logic [7:0]       bit_error_count,

    // ---- 数据接口（AXI-Stream） ----
    axi_stream_if.master     rx_data,
    axi_stream_if.slave      tx_data
);
    // 编码（8b/10b、64b/66b）、时钟恢复（CDR）、均衡见文档
endmodule

// ---- PCIe 接口（Endpoint） ----
module pcie_ep #(
    parameter int LANES = 4
)(
    input  logic [LANES-1:0] rx_p,
    input  logic [LANES-1:0] rx_n,
    output logic [LANES-1:0] tx_p,
    output logic [LANES-1:0] tx_n,
    input  logic             ref_clk_p,
    input  logic             ref_clk_n,
    output logic             link_up,
    // 事务层接 AXI（见 axi_if.sv）
    axi_if.master            axi
);
endmodule
