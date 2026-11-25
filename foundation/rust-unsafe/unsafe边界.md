# unsafe 边界

> 模块目录：`workspace/foundation/rust-unsafe/` · 接口文件：`src/lib.rs`

## 1. 模块定位与目标
规范 unsafe 代码的使用：将不可用安全 Rust 表达的底层操作（MMIO、FFI、裸指针、内联汇编）收口到最小、可审计的边界，用安全 API 封装，保证整体内存安全承诺。

**服务场景**：PAC 寄存器访问、FFI（rust-ffi）、中断向量、内存映射 DMA。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 最小化 | unsafe 代码占比最小、集中 |
| 封装 | unsafe 实现 + 安全 API 暴露 |
| 审计 | unsafe 块可追溯、有理由 |
| 约束 | 禁止在业务层散落 unsafe |

## 3. 架构设计

### 3.1 unsafe 使用场景
| 场景 | 说明 |
|------|------|
| MMIO 寄存器读写 | 裸指针访问外设地址 |
| FFI | 调用 C 函数（见 rust-ffi） |
| 裸指针解引用 | 内存映射、缓冲 |
| 内联汇编 | 特殊指令、屏障 |
| 全局可变静态 | 单例外设句柄 |
| unsafe trait | 不变量由实现者保证 |

### 3.2 封装模式
- unsafe 函数内部做完整安全检查与前置校验。
- 暴露安全接口，用类型系统（`&mut` 独占）保证不变量。
- 用 `// SAFETY:` 注释说明不变量依据。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `mmio::read/write(addr,val)` | 寄存器读写（unsafe 收口） |
| `RegBlock::new/read/write` | 带地址白名单校验的安全访问 |
| `asm::dsb/dmb/compiler_fence` | 内联汇编屏障封装 |

## 5. 关键流程与时序
安全调用 → 前置校验 → unsafe 底层操作 → 恢复不变量 → 返回。任何不变量破坏在 unsafe 边界内处理，不外泄。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| unsafe 位置 | 集中底层 | 散落 | 可审计 |
| 封装 | 安全 API | 裸暴露 | 不变量由类型保证 |
| 文档 | SAFETY 注释 | 无 | 可审查 |

## 7. 性能指标（KPI）
- unsafe 代码占比可量化（目标 < 5%）。
- unsafe 块全部有 SAFETY 注释。

## 8. 可靠性设计
- unsafe 块前置校验防不变量破坏。
- 用断言捕获违反。

## 9. 安全设计
- unsafe 最小化即降低内存安全风险面。

## 10. 测试与验证策略
- unsafe 边界专项测试 + Miri 内存检查。
- 代码评审重点审查 unsafe。

## 11. 集成与部署
- CI 用 `#![deny(unsafe_code)]` 在业务 crate 禁用 unsafe。

## 12. 参考资料
- Rustonomicon（unsafe 指南）。
- Rust 安全编码规范。

## 关联模块
- `driver/pac-hal`：PAC 的 unsafe 边界应用。
- `foundation/rust-ffi`：FFI 的 unsafe。
- `memory/ownership`：安全侧抽象。
