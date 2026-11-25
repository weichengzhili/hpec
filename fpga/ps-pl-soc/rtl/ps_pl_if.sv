// ps_pl_if —— 软硬核 SoC / PS-PL 接口
// 对应文档: ../05-fpga/fpga-10-软硬核SoC-PS-PL.md
// 职责: 处理器（软核/硬核 PS）与可编程逻辑（PL）的协同接口。
`timescale 1ns / 1ps

// ---- 软核处理器（MicroBlaze/Nios V/RISC-V）与逻辑的互联 ----
module ps_pl_top (
    input  logic             clk,
    input  logic             rst_n,

    // ---- 处理器侧总线（AXI） ----
    axi_if.master            cpu_axi,     // 处理器 AXI 主端口

    // ---- 中断（PL -> 处理器） ----
    output logic             irq_0,
    output logic             irq_1,

    // ---- 共享内存/DMA 通道 ----
    axi_if.slave             dma_axi,
    output logic             dma_irq
);
    // 软核例化 + 外设互联（见文档软核 vs 硬核）
endmodule

// ---- 软核处理器例化接口（占位） ----
module soft_cpu_wrapper (
    input  logic             clk,
    input  logic             rst_n,
    axi_if.master            m_axi,
    input  logic             ext_irq
);
    // MicroBlaze / Nios V / RISC-V 软核例化
endmodule
