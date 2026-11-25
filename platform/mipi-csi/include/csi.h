/*
 * mipi-csi —— RK3588 MIPI CSI 相机接口
 * 对应文档: ../04-rk3588/rk3588-09-MIPI-CSI相机.md
 * 职责: MIPI CSI 采集流水线接口（V4L2 抽象）。
 */
#ifndef RK3588_CSI_H
#define RK3588_CSI_H

#include <stdint.h>
#include <stddef.h>

/* ---- 像素格式 ---- */
typedef enum {
    CSI_FMT_YUV422 = 0,
    CSI_FMT_NV12,
    CSI_FMT_RAW10,
    CSI_FMT_RAW12,
    CSI_FMT_RGB888,
} csi_pixel_format_t;

/* ---- 采集配置 ---- */
typedef struct {
    uint32_t width;
    uint32_t height;
    csi_pixel_format_t format;
    uint32_t fps;
    uint32_t lanes;          /* 数据 lane 数 */
} csi_config_t;

/* ---- 帧缓冲 ---- */
typedef struct {
    void    *data;
    size_t   size;
    int      dmabuf_fd;      /* 零拷贝供 NPU/编码器复用 */
    uint64_t timestamp_us;
} csi_frame_t;

/* ---- 采集流水线 ---- */
int csi_open(uint32_t csi_id);
int csi_config(uint32_t csi_id, const csi_config_t *cfg);
int csi_start_stream(uint32_t csi_id);
int csi_stop_stream(uint32_t csi_id);

/* ---- 取帧（阻塞/超时） ---- */
int csi_get_frame(uint32_t csi_id, csi_frame_t *frame, uint32_t timeout_ms);
int csi_put_frame(uint32_t csi_id, csi_frame_t *frame);

/* ---- 传感器控制（I2C 配置，见 driver/hal ii2c） ---- */
int csi_sensor_set_exposure(uint32_t csi_id, uint32_t exposure_us);
int csi_sensor_set_gain(uint32_t csi_id, uint32_t gain);

int csi_close(uint32_t csi_id);

#endif /* RK3588_CSI_H */
