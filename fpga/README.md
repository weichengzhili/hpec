# fpga FPGA 逻辑设计

> FPGA 逻辑设计功能模块（SystemVerilog 接口 / 约束文件）。

| 模块 | 功能 | 接口文件 |
|------|------|----------|
| [FPGA开发总览](overview/FPGA开发总览.md) | 顶层接口、工程流程 | rtl/fpga_top.sv |
| [Verilog-SystemVerilog](hdl/Verilog-SystemVerilog.md) | 可综合 RTL 规范 | rtl/module_template.sv |
| [时序分析与收敛](timing/时序分析与收敛.md) | STA、时钟约束 | constraints/timing.sdc |
| [跨时钟域CDC](cdc/跨时钟域CDC.md) | 同步器、异步 FIFO | rtl/cdc_sync.sv |
| [FIFO与片上存储](fifo/FIFO与片上存储.md) | 同步/异步 FIFO、BRAM | rtl/fifo.sv |
| [AXI总线协议](axi/AXI总线协议.md) | AXI4/AXI-Lite/AXI-Stream | rtl/axi_if.sv |
| [高层次综合HLS](hls/高层次综合HLS.md) | HLS IP 封装、优化 | rtl/hls_wrapper.sv |
| [DSP信号处理](dsp/DSP信号处理.md) | FIR/FFT/CORDIC | rtl/fir_filter.sv |
| [高速串行接口](serdes/高速串行接口.md) | SerDes、PCIe、以太网 | rtl/serdes_if.sv |
| [软硬核SoC-PS-PL](ps-pl-soc/软硬核SoC-PS-PL.md) | 软核/硬核协同 | rtl/ps_pl_if.sv |
| [验证与UVM](verification/验证与UVM.md) | 仿真、断言、UVM | tb/dut_if.sv |
| [硬件调试](debug/硬件调试.md) | ILA、信号探测 | rtl/ila_probe.sv |
