# volatile 与内存屏障

> 模块目录：`workspace/memory/memory-barrier/` · 接口文件：`include/volatile_barrier.h`

## 1. 模块定位与目标
规范硬件寄存器访问与跨执行单元（核/中断/DMA）共享数据的一致性语义，消除编译器与 CPU 乱序带来的隐蔽缺陷。是驱动与实时内核正确性的关键。

**边界**：只负责"访问语义与顺序"，不负责锁（见 concurrency/interrupt-critical、kernel/lock）。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 语义 | 区分 volatile 与内存屏障两种完全不同的机制 |
| 多核 | RK3588 8 核 A76+A55，需跨核缓存一致性 |
| 约束 | 裸机与带 OS 两套访问模型 |
| 强制 | 硬件 FIFO/状态寄存器必须每次真实读写，禁止合并/缓存 |

## 3. 架构设计

### 3.1 volatile 语义边界
- 保证每次访问都产生真实内存操作、不优化删除/重排 volatile 访问本身。
- **不保证**：原子性、跨核可见性、与其它非 volatile 访问的相对顺序。
- 适用：MMIO 寄存器轮询、信号量标志位。

### 3.2 内存屏障分类
| 屏障 | 语义 | 适用 |
|------|------|------|
| 编译器屏障 `asm volatile("":::"memory")` | 阻止编译器跨屏障重排 | 所有 |
| DMB | 数据内存屏障 | 跨核数据可见性 |
| DSB | 数据同步屏障 | 外设、缓存、TLB 操作后 |
| ISB | 指令同步屏障 | 上下文切换、异常返回 |
| 读/写屏障 | 单侧排序 | 生产者-消费者 |

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `mmio_read8/16/32/64(addr)` | 寄存器读取（volatile 封装） |
| `mmio_write8/16/32/64(addr,v)` | 寄存器写入 |
| `mb_compiler()` | 编译器屏障 |
| `mb_read()/mb_write()/mb_full()` | 读/写/完整屏障 |
| `mb_isb()/mb_dsb()` | 指令/数据同步屏障 |
| `mb_sig_atomic_t` | 中断安全标志类型 |
| `cache_clean_range/invalidate_range/clean_invalidate_range()` | DMA 前后缓存一致性 |

## 5. 关键流程与时序
典型驱动模式：
```
1. WRITE 命令寄存器（mb_write 隔离）
2. 轮询状态寄存器（volatile READ 循环）
3. 数据就绪后 READ 数据寄存器
4. cache_clean_range() → DMA 传输 → cache_invalidate_range()
5. mb_full() 后让其它核/中断读取
```

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 寄存器访问 | 统一宏/函数封装 | 直接 `*(volatile u32*)` | 可审计、可注入仿真、避免遗漏 volatile |
| 跨核同步 | 内核原子/lock | volatile | volatile 不提供原子性 |
| 内存序 | acquire/release 语义 | 全屏障 | 更高效 |
| 中断标志 | volatile sig_atomic_t | 普通变量 | 唯一标准保证的类型 |

## 7. 性能指标（KPI）
- 驱动热路径屏障数量最小化。
- 轮询循环避免全屏障，用 acquire 语义。

## 8. 可靠性设计
- 生产者写屏障 + 消费者读屏障配对使用，防止乱序读旧数据。
- DMA 前后执行 cache 清理/无效化，保证一致性。

## 9. 安全设计
- MMIO 地址越界防护：统一接口做白名单映射表校验。
- 寄存器写保护位：关键寄存器写后回读校验。

## 10. 测试与验证策略
- 跨核共享结构压力测试（ThreadSanitizer/helgrind）。
- 反汇编检查关键路径无意外重排。

## 11. 集成与部署
- 内存序宏随内核/工具链版本固化，避免升级引入语义变化。

## 12. 参考资料
- C11 内存模型（volatile 与 _Atomic 区别）。
- ARM《ARM Architecture Reference Manual》内存序章节。
- Linux 内核 `Documentation/memory-barriers.txt`。

## 关联模块
- `kernel/lock`：锁的底层内存序实现。
- `kernel/dma`：DMA 缓存一致性。
- `driver/bit-register`：寄存器访问（基于本模块 mmio 封装）。
