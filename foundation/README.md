# foundation 语言基础与规范

> 跨 C/C++/Rust 的语言基础设施、编码规范、错误处理、调试与可移植性模块。

| 模块 | 功能 | 接口文件 |
|------|------|----------|
| [constexpr与编译期计算](constexpr/constexpr与编译期计算.md) | 编译期查表、CRC、位布局 | include/constexpr_utils.hpp |
| [模板元编程](templates/模板元编程.md) | 类型萃取、强类型单位、CRTP | include/type_traits.hpp |
| [RAII与资源管理](raii/RAII与资源管理.md) | 资源自动释放、锁守卫、scope_exit | include/raii.hpp |
| [无异常与错误处理](result/无异常与错误处理.md) | Result 类型、错误传播 | include/result.hpp |
| [移动语义与性能](move-semantics/移动语义与性能.md) | 零拷贝缓冲、移动语义 | include/move_buffer.hpp |
| [嵌入式C++编码规范](cpp-standard/嵌入式C++编码规范.md) | 安全子集、固定宽度类型 | include/types.hpp |
| [MISRA-C与功能安全编码](misra/MISRA-C与功能安全编码.md) | 安全类型、防御宏、返回码 | include/misra_safety.h |
| [跨平台可移植性](portability/跨平台可移植性.md) | 字节序、HAL、类型可移植 | include/portability.h |
| [调试与工具链](debug-log/调试与工具链.md) | 日志分级、断言、崩溃转储 | include/debug_log.h |
| [no_std环境](rust-nostd/no_std环境.md) | 无标准库运行时、panic 处理 | src/lib.rs |
| [unsafe边界](rust-unsafe/unsafe边界.md) | unsafe 收口、安全 API 封装 | src/lib.rs |
| [错误处理](rust-error/错误处理.md) | Rust Result、错误枚举 | src/lib.rs |
| [C-FFI互操作](rust-ffi/C-FFI互操作.md) | Rust/C ABI、内存所有权 | src/lib.rs |
| [交叉编译与构建](rust-cross-build/交叉编译与构建.md) | 目标三元组、链接脚本、build.rs | build.rs |
| [嵌入式Rust规范](rust-guidelines/嵌入式Rust规范.md) | clippy、目录组织、依赖审计 | src/lib.rs |
