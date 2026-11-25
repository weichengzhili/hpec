/*
 * portability —— 跨平台可移植性接口
 * 对应文档: ../01-c/c-08-跨平台可移植性.md
 * 职责: 固定宽度类型、字节序转换、平台抽象层（HAL）统一接口。
 */
#ifndef PORTABILITY_H
#define PORTABILITY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---- 字节序转换 ---- */
uint16_t port_htons(uint16_t v);
uint16_t port_ntohs(uint16_t v);
uint32_t port_htonl(uint32_t v);
uint32_t port_ntohl(uint32_t v);

/* ---- 平台抽象层（HAL）统一接口 ---- */
uint32_t hal_time_ms(void);
uint64_t hal_time_us(void);
void     hal_delay_ms(uint32_t ms);
void     hal_delay_us(uint32_t us);

void hal_gpio_set(uint32_t pin, bool level);
bool hal_gpio_get(uint32_t pin);
void hal_gpio_config(uint32_t pin, uint32_t mode);

void hal_uart_send(uint32_t port, const uint8_t *data, size_t len);
size_t hal_uart_recv(uint32_t port, uint8_t *data, size_t len);

const char *hal_platform_name(void);
uint32_t    hal_platform_id(void);

#endif /* PORTABILITY_H */
