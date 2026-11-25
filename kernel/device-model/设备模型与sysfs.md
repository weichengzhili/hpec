# 设备模型与 sysfs

> 模块目录：`workspace/kernel/device-model/` · 接口文件：`include/device_model.h`

## 1. 模块定位与目标
建立 Linux 设备模型方案：总线/设备/驱动三层抽象、设备与驱动的匹配、sysfs 设备信息暴露，支撑驱动统一管理与用户态访问。

**服务场景**：所有驱动的注册与匹配，sysfs 设备配置/状态查询。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 模型 | bus / device / driver 抽象 |
| 匹配 | 设备树 compatible / 总线 ID |
| 暴露 | sysfs 提供设备属性 |

## 3. 架构设计

### 3.1 三层模型
| 层 | 说明 |
|----|------|
| bus | 总线（platform/I2C/SPI/USB/PCI） |
| device | 设备实例（来自 DT 或枚举） |
| driver | 驱动（匹配后 probe） |

### 3.2 sysfs
- 设备树、属性、状态通过 `/sys` 暴露。
- 设备属性（attribute）供用户态读写。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `device_t/device_driver_t` | 设备/驱动抽象 |
| `bus_register()/device_register()/driver_register()` | 注册 |
| `bus_match_driver()` | 匹配 |
| `device_attribute_t` | sysfs 属性 |
| `sysfs_create_group/remove_group()` | 属性组 |
| `uevent_send()` | 热插拔通知 |

## 5. 关键流程与时序
设备注册 → 总线匹配驱动 → 调用 driver probe → 驱动初始化 → 暴露 sysfs 属性。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 匹配 | compatible（DT） | 总线 ID | 平台驱动标准 |
| 暴露 | sysfs 属性 | ioctl | 标准用户态接口 |
| 热插拔 | uevent | 轮询 | 动态设备管理 |

## 7. 性能指标（KPI）
- 驱动 probe 成功率。
- sysfs 访问开销可控。

## 8. 可靠性设计
- 驱动加载/卸载正确清理。
- 属性访问并发安全。

## 9. 安全设计
- sysfs 属性权限控制。

## 10. 测试与验证策略
- 驱动绑定/解绑测试。
- sysfs 属性读写测试。

## 11. 集成与部署
- 设备树与驱动匹配纳入 BSP 验证。

## 12. 参考资料
- Linux 设备模型文档（driver-model）。
- sysfs 文档。

## 关联模块
- `kernel/char-device`：字符设备（基于设备模型）。
- `kernel/platform-driver`：平台驱动匹配。
- `platform/devicetree-bsp`：设备树描述。
