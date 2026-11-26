# DMA 引擎

> 模块目录：`workspace/platform/dma-engine/` · 接口文件：`include/dma_engine.h`

## 0. 问题定位

- **解决什么问题**：DMA 通道如何正确搬运数据并处理缓存一致性和错误。
- **边界标准**：
  1. 传输完成前 CPU 不访问该 buffer，所有权明确（硬约束）。
  2. cache clean（D→M）与 invalidate（M→D）时机正确（硬约束）。
  3. DMA 错误（超时/部分传输）有检测和恢复，不死等（硬约束）。
- **系统位置**：环 5；`memory-barrier`、`gic` → 本模块 → `mipi-csi`、`video-codec`。

## 1. 模块定位与目标
建立 RK3588 DMA 传输方案：DMA 控制器、传输通道、缓存一致性管理，实现外设与内存间的高效数据搬移，减轻 CPU 负担，保障流媒体/网关高吞吐。

**服务场景**：流媒体缓冲、网关报文收发、AI 预处理数据搬移、高速外设。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 高吞吐 | 多媒体/AI 需大带宽 DMA |
| 一致性 | 缓存与 DMA 数据一致性 |
| 约束 | DMA 缓冲区对齐、映射与生命周期 |

## 3. 架构设计

### 3.1 DMA 体系
| 组件 | 说明 |
|------|------|
| DMA 控制器 | 多通道传输 |
| 描述符链 | 分散/聚集（scatter-gather） |
| DMA 缓冲 | 专用 dma_alloc 分配 |

### 3.2 缓存一致性
- DMA 方向：TO_DEVICE / FROM_DEVICE / BIDIRECTIONAL。
- 同步：`dma_map/unmap` 负责 cache 清理/无效化。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `dma_dir_t` | 传输方向 |
| `dma_alloc_coherent/free_coherent()` | 一致性缓冲 |
| `dma_map_single/unmap_single()` | 流式映射 |
| `dma_sync_single_for_cpu/device()` | cache 同步 |
| `dma_sg_entry_t` | scatter-gather 描述符 |
| `dma_transfer()/wait_complete()` | 传输与等待 |

## 5. 关键流程与时序
申请缓冲 → 映射（cache 清理）→ 配置描述符 → 启动传输 → 完成中断 → 解映射（cache 无效化）→ 处理数据。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 缓冲 | 一致性/流式按需 | 单一 | 平衡开销 |
| 传输 | scatter-gather | 连续 | 高效非连续 |
| 完成 | 中断/轮询按吞吐 | 单一 | 延迟与吞吐平衡 |

## 7. 性能指标（KPI）
- DMA 吞吐、CPU 占用率降低量化。
- 传输完成延迟。

## 8. 可靠性设计
- 传输错误检测与重试。
- 缓冲区生命周期管理防 use-after-free。

## 9. 安全设计
- DMA 缓冲区隔离（IOMMU/SMMU），防越界访问。

## 10. 测试与验证策略
- 大块/分散传输压力测试。
- 缓存一致性专项测试。

## 11. 集成与部署
- DMA 缓冲池纳入内存预算。

## 12. 参考资料
- RK3588 DMA 章节（TRM）。
- Linux DMA-API / dmaengine 文档。

## 关联模块
- `kernel/dma`：内核 DMA API。
- `memory/memory-barrier`：缓存一致性。
- `ai/npu-runtime`：AI 数据搬移应用。
