# PCIe / USB / 存储

> 模块目录：`workspace/platform/pcie-usb-storage/` · 接口文件：`include/io.h`

## 0. 问题定位

- **解决什么问题**：PCIe/USB/存储的 IO 数据流如何正确传输，热插拔如何安全。
- **边界标准**：
  1. IO 数据 buffer 的 cache 一致性正确（硬约束）。
  2. 传输错误/超时处理完整（硬约束）。
  3. 热插拔/拔插生命周期安全，无 use-after-free（硬约束）。
- **系统位置**：环 5；`dma-engine` → 本模块。

## 1. 模块定位与目标
建立 RK3588 高速外设扩展与存储方案：PCIe 3.0（接 FPGA/NVMe/网卡）、USB 3.1、eMMC/SD/SATA 存储，支撑大带宽扩展与数据持久化。

**服务场景**：网关接 FPGA 加速卡、NVMe 高速存储、USB 外设/摄像头、eMMC 系统存储。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| PCIe | Gen3，支持 RC/EP 模式 |
| USB | USB 3.1 Gen1，Host/Device/OTG |
| 存储 | eMMC/SD/NVMe/SATA |
| 约束 | 高速信号完整性、供电 |

## 3. 架构设计

### 3.1 接口规划
| 接口 | 用途 | 典型设备 |
|------|------|----------|
| PCIe | 扩展 | FPGA、NVMe SSD、万兆网卡 |
| USB | 外设 | 摄像头、键盘、U 盘 |
| eMMC/SD | 系统存储 | 根文件系统 |
| NVMe/SATA | 大容量存储 | 日志/数据 |

### 3.2 存储层次
- 引导存储（eMMC/SD）→ 系统分区（见 uboot）。
- 数据存储（NVMe/SATA）→ 日志与业务数据。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `pcie_mode_t` | PCIe 模式（RC/EP） |
| `pcie_init/enum_devices/config_read/write()` | PCIe 操作 |
| `usb_mode_t` | USB 模式 |
| `usb_init/detect_device()` | USB 操作 |
| `storage_type_t` | 存储类型 |
| `storage_get_info/read/write()` | 存储访问 |

## 5. 关键流程与时序
PCIe 枚举 → 配置设备 → 驱动加载；USB 枚举 → 配置描述符 → 驱动绑定；存储挂载 → 文件系统 → 应用访问。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| PCIe | RC 接 FPGA/存储 | EP | 高带宽低延迟 |
| 系统盘 | eMMC | NVMe | 集成、成本 |
| 数据盘 | NVMe | SATA | 高速大容量 |

## 7. 性能指标（KPI）
- PCIe/USB 带宽、存储读写 IOPS/吞吐。
- 链路稳定性（误码率）。

## 8. 可靠性设计
- 存储掉电保护、坏块管理。
- 热插拔（USB/PCIe）正确处理。

## 9. 安全设计
- 存储加密（可选）、访问控制。
- PCIe 设备隔离（SMMU）。

## 10. 测试与验证策略
- 高速信号完整性测试。
- 存储读写压力与掉电测试。

## 11. 集成与部署
- 存储分区与容量规划纳入部署。

## 12. 参考资料
- RK3588 PCIe/USB/存储章节（TRM）。
- Linux PCIe/USB/块设备文档。

## 关联模块
- `kernel/block-device`：块设备层。
- `fpga/serdes`：FPGA 通过 PCIe 互联。
- `system/gateway`：FPGA 加速卡应用。
