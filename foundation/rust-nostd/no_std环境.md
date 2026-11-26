# no_std 环境

> 模块目录：`workspace/foundation/rust-nostd/` · 接口文件：`src/lib.rs`

## 0. 问题定位

- **解决什么问题**：Rust 在裸机/受限环境下如何运行。
- **边界标准**：
  1. `#![no_std]` 编译通过，无 std 依赖，panic 处理定义清楚。
- **系统位置**：环 1、环 6。

## 1. 模块定位与目标
在无操作系统、无标准库的裸机环境下运行 Rust：去掉 `std`，配置 `core`/`alloc`、panic 处理、全局分配器、运行时初始化，建立最小可运行 Rust 环境。

**服务场景**：MCU 裸机、FPGA 软核固件、域控协处理器、引导阶段。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 环境 | 无 OS、无堆（可选 alloc） |
| 依赖 | 仅 `core`（必选）、`alloc`（可选） |
| 约束 | 无标准 IO、无线程、无文件系统 |
| 启动 | 需自建 `_start` 与运行时 |

## 3. 架构设计

### 3.1 crate 层级
| crate | 内容 | 依赖 |
|-------|------|------|
| core | 语言核心（Option/Result/迭代器） | 无 |
| alloc | 堆分配（Vec/String）需分配器 | core |
| std | 完整标准库 | 本模块不使用 |

### 3.2 运行时组件
- `#[panic_handler]`：panic 处理（记录/复位）。
- 全局分配器 `#[global_allocator]`（若用 alloc）。
- 启动入口：设置栈、初始化 `.bss`、跳转 main。
- 链接脚本：段布局、向量表。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `#![no_std]` | 无标准库声明 |
| `panic_handler(info)` | panic 处理（复位到安全态） |
| `GlobalAlloc` trait | 全局分配器约定 |
| `_start()` | 启动入口 |
| `__bss_start__/__bss_end__` 等 | 链接脚本导出符号 |
| `zero_bss()` | 清零 BSS |

## 5. 关键流程与时序
复位 → 启动入口 → 初始化栈与 BSS → 初始化分配器 → 调用 `main()` → panic 时进入 panic_handler。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| std | 禁用 | 使用 | 无 OS 环境 |
| alloc | 按需启用 | 全部 | 控制堆引入 |
| panic | 自定义 handler | 默认 | 复位/记录/降级 |
| 依赖 | 优先 core | std | 最小化 |

## 7. 性能指标（KPI）
- 二进制体积可控（no_std 无 std 开销）。
- 启动到 main 时间可量化。

## 8. 可靠性设计
- panic 处理保证系统复位到已知状态。
- 分配器确定性（见 memory/allocator）。

## 9. 安全设计
- 无 unsafe 堆操作、分配器隔离。

## 10. 测试与验证策略
- 目标平台单元测试（cargo test + 仿真/硬件）。
- 启动与 panic 路径测试。

## 11. 集成与部署
- 依赖锁（Cargo.lock）、工具链版本固化（见 rust-cross-build）。

## 12. 参考资料
- Embedded Rust Book（no_std 章节）。
- core/alloc crate 文档。

## 关联模块
- `boot/startup`：C 启动流程对应。
- `foundation/rust-guidelines`：no_std 工程规范。
- `memory/ownership`：分配器与所有权。
