# HPEC 嵌入式技术文档库

> 工业级嵌入式开发知识库，面向 **汽车域控、工业网关、高端 IoT 边缘节点、AI 边缘计算部署、工业机器人** 五大落地场景。
> 技术底座：**C / C++ / Rust** 三语言 + **RK3588 SoC、FPGA、Linux 内核（含 PREEMPT_RT）、边缘 AI** 四领域。
> 面向正式生产环境，非 Demo。按功能领域划分，每个功能点独立成模块，统一文档结构。

---

## 0. 系统问题链（总纲）

整个项目围绕一条**系统问题链**展开，而不是围绕概念分类展开。详见 [系统问题链](system/document-resource-management/系统问题链.md)；每个模块「解决什么问题、边界标准是什么、位于哪一环」，见 [模块问题定义](system/document-resource-management/模块问题定义.md)。

```text
物理真相 → 启动 → 可见性 → 控制 → 事件 → 数据 → 并发 → 资源 → 故障 → 证据 → 演进
```

每个领域、每个模块，都挂靠在这条链的某一环上。新增或修改任何模块前，先确认它在问题链上的位置和上下游依赖。

---

## 1. 目录组织

- **按功能领域划分**（不按语言/序号），共 **10 个领域、86 个功能模块**。
- 每个模块目录内包含两部分：
  - **设计文档**（`.md`）：模块定位、架构、接口、流程、权衡、KPI 等 12 章节。
  - **接口声明 / 资源清单**：C 用 `.h`、C++ 用 `.hpp`、Rust 用 `.rs`、FPGA 用 `.sv`；系统资源管理类模块使用 `.json` 清单作为机器可读接口。

```
HPEC/
├── memory/          # 内存管理（跨 C/C++/Rust）
├── concurrency/     # 并发、中断、实时（跨 C/C++/Rust）
├── driver/          # 设备驱动与外设（跨 C/C++/Rust）
├── boot/            # 启动引导
├── foundation/      # 语言基础与规范
├── platform/        # RK3588 平台
├── kernel/          # Linux 内核
├── fpga/            # FPGA 逻辑
├── ai/              # 边缘 AI
└── system/          # 场景集成与系统资源
```

## 2. 领域总览

| 领域 | 模块数 | 说明 |
|------|--------|------|
| [memory](memory/README.md) | 6 | 内存模型、屏障、内存池、分配器、所有权、资源调度 |
| [concurrency](concurrency/README.md) | 6 | 临界区、状态机、RTOS、异步、RTIC |
| [driver](driver/README.md) | 3 | 寄存器编程、硬件抽象、PAC/HAL |
| [boot](boot/README.md) | 1 | 链接脚本与启动流程 |
| [foundation](foundation/README.md) | 15 | 编译期计算、模板、RAII、错误处理、规范、调试、可移植 |
| [platform](platform/README.md) | 13 | SoC 架构、启动、时钟电源、NPU、多媒体、驱动 |
| [kernel](kernel/README.md) | 15 | 调度、内存、中断、锁、驱动、实时、安全 |
| [fpga](fpga/README.md) | 12 | RTL、时序、CDC、FIFO、AXI、DSP、验证 |
| [ai](ai/README.md) | 9 | 量化、NPU 运行时、框架、视觉、MLOps |
| [system](system/README.md) | 6 | 场景集成方案、文档资源管理 |

## 3. 统一模块文档结构

每个模块的 `.md` 设计文档包含以下章节：

1. 模块定位与目标
2. 需求与约束
3. 架构设计
4. 接口设计
5. 关键流程与时序
6. 设计决策与权衡
7. 性能指标（KPI）
8. 可靠性设计
9. 安全设计
10. 测试与验证策略
11. 集成与部署
12. 参考资料 + 关联模块

## 4. 五大场景与技术映射

| 场景 | 核心诉求 | 依赖领域 |
|------|----------|----------|
| 汽车域控 | 功能安全/信息安全、实时、OTA | kernel、platform、foundation、ai |
| 工业网关 | 多协议、边缘计算、TSN、可靠 | kernel、platform、fpga、concurrency |
| IoT 边缘节点 | 边缘推理、低功耗、无线 | platform、ai、foundation |
| AI 边缘部署 | 量化、NPU、视觉、MLOps | ai、platform、kernel |
| 工业机器人 | 运动控制、实时、EtherCAT、安全 | kernel、fpga、platform、ai |

## 5. 模块依赖关系

```
            ┌──────────────────────────────────────────┐
            │   system（系统级整合）                     │
            │ 汽车域控/工业网关/IoT边缘/AI部署/工业机器人 │
            └──────────────────┬───────────────────────┘
                               │
   ┌───────────┬───────────────┼───────────────┬───────────┐
   ▼           ▼               ▼               ▼           ▼
 platform      fpga          kernel           ai
   │           │               │               │
   └───────────┴───────────────┴───────────────┘
               │  语言/基础基座
   ┌───────────┴───────────────────────────────┐
   │ memory · concurrency · driver ·            │
   │ boot · foundation                          │
   └────────────────────────────────────────────┘
```

- **语言/基础层**（memory、concurrency、driver、boot、foundation）是基座。
- **平台/领域层**（platform、fpga、kernel、ai）构建于基座之上。
- **system** 将各领域模块做系统级整合。
