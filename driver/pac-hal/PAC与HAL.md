# PAC 与 HAL

> 模块目录：`workspace/driver/pac-hal/` · 接口文件：`src/lib.rs`

## 0. 问题定位

- **解决什么问题**：外设寄存器如何从单一数据源生成类型安全访问，杜绝手写地址错误。
- **边界标准**：
  1. 寄存器访问层由 SVD/IP-XACT 生成，禁止手写（硬约束）。
  2. 生成的 API 类型安全（字段级、位宽检查）（硬约束）。
  3. 生成物与源数据源漂移通过 CI diff 检测（硬约束）。
- **系统位置**：环 3；`bit-register` → 本模块 → `driver-dev`；横切环 10。

## 1. 模块定位与目标
用 PAC（外设访问层）与 HAL（硬件抽象层）规范寄存器与外设访问：PAC 提供类型安全寄存器映射，HAL 提供嵌入式设备抽象。遵循 embedded-hal 生态，实现跨平台驱动复用。

**服务场景**：RK3588/MCU 外设驱动、传感器/通信外设驱动、跨芯片移植。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| PAC | 由 SVD 生成，类型安全寄存器访问 |
| HAL | 遵循 embedded-hal trait 约定 |
| 约束 | unsafe 集中在 PAC 层，HAL 提供安全 API |

## 3. 架构设计
```
驱动(embedded-hal trait 使用者)
      ↓
HAL(embedded-hal 实现，安全 API)
      ↓
PAC(寄存器访问，unsafe 边界)
      ↓
硬件外设
```

### 3.1 embedded-hal 核心 trait
| trait | 用途 |
|-------|------|
| `OutputPin/InputPin` | GPIO |
| `SerialRead/SerialWrite` | UART |
| `SpiBus` | SPI |
| `I2c` | I2C |
| `DelayNs` | 延时 |

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `pac::read_reg/write_reg/modify_reg` | 寄存器访问（unsafe 收口） |
| `OutputPin/InputPin` | GPIO trait |
| `SerialRead/SerialWrite` | 串口 trait |
| `SpiBus` | SPI trait |
| `I2c` | I2C trait |
| `DelayNs` | 延时 trait |

## 5. 关键流程与时序
驱动通过 trait 调用 HAL → HAL 调用 PAC 读写寄存器 → PAC 执行 MMIO 访问。unsafe 只在 PAC 边界出现。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 寄存器访问 | PAC（SVD 生成） | 手写 | 类型安全、防错位 |
| 抽象 | embedded-hal | 自定义 trait | 生态互通、驱动复用 |
| unsafe | 收口 PAC | 散落 | 最小化不安全面 |

## 7. 性能指标（KPI）
- 驱动跨芯片复用（仅换 HAL）。
- unsafe 代码集中在 PAC 层，可审计。

## 8. 可靠性设计
- 寄存器字段类型安全防错位写入。
- 状态机外设封装保证正确使用顺序。

## 9. 安全设计
- unsafe 边界最小化（见 foundation/rust-unsafe）。
- 外设独占所有权防止冲突访问。

## 10. 测试与验证策略
- HAL 用 mock 外设单测。
- PAC 寄存器访问仿真测试。

## 11. 集成与部署
- 依赖 embedded-hal 版本锁定，生态兼容。

## 12. 参考资料
- embedded-hal 规范。
- SVD2Rust、svd2pac 工具。

## 关联模块
- `driver/hal`：C++ 版 HAL 抽象。
- `foundation/rust-unsafe`：PAC 的 unsafe 边界。
- `driver/bit-register`：寄存器字段操作思想。
