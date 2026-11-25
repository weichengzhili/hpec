# AXI 总线协议

> 模块目录：`workspace/fpga/axi/` · 接口文件：`rtl/axi_if.sv`

## 1. 模块定位与目标
建立 AXI 总线（AMBA）协议的设计规范：AXI4（存储器映射）、AXI4-Lite（寄存器）、AXI-Stream（流式），用于模块互连、PS-PL 通信、IP 集成，是 FPGA SoC 互连的标准。

**服务场景**：SoC 模块互连、PS-PL 数据通道、DMA 接口、IP 核封装。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 协议 | AXI4 / AXI4-Lite / AXI-Stream |
| 握手 | VALID/READY 握手 |
| 约束 | 遵循协议时序，避免死锁 |

## 3. 架构设计

### 3.1 协议类型
| 类型 | 用途 |
|------|------|
| AXI4 | 高性能存储映射（突发传输） |
| AXI4-Lite | 简单寄存器访问 |
| AXI-Stream | 流式数据（无地址） |

### 3.2 通道结构
- 地址通道、读数据通道、写数据通道、写响应通道。
- 每通道 VALID/READY 握手。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `axi_lite_if` | AXI4-Lite（AW/W/B/AR/R 通道） |
| `axi_stream_if` | AXI-Stream（TVALID/TREADY/TDATA/TKEEP/TLAST） |
| `modport master/slave` | 方向定义 |

## 5. 关键流程与时序
读：AR 通道发地址 → R 通道返回数据。写：AW 发地址 → W 发数据 → B 返回响应。握手：VALID 且 READY 同时有效才传输。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 互连 | AXI 标准 | 私有 | 生态互通 |
| 寄存器 | AXI-Lite | AXI4 | 简单控制 |
| 流数据 | AXI-Stream | AXI4 | 高吞吐无地址 |

## 7. 性能指标（KPI）
- 总线吞吐、突发效率。
- 握手无死锁。

## 8. 可靠性设计
- 握手协议正确，防死锁。
- 超时/错误响应处理（RRESP/BRESP）。

## 9. 安全设计
- 地址范围检查，防越界访问。

## 10. 测试与验证策略
- 协议检查（断言/VIP）。
- 突发/边界传输测试。

## 11. 集成与部署
- AXI 接口作为模块间标准接口。

## 12. 参考资料
- ARM AMBA AXI 规范。
- 厂商 AXI IP 文档。

## 关联模块
- `fpga/ps-pl-soc`：处理器互联。
- `fpga/hls`：HLS IP 接口。
- `platform/pcie-usb-storage`：与 RK3588 数据通道。
