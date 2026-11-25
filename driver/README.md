# driver 设备驱动与外设

> 跨 C/C++/Rust 的外设访问与硬件抽象相关功能模块。

| 模块 | 功能 | 接口文件 |
|------|------|----------|
| [位操作与寄存器编程](bit-register/位操作与寄存器编程.md) | 位操作原语、寄存器字段读写 | include/bit_register.h |
| [硬件抽象层设计](hal/硬件抽象层设计.md) | 抽象接口、依赖注入、可测试 | include/hal.hpp |
| [PAC与HAL](pac-hal/PAC与HAL.md) | 外设访问层、embedded-hal | src/lib.rs |
