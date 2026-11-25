# RTOS 集成

> 模块目录：`workspace/concurrency/rtos/` · 接口文件：`include/rtos.hpp`

## 1. 模块定位与目标
用 C++ 对 RTOS 原语（任务、队列、互斥量、信号量、定时器、事件组）做类型安全的 RAII 封装，消除句柄裸管理，提升并发代码的安全性与可读性。

**边界**：C++ 封装层；底层为 FreeRTOS/Zephyr/ThreadX/RT-Thread 等 RTOS。

**服务场景**：域控 RTOS 应用、机器人运动控制任务、网关协议任务。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| RTOS | FreeRTOS / Zephyr / ThreadX / RT-Thread |
| 约束 | 封装不增加运行时开销，内联后与原语等价 |
| 约束 | 无异常/无 RTTI（见 foundation/cpp-standard） |

## 3. 架构设计

### 3.1 封装对象
| RTOS 原语 | C++ 封装 | RAII/特性 |
|-----------|----------|-----------|
| 任务 | `task` | 栈、优先级、入口可配 |
| 队列 | `queue<T>` | 类型安全入队/出队 |
| 互斥量 | `mutex` + `guard` | 作用域自动解锁 |
| 信号量 | `semaphore` | 计数/二值 |
| 定时器 | `timer` | 周期/单次回调 |

### 3.2 任务封装
- 任务入口绑定成员函数，参数类型安全。
- 栈大小、优先级、核心亲和性在构造时确定（`task_config`）。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `task(task_config, Fn&&)` | 构造任务，绑定函数体 |
| `task::start/request_stop/join/is_running` | 生命周期 |
| `queue<T>::push/pop` | 类型安全队列（含 from_isr） |
| `mutex::guard` | RAII 守卫 |
| `semaphore::take/give` | 信号量 |
| `timer::start_periodic/oneshot` | 定时器 |

## 5. 关键流程与时序
- 任务入口为静态跳板（RTOS 要求 C 函数指针）→ 转换到 C++ 对象成员函数 → 业务循环。
- 队列传递类型化对象，避免 void* 类型擦除。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 队列 | 类型安全 `queue<T>` | void* | 消除类型错误 |
| 锁 | RAII 守卫 | 手动 | 防忘解锁/死锁 |
| 任务跳板 | 静态函数+对象指针 | 成员函数指针 | 满足 RTOS 接口 |
| 封装 | 全部内联 | 虚函数 | 零开销 |

## 7. 性能指标（KPI）
- 封装层反汇编与原语直接调用等价（零开销）。
- 无句柄泄漏、无锁遗漏。

## 8. 可靠性设计
- 队列满/空处理显式化。
- 任务栈溢出检测（见 memory/memory-model）。
- 死锁防护：锁顺序约定 + 超时。

## 9. 安全设计
- 任务间数据隔离，队列类型安全。
- 权限最小化：任务只访问所需资源。

## 10. 测试与验证策略
- 封装层单测（mock RTOS 或真实 RTOS）。
- 并发压力测试。

## 11. 集成与部署
- RTOS 封装作为中间层库，屏蔽 RTOS 差异，便于切换。

## 12. 参考资料
- 目标 RTOS API 文档。
- 嵌入式 C++ 并发封装模式。

## 关联模块
- `concurrency/interrupt-critical`：中断上下文约束。
- `concurrency/async`：协作式并发（Rust）。
- `foundation/raii`：RAII 封装基础。
