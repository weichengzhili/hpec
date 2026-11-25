# 启动流程与 U-Boot

> 模块目录：`workspace/platform/uboot/` · 接口文件：`include/boot.h`

## 1. 模块定位与目标
建立 RK3588 完整启动链路与引导方案：ROM → 一级引导（TPL/SPL）→ U-Boot → 内核 → 根文件系统，涵盖安全启动、固件分区布局、A/B 冗余升级。是平台可靠性与安全性的根基。

**服务场景**：域控安全启动、IoT 边缘 A/B OTA、网关快速启动。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 启动链 | ROM → BootROM → SPL/TPL → U-Boot → Kernel → Rootfs |
| 安全 | 安全启动（Secure Boot）校验签名 |
| 升级 | A/B 分区冗余，失败回退 |
| 约束 | 启动时间、防变砖（brick）是硬指标 |

## 3. 架构设计

### 3.1 启动阶段划分
| 阶段 | 载体 | 职责 |
|------|------|------|
| BootROM | 芯片内 ROM | 初始化最小外设，加载 TPL/SPL |
| TPL/SPL | SRAM/DRAM | 初始化 DDR、加载 U-Boot |
| U-Boot | DRAM | 初始化外设、加载内核/设备树 |
| Kernel | DRAM | 系统启动 |
| Rootfs | 存储 | 挂载根文件系统 |

### 3.2 分区布局
```
idbloader/bootloader → misc/dtbo → boot_a/boot_b → system_a/system_b → vendor/userdata
```

### 3.3 安全启动
- eFuse 烧录根公钥/哈希。
- 逐级校验：BootROM 验 SPL，SPL 验 U-Boot，U-Boot 验内核/根文件系统。
- 失败进入恢复模式。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `boot_stage_t` | 启动阶段枚举 |
| `boot_slot_t` | A/B 槽位 |
| `boot_partition_layout_t` | 分区布局标识 |
| `boot_verify_image()` | 信任链校验 |
| `boot_get_current_slot()/switch_slot()` | A/B 槽位管理 |
| `boot_enter_recovery()` | 恢复模式 |
| `boot_get_kernel_cmdline()/set_dtb_load_addr()` | 启动参数 |

## 5. 关键流程与时序
上电 → BootROM → 校验加载 SPL → SPL 初始化 DDR → 加载 U-Boot → U-Boot 选 A/B 槽 → 加载内核+DTB → 内核启动 → 挂载 rootfs → 进入系统。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 升级 | A/B 分区 | 单分区 | 失败可回退 |
| 安全 | 逐级签名校验 | 无 | 建立信任链 |
| 恢复 | 独立 recovery | 无 | 防变砖 |

## 7. 性能指标（KPI）
- 冷启动到系统可用时间量化。
- 升级失败回退成功率 100%。
- 安全启动校验覆盖全链。

## 8. 可靠性设计
- A/B 回退：启动失败自动切换槽位。
- 看门狗贯穿启动各阶段。
- 镜像完整性校验（哈希/签名）。

## 9. 安全设计
- 安全启动信任链（Root of Trust）。
- 启动日志与回退记录审计。
- 调试口安全控制。

## 10. 测试与验证策略
- 启动时序测试。
- 故障注入：镜像损坏、签名失败、DDR 故障。
- A/B 升级回退测试。

## 11. 集成与部署
- 固件签名流程纳入 CI/CD。
- 分区表版本化，升级兼容性管理。

## 12. 参考资料
- Rockchip U-Boot 源码与文档。
- RK3588 安全启动白皮书。

## 关联模块
- `boot/startup`：MCU 启动（对比）。
- `platform/devicetree-bsp`：设备树加载。
- `system/automotive`：域控安全启动应用。
