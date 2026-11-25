# GPIO 与引脚复用

> 模块目录：`workspace/platform/gpio-pinctrl/` · 接口文件：`include/gpio.h`

## 1. 模块定位与目标
建立 RK3588 GPIO 与引脚复用（Pinctrl）方案：引脚功能复用配置、输入输出控制、中断、上拉下拉、驱动强度，支撑外设连接与自定义 IO 控制。

**服务场景**：网关开关量采集、机器人 IO 控制、域控离散信号、外设使能。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 复用 | 引脚多功能复用（GPIO/UART/I2C/SPI/PWM） |
| 中断 | GPIO 中断（边沿/电平） |
| 约束 | 复用冲突检测，避免同一引脚被多驱动占用 |

## 3. 架构设计

### 3.1 Pinctrl 子系统
- 引脚组（pin group）定义功能复用。
- 引脚状态（pin state）：default/sleep。
- 设备树声明引脚配置。

### 3.2 GPIO 能力
| 能力 | 说明 |
|------|------|
| 方向 | 输入/输出 |
| 值 | 读写电平 |
| 中断 | 上升/下降/双边沿、高低电平 |
| 电气 | 上拉/下拉、驱动强度 |

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `pin_func_t` | 引脚功能枚举 |
| `pinctrl_set_func()/set_state()` | 复用配置 |
| `gpio_request/release()` | 请求/释放 |
| `gpio_set_direction/set_value/get_value()` | 方向与电平 |
| `gpio_set_pull/set_drive_strength()` | 电气配置 |
| `gpio_request_irq()` | GPIO 中断 |

## 5. 关键流程与时序
驱动 probe → 请求引脚（gpio_request）→ 配置方向/复用 → 使用 → 释放。中断注册需处理去抖与优先级（见 gic）。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 接口 | gpiolib/libgpiod | 裸寄存器 | 统一、可审计 |
| 复用 | 设备树声明 | 硬编码 | 集中管理、冲突可检测 |
| 去抖 | 硬件/软件去抖 | 无 | 消除毛刺误触发 |

## 7. 性能指标（KPI）
- GPIO 翻转速率、中断响应延迟量化。
- 引脚占用无冲突。

## 8. 可靠性设计
- 关键信号去抖、防抖动。
- 中断风暴防护（见 concurrency/interrupt-critical）。

## 9. 安全设计
- 引脚权限控制，防未授权访问（安全 IO）。

## 10. 测试与验证策略
- 引脚复用冲突检测。
- 电平/中断/去抖功能测试。

## 11. 集成与部署
- 引脚分配表纳入板级资源管理。

## 12. 参考资料
- RK3588 GPIO/Pinctrl 章节（TRM）。
- Linux gpiolib/pinctrl 文档。

## 关联模块
- `driver/bit-register`：寄存器访问底层。
- `platform/gic`：GPIO 中断处理。
- `platform/devicetree-bsp`：引脚声明。
