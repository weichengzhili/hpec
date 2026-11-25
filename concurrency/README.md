# concurrency 并发与实时

> 跨 C/C++/Rust 的并发、临界区、状态机、实时框架相关功能模块。

| 模块 | 功能 | 接口文件 |
|------|------|----------|
| [中断安全与临界区](interrupt-critical/中断安全与临界区.md) | 临界区、原子操作、SPSC 队列 | include/interrupt_critical.h |
| [状态机与任务设计](fsm/状态机与任务设计.md) | 表驱动状态机、事件队列 | include/fsm.h |
| [RTOS集成](rtos/RTOS集成.md) | RTOS 原语 RAII 封装 | include/rtos.hpp |
| [状态机与事件驱动](fsm-event/状态机与事件驱动.md) | 强类型状态机框架 | include/fsm.hpp |
| [异步嵌入式与执行器](async/异步嵌入式与执行器.md) | Rust async/await、执行器 | src/lib.rs |
| [RTIC实时框架](rtic/RTIC实时框架.md) | 硬实时中断驱动并发 | src/lib.rs |
