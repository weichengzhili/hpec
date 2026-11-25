/*
 * net-device —— 网络设备驱动接口
 * 对应文档: ../06-kernel/kernel-10-网络设备驱动.md
 * 职责: net_device 注册、NAPI 收包、发包接口。
 */
#ifndef KERNEL_NETDEV_H
#define KERNEL_NETDEV_H

#include <stdint.h>
#include <stddef.h>

/* ---- 网络设备 ---- */
typedef struct net_device {
    const char *name;
    uint8_t  mac[6];
    uint32_t mtu;
    uint32_t features;       /* 硬件卸载能力位 */
    void    *priv;
} net_device_t;

/* ---- 网络设备操作 ---- */
typedef struct {
    int (*open)(net_device_t *dev);
    int (*stop)(net_device_t *dev);
    int (*xmit)(net_device_t *dev, const void *skb, size_t len);  /* 发包 */
    int (*set_mac)(net_device_t *dev, const uint8_t *mac);
} netdev_ops_t;

/* ---- 注册 ---- */
int netdev_register(net_device_t *dev, const netdev_ops_t *ops);
void netdev_unregister(net_device_t *dev);

/* ---- NAPI 收包（轮询批量收包） ---- */
typedef int (*napi_poll_fn_t)(net_device_t *dev, int budget);
void napi_enable(net_device_t *dev, napi_poll_fn_t poll);
void napi_disable(net_device_t *dev);
void napi_schedule(net_device_t *dev);   /* 中断触发调度 */
void napi_complete(net_device_t *dev);

/* ---- 收包上交协议栈 ---- */
void netif_receive(net_device_t *dev, const void *data, size_t len);

/* ---- 统计 ---- */
uint64_t netdev_rx_packets(const net_device_t *dev);
uint64_t netdev_tx_packets(const net_device_t *dev);

#endif /* KERNEL_NETDEV_H */
