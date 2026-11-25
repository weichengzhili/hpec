/*
 * pcie-usb-storage —— RK3588 PCIe / USB / 存储接口
 * 对应文档: ../04-rk3588/rk3588-11-PCIe-USB-存储.md
 * 职责: PCIe/USB/存储设备接口声明。
 */
#ifndef RK3588_IO_H
#define RK3588_IO_H

#include <stdint.h>
#include <stddef.h>

/* ---- PCIe ---- */
typedef enum {
    PCIE_RC = 0,    /* Root Complex */
    PCIE_EP,        /* Endpoint */
} pcie_mode_t;

int pcie_init(uint32_t controller, pcie_mode_t mode);
int pcie_enum_devices(uint32_t controller);
int pcie_config_read(uint32_t controller, uint32_t dev, uint32_t reg, uint32_t *val);
int pcie_config_write(uint32_t controller, uint32_t dev, uint32_t reg, uint32_t val);

/* ---- USB ---- */
typedef enum {
    USB_MODE_HOST = 0,
    USB_MODE_DEVICE,
    USB_MODE_OTG,
} usb_mode_t;

int usb_init(uint32_t controller, usb_mode_t mode);
int usb_detect_device(uint32_t controller, bool *connected);

/* ---- 存储 ---- */
typedef enum {
    STORAGE_EMMC = 0,
    STORAGE_SD,
    STORAGE_NVME,
    STORAGE_SATA,
} storage_type_t;

typedef struct {
    storage_type_t type;
    uint64_t       capacity_bytes;
    uint32_t       block_size;
} storage_info_t;

int storage_get_info(storage_type_t type, storage_info_t *info);
int storage_read(storage_type_t type, uint64_t offset, void *buf, size_t len);
int storage_write(storage_type_t type, uint64_t offset, const void *buf, size_t len);

#endif /* RK3588_IO_H */
