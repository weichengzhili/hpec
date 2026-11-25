# MIPI CSI 相机

> 模块目录：`workspace/platform/mipi-csi/` · 接口文件：`include/csi.h`

## 1. 模块定位与目标
建立 RK3588 MIPI CSI 图像采集方案：传感器接入、CSI 接口、ISP 处理、V4L2 采集流水线，为视觉/AI 应用提供高质量图像输入。

**服务场景**：机器人视觉、舱内摄像头、AI 边缘视觉、工业检测相机。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| 接口 | MIPI CSI-2，支持多路 |
| 传感器 | I2C 配置 + CSI 数据 |
| ISP | 去马赛克、白平衡、曝光等 |
| 约束 | 带宽与内存占用（多路高清） |

## 3. 架构设计
```
传感器 → MIPI CSI-2 → ISP → 内存(DMA) → V4L2 应用 → AI/编码
```

### 3.1 采集流水线
| 阶段 | 说明 |
|------|------|
| 传感器 | 曝光/增益/分辨率配置（I2C） |
| CSI 接收 | 数据接收、多 lane |
| ISP | 图像处理（3A 等） |
| V4L2 | 标准采集接口 |

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `csi_pixel_format_t` | 像素格式 |
| `csi_config_t` | 采集配置 |
| `csi_frame_t` | 帧缓冲（dmabuf） |
| `csi_open/config/start_stream/stop_stream()` | 流水线控制 |
| `csi_get_frame/put_frame()` | 取帧/还帧 |
| `csi_sensor_set_exposure/set_gain()` | 传感器控制 |

## 5. 关键流程与时序
传感器初始化 → 配置格式/分辨率 → 启动数据流 → DMA 搬运到缓冲 → V4L2 出帧 → 应用处理。零拷贝 dmabuf 供 NPU/编码器复用。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 接口 | V4L2 + media controller | 私有 | 标准、通用 |
| 缓冲 | dmabuf 零拷贝 | 拷贝 | 供 AI/编码复用 |
| ISP | 硬件 ISP | 软件 | 高质量低 CPU |

## 7. 性能指标（KPI）
- 采集帧率/分辨率、延迟。
- 图像质量（ISP 调校）。

## 8. 可靠性设计
- 传感器热插拔/掉线检测。
- 帧丢失/异常恢复。

## 9. 安全设计
- 摄像头数据访问权限控制（隐私）。

## 10. 测试与验证策略
- 多传感器并行采集测试。
- 长时间采集稳定性。

## 11. 集成与部署
- 传感器驱动与 ISP 调校参数版本化。

## 12. 参考资料
- RK3588 MIPI/ISP 章节（TRM）。
- Linux V4L2 / media controller 文档。

## 关联模块
- `ai/vision-pipeline`：视觉流水线应用。
- `platform/video-codec`：编码协同。
- `driver/hal`：传感器 I2C 控制。
