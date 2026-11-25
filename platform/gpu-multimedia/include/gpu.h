/*
 * gpu-multimedia —— RK3588 GPU 与多媒体接口
 * 对应文档: ../04-rk3588/rk3588-08-GPU与多媒体.md
 * 职责: Mali GPU 渲染与多路显示接口声明。
 */
#ifndef RK3588_GPU_H
#define RK3588_GPU_H

#include <stdint.h>
#include <stddef.h>

/* ---- 图形 API 类型 ---- */
typedef enum {
    GPU_API_OPENGL_ES = 0,
    GPU_API_VULKAN,
    GPU_API_OPENCL,
} gpu_api_t;

/* ---- 显示接口类型 ---- */
typedef enum {
    DISP_HDMI = 0,
    DISP_DP,
    DISP_MIPI_DSI,
    DISP_EDP,
} disp_iface_t;

/* ---- 显示模式 ---- */
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t refresh_hz;
} disp_mode_t;

/* ---- GPU 初始化与上下文 ---- */
int gpu_init(gpu_api_t api);
void *gpu_create_context(void);
int gpu_destroy_context(void *ctx);

/* ---- 帧缓冲共享（dmabuf 零拷贝） ---- */
int gpu_export_dmabuf(void *surface, int *fd, uint32_t *stride, uint32_t *size);

/* ---- 显示控制 ---- */
int disp_open(disp_iface_t iface);
int disp_set_mode(disp_iface_t iface, const disp_mode_t *mode);
int disp_present(disp_iface_t iface, int dmabuf_fd);
int disp_hotplug_detect(disp_iface_t iface, bool *connected);

#endif /* RK3588_GPU_H */
