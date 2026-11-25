# memory 内存管理

> 跨 C/C++/Rust 的内存相关功能模块。

| 模块 | 功能 | 接口文件 |
|------|------|----------|
| [内存模型与存储布局](memory-model/内存模型与存储布局.md) | 存储类、内存段、对齐、端序、ABI | include/memory_model.h |
| [volatile与内存屏障](memory-barrier/volatile与内存屏障.md) | MMIO 访问、内存屏障、缓存一致性 | include/volatile_barrier.h |
| [内存管理与内存池](memory-pool/内存管理与内存池.md) | 固定块内存池，O(1) 分配 | include/memory_pool.h |
| [内存池与定制分配器](allocator/内存池与定制分配器.md) | C++ Allocator、STL 容器接入 | include/memory_pool.hpp |
| [所有权借用与内存安全](ownership/所有权借用与内存安全.md) | Rust 所有权、借用、原子 | src/lib.rs |
