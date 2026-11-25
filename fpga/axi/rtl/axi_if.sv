// axi_if —— AXI 总线协议接口
// 对应文档: ../05-fpga/fpga-06-AXI总线协议.md
// 职责: AXI4 / AXI4-Lite / AXI-Stream 接口定义。
`timescale 1ns / 1ps

// ---- AXI4-Lite（寄存器访问，简单控制） ----
interface axi_lite_if #(
    parameter int ADDR_WIDTH = 32,
    parameter int DATA_WIDTH = 32
)(
    input logic clk,
    input logic rst_n
);
    logic [ADDR_WIDTH-1:0] awaddr;
    logic                  awvalid;
    logic                  awready;
    logic [DATA_WIDTH-1:0] wdata;
    logic [DATA_WIDTH/8-1:0] wstrb;
    logic                  wvalid;
    logic                  wready;
    logic [1:0]            bresp;
    logic                  bvalid;
    logic                  bready;
    logic [ADDR_WIDTH-1:0] araddr;
    logic                  arvalid;
    logic                  arready;
    logic [DATA_WIDTH-1:0] rdata;
    logic [1:0]            rresp;
    logic                  rvalid;
    logic                  rready;

    modport master (output awaddr, awvalid, wdata, wstrb, wvalid, araddr, arvalid, bready, rready,
                    input  awready, wready, bresp, bvalid, arready, rdata, rresp, rvalid);
    modport slave  (input  awaddr, awvalid, wdata, wstrb, wvalid, araddr, arvalid, bready, rready,
                    output awready, wready, bresp, bvalid, arready, rdata, rresp, rvalid);
endinterface

// ---- AXI-Stream（流式数据，无地址） ----
interface axi_stream_if #(
    parameter int DATA_WIDTH = 32
)(
    input logic clk,
    input logic rst_n
);
    logic [DATA_WIDTH-1:0] tdata;
    logic                  tvalid;
    logic                  tready;
    logic [DATA_WIDTH/8-1:0] tkeep;
    logic                  tlast;

    modport master (output tdata, tvalid, tkeep, tlast, input tready);
    modport slave  (input  tdata, tvalid, tkeep, tlast, output tready);
endinterface
