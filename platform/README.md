# platform RK3588 平台

> RK3588 SoC 平台功能模块（寄存器定义、驱动接口、设备树）。

| 模块 | 功能 | 接口文件 |
|------|------|----------|
| [SoC架构总览](soc-arch/SoC架构总览.md) | 基址、资源分配、子系统 | include/soc.h |
| [启动流程与U-Boot](uboot/启动流程与U-Boot.md) | 多级启动、安全启动、A/B | include/boot.h |
| [时钟与电源管理](clock-power/时钟与电源管理.md) | CRU、功耗域、DVFS | include/cru.h |
| [GPIO与引脚复用](gpio-pinctrl/GPIO与引脚复用.md) | Pinctrl、GPIO 控制、中断 | include/gpio.h |
| [中断与GIC](gic/中断与GIC.md) | GIC-600、SPI/PPI/SGI | include/gic.h |
| [DMA引擎](dma-engine/DMA引擎.md) | DMA 通道、缓存一致性 | include/dma_engine.h |
| [NPU与AI加速](npu/NPU与AI加速.md) | RKNN、6TOPS、张量 | include/npu.h |
| [GPU与多媒体](gpu-multimedia/GPU与多媒体.md) | Mali GPU、多路显示 | include/gpu.h |
| [MIPI-CSI相机](mipi-csi/MIPI-CSI相机.md) | 采集流水线、ISP、V4L2 | include/csi.h |
| [视频编解码](video-codec/视频编解码.md) | 8K 编解码、VPU | include/codec.h |
| [PCIe-USB-存储](pcie-usb-storage/PCIe-USB-存储.md) | PCIe/USB/存储接口 | include/io.h |
| [设备树与BSP](devicetree-bsp/设备树与BSP.md) | DTS 分层、板级定制 | dts/rk3588-board.dtsi |
| [驱动开发](driver-dev/驱动开发.md) | platform/字符设备驱动 | include/driver.h |
