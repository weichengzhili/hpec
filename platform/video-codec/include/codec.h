/*
 * video-codec —— RK3588 视频编解码接口
 * 对应文档: ../04-rk3588/rk3588-10-视频编解码.md
 * 职责: 硬件 VPU 编解码（V4L2 mem2mem 抽象）。
 */
#ifndef RK3588_CODEC_H
#define RK3588_CODEC_H

#include <stdint.h>
#include <stddef.h>

/* ---- 编解码格式 ---- */
typedef enum {
    CODEC_H264 = 0,
    CODEC_H265,
    CODEC_VP9,
    CODEC_AV1,
    CODEC_MJPEG,
} codec_format_t;

/* ---- 编码参数 ---- */
typedef struct {
    uint32_t width;
    uint32_t height;
    codec_format_t format;
    uint32_t bitrate_kbps;
    uint32_t fps;
    uint32_t gop;
    uint32_t profile;
    uint32_t level;
} codec_enc_param_t;

/* ---- 解码参数 ---- */
typedef struct {
    codec_format_t format;
    uint32_t width;
    uint32_t height;
} codec_dec_param_t;

/* ---- 编解码会话 ---- */
typedef struct codec_session codec_session_t;

codec_session_t *codec_encoder_open(const codec_enc_param_t *param);
codec_session_t *codec_decoder_open(const codec_dec_param_t *param);

/* ---- 编码：输入帧 -> 输出码流 ---- */
int codec_encode(codec_session_t *s, const void *frame, int frame_fd, void *bitstream, size_t *bitstream_len);

/* ---- 解码：输入码流 -> 输出帧 ---- */
int codec_decode(codec_session_t *s, const void *bitstream, size_t len, void *frame, int *frame_fd);

int codec_close(codec_session_t *s);

#endif /* RK3588_CODEC_H */
