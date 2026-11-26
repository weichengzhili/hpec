# GPU 与多媒体

> 模块目录：`workspace/platform/gpu-multimedia/` · 接口文件：`include/gpu.h`

## 0. 问题定位

- **解决什么问题**：Mali GPU 算力与多路显示如何被正确调度和同步，避免渲染撕裂和资源冲突。
- **边界标准**：
  1. GPU 任务调度与 CPU 协调，无渲染撕裂（硬约束）。
  2. 多路显示帧同步正确（硬约束）。
  3. GPU 挂起/超时可检测和恢复（硬约束）。
- **系统位置**：环 7；`clock-power` → 本模块 → `ai/performance`。

## 1. 模块定位与目标
建立 RK3588 GPU（Mali G610）与显示/多媒体子系统方案：图形渲染、多路显示输出、UI 加速，支撑舱内显示、工业 HMI、边缘可视化。

**服务场景**：域控仪表/中控、工业 HMI 大屏、IoT 边缘可视化界面。

## 2. 需求与约束
| 类别 | 要求 |
|------|------|
| GPU | Mali G610（Valhall），OpenGL ES/Vulkan/OpenCL |
| 显示 | 多路 HDMI/DP/MIPI-DSI/eDP |
| 约束 | GPU 与显示功耗管理（见 clock-power） |

## 3. 架构设计

### 3.1 图形栈
```
应用 → 图形框架(Qt/EGL/Vulkan) → GPU 驱动(Mali) → 显示引擎 → 输出接口
```

### 3.2 显示子系统
- 显示控制器支持多路输出、图层合成。
- 显示框架：DRM/KMS。

## 4. 接口设计

| 接口 | 说明 |
|------|------|
| `gpu_api_t` | 图形 API 类型 |
| `disp_iface_t` | 显示接口类型 |
| `disp_mode_t` | 显示模式 |
| `gpu_init/create_context/destroy_context()` | GPU 上下文 |
| `gpu_export_dmabuf()` | 帧缓冲共享（dmabuf） |
| `disp_open/set_mode/present/hotplug_detect()` | 显示控制 |

## 5. 关键流程与时序
应用渲染 → GPU 生成帧缓冲 → 显示控制器合成 → 扫描输出。多图层合成由硬件完成。

## 6. 设计决策与权衡
| 决策点 | 选择 | 备选 | 理由 |
|--------|------|------|------|
| 图形 | Vulkan 高性能 / GL ES 兼容 | 单一 | 按场景 |
| 显示 | DRM/KMS | 私有 | 标准、多输出 |
| 共享 | dmabuf 零拷贝 | 拷贝 | 降低内存带宽 |

## 7. 性能指标（KPI）
- 渲染帧率、UI 流畅度。
- 多路输出分辨率/刷新率。

## 8. 可靠性设计
- 显示链路热插拔处理。
- GPU 异常恢复。

## 9. 安全设计
- 显示内容保护（HDCP 可选）。

## 10. 测试与验证策略
- 图形基准、多路显示测试。
- 长时间运行稳定性。

## 11. 集成与部署
- 图形栈版本与驱动匹配管理。

## 12. 参考资料
- Mali G610 驱动文档。
- Linux DRM/KMS 文档。

## 关联模块
- `system/automotive`：舱内显示应用。
- `platform/video-codec`：视频显示协同。
- `kernel/net-device`：无（仅参考）。
