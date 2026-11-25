# 嵌入式 Rust 规范

> 模块目录：`workspace/foundation/rust-guidelines/` · 接口文件：`src/lib.rs`

## 1. 模块定位与目标
定义嵌入式 Rust 的工程化规范：稳定版约束、生态 crate 选型、clippy 规则、panic/unsafe 策略、目录组织，使团队代码一致、可审计、可长期维护。

**服务场景**：所有 Rust 嵌入式项目的工程治理。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 工具链 | 锁定 stable 版本（或明确 nightly 特性） |
| 生态 | 选型嵌入式 crate（embedded-hal 等） |
| 约束 | clippy 全开、deny 关键 lint |

## 3. 架构设计

### 3.1 工程组织
```
crate/
├── src/
│   ├── lib.rs
│   ├── bsp/        # 板级支持
│   ├── drivers/    # 驱动
│   └── app/        # 业务
├── Cargo.toml
├── build.rs
└── .cargo/config.toml
```

### 3.2 规范要点
| 项 | 规范 |
|----|------|
| lint | `#![warn(clippy::all)]` + deny unsafe(业务层) |
| unsafe | 集中底层，SAFETY 注释 |
| panic | 统一 panic_handler 策略 |
| 错误 | Result 显式传播 |
| 依赖 | 最小化、版本锁定、审计 |

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| crate 级 lint（deny） | 质量门禁 |
| `bsp/drivers/app` 分层 | 目录组织 |
| 依赖审计约定 | 供应链安全 |

## 5. 关键流程与时序
编码 → cargo fmt/clippy → 测试 → 依赖审计 → 合入（CI 门禁）。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 工具链 | stable | nightly | 稳定性、可复现 |
| lint | deny 关键规则 | 仅 warn | 质量门禁 |
| unsafe | 业务层禁用 | 允许 | 安全 |

## 7. 性能指标（KPI）
- clippy 告警清零。
- 测试覆盖率达标。
- 依赖漏洞 audit 清零。

## 8. 可靠性设计
- 规范聚焦内存安全与确定性。

## 9. 安全设计
- 依赖供应链审计 + 最小依赖面。

## 10. 测试与验证策略
- CI 集成 fmt/clippy/test/audit。

## 11. 集成与部署
- 规范文档 + 工具配置纳入仓库。

## 12. 参考资料
- Rust API Guidelines。
- Embedded Rust 生态最佳实践。

## 关联模块
- `foundation/rust-nostd`：no_std 工程。
- `foundation/rust-unsafe`：unsafe 规范。
- `foundation/cpp-standard`：C++ 规范对应。
