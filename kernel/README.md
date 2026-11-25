# kernel Linux 内核

> Linux 内核子系统功能模块（内核风格接口声明）。

| 模块 | 功能 | 接口文件 |
|------|------|----------|
| [内核架构总览](kernel-arch/内核架构总览.md) | 子系统、模块、系统调用 | include/kernel.h |
| [进程调度](scheduler/进程调度.md) | CFS、实时调度、亲和性 | include/sched.h |
| [内存管理](mm/内存管理.md) | 页、slab、映射、回收 | include/mm.h |
| [中断与下半部](irq/中断与下半部.md) | 上半部/下半部、softirq | include/irq.h |
| [内核锁与同步](lock/内核锁与同步.md) | 自旋锁、互斥、RCU、原子 | include/lock.h |
| [设备模型与sysfs](device-model/设备模型与sysfs.md) | 总线/设备/驱动、sysfs | include/device_model.h |
| [字符设备驱动](char-device/字符设备驱动.md) | cdev、file_operations | include/cdev.h |
| [平台驱动与设备树](platform-driver/平台驱动与设备树.md) | compatible、资源获取 | include/platform.h |
| [块设备驱动](block-device/块设备驱动.md) | blk-mq、请求队列 | include/block.h |
| [网络设备驱动](net-device/网络设备驱动.md) | netdev、NAPI | include/netdev.h |
| [工作队列与内核线程](workqueue/工作队列与内核线程.md) | workqueue、kthread | include/workqueue.h |
| [DMA与缓存一致性](dma/DMA与缓存一致性.md) | DMA API、IOMMU | include/dma.h |
| [内核调试](debug/内核调试.md) | printk、ftrace、kprobe | include/debug.h |
| [实时内核与PREEMPT_RT](realtime/实时内核与PREEMPT_RT.md) | 完全抢占、中断线程化 | include/realtime.h |
| [安全与加固](security/安全与加固.md) | LSM、seccomp、内存保护 | include/security.h |
