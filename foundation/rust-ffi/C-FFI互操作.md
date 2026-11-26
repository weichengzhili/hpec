# C FFI 互操作

> 模块目录：`workspace/foundation/rust-ffi/` · 接口文件：`src/lib.rs`

## 0. 问题定位

- **解决什么问题**：Rust/C 之间 ABI 与内存所有权如何正确。
- **边界标准**：
  1. ABI 类型匹配，所有权明确，unsafe 最小化。
- **系统位置**：环 2、环 3。

## 1. 模块定位与目标
实现 Rust 与 C/C++ 的安全互操作：调用既有 C 库、暴露 Rust 接口给 C、ABI 匹配、内存/所有权边界管理。支持混合项目渐进式引入 Rust。

**服务场景**：复用既有 C 驱动/协议栈、向 C/C++ 项目暴露 Rust 安全模块、内核/应用混合。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| ABI | 使用 C ABI（`extern "C"`），跨语言布局一致 |
| 所有权 | 明确跨界内存归属，防双重释放 |
| 约束 | 不可跨 FFI 传 Rust 特有类型（String 等需转换） |

## 3. 架构设计

### 3.1 互操作方向
| 方向 | 机制 |
|------|------|
| Rust → C | `extern "C"` 声明 + unsafe 调用 |
| C → Rust | `#[no_mangle] pub extern "C" fn` |
| 类型映射 | C 类型 ↔ Rust 类型（c_int/u32 等） |
| 内存 | 指针传递 + 明确所有权约定 |

### 3.2 工具
- `bindgen`：从 C 头文件生成 Rust 声明。
- `cbindgen`：从 Rust 生成 C 头文件。
- 手动声明：控制精确布局。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `c_char/c_int/c_uint` 等 | C 类型映射 |
| `hal_uart_send/recv/hal_time_ms` | 外部 C 函数声明 |
| `ffi_add(a,b)` | 导出给 C 的函数 |
| `into_raw<T>()/from_raw<T>()` | 跨界内存所有权转移 |

## 5. 关键流程与时序
Rust 调 C：unsafe 调用 → 参数/返回值类型转换 → 处理 C 错误码。C 调 Rust：`extern "C"` 导出 → C 头文件声明 → 调用。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 声明 | bindgen 自动生成 | 手写 | 减少手写错误 |
| 所有权 | 明确约定 | 模糊 | 防双重释放 |
| 字符串 | 显式 CStr/CString | 强转 | 处理 null 终止 |

## 7. 性能指标（KPI）
- FFI 调用开销与 C 间调用等价（零额外）。
- 无跨 FFI 内存错误（unsafe 审计）。

## 8. 可靠性设计
- FFI 边界做参数校验（null、长度）。
- 跨界对象生命周期明确（RAII + into_raw）。

## 9. 安全设计
- unsafe 边界集中（见 rust-unsafe）。
- 校验 C 输入防越界/注入。

## 10. 测试与验证策略
- FFI 接口双向单测。
- Valgrind/ASan 检测跨界内存错误。

## 11. 集成与部署
- 生成的绑定纳入构建（build.rs）与版本管理。

## 12. 参考资料
- Rust FFI 指南。
- bindgen/cbindgen 文档。

## 关联模块
- `foundation/rust-unsafe`：FFI 的 unsafe 边界。
- `foundation/portability`：C 侧 HAL 接口。
- `foundation/rust-cross-build`：bindgen 集成到 build.rs。
