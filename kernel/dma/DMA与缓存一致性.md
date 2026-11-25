# DMA 与缓存一致性

> 模块目录：`workspace/kernel/dma/` · 接口文件：`include/dma.h`

## 1. 模块定位与目标
建立内核 DMA 与缓存一致性管理方案：DMA API、一致性/流式映射、IOMMU/SMMU 保护，保证设备与 CPU 之间数据一致性，支撑高吞吐外设。

**服务场景**：RK3588 流媒体/AI 数据搬移、PCIe 设备 DMA、存储 DMA（与 platform/dma-engine 对应）。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 一致性 | DMA 与 CPU 缓存数据一致 |
| 映射 | 一致性/流式映射 |
| 约束 | 缓冲对齐、方向正确 |

## 3. 架构设计

### 3.1 DMA API
| 类型 | 说明 |
|------|------|
| 一致性映射 (coherent) | 自动一致性 |
| 流式映射 (streaming) | 手动 cache 同步 |
| scatter-gather | 非连续缓冲 |

### 3.2 IOMMU/SMMU
- 地址转换与设备访问隔离。
- 防设备越界访问内存。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `dma_dir_t` | 方向 |
| `dma_alloc_coherent/free_coherent()` | 一致性映射 |
| `dma_map_single/unmap_single()` | 流式映射 |
| `dma_sync_for_cpu/device()` | cache 同步 |
| `dma_sg_t` + `dma_map_sg/unmap_sg()` | scatter-gather |
| `iommu_attach/detach/map/unmap()` | IOMMU |

## 5. 关键流程与时序
分配缓冲 → 映射（cache 清理/无效化）→ 设备传输 → 同步 → CPU 访问。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 映射 | 流式为主 | 一致性 | 灵活、开销可控 |
| 保护 | IOMMU | 无 | 隔离与安全 |
| 缓冲 | CMA 连续 | 离散 | 设备要求 |

## 7. 性能指标（KPI）
- DMA 吞吐、CPU 占用。
- 缓存一致性问题 = 0。

## 8. 可靠性设计
- 缓冲生命周期管理。
- DMA 错误检测。

## 9. 安全设计
- IOMMU 隔离设备 DMA 访问。

## 10. 测试与验证策略
- DMA 压力测试、一致性专项测试。

## 11. 集成与部署
- DMA 缓冲池与 IOMMU 配置。

## 12. 参考资料
- Linux DMA-API 文档。
- IOMMU 文档。

## 关联模块
- `platform/dma-engine`：RK3588 DMA 控制器。
- `memory/memory-barrier`：缓存操作底层。
- `kernel/mm`：CMA 连续内存。
