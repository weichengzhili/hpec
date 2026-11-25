/*
 * gpio-pinctrl —— RK3588 GPIO 与引脚复用接口
 * 对应文档: ../04-rk3588/rk3588-04-GPIO与引脚复用.md
 * 职责: GPIO 控制与 Pinctrl 复用接口声明。
 */
#ifndef RK3588_GPIO_H
#define RK3588_GPIO_H

#include <stdint.h>
#include <stdbool.h>

/* ---- 引脚功能复用 ---- */
typedef enum {
    PIN_FUNC_GPIO = 0,
    PIN_FUNC_UART,
    PIN_FUNC_I2C,
    PIN_FUNC_SPI,
    PIN_FUNC_PWM,
} pin_func_t;

/* ---- GPIO 方向与电平 ---- */
typedef enum {
    GPIO_IN = 0,
    GPIO_OUT = 1,
} gpio_dir_t;

/* ---- 中断触发类型 ---- */
typedef enum {
    GPIO_IRQ_NONE = 0,
    GPIO_IRQ_RISING,
    GPIO_IRQ_FALLING,
    GPIO_IRQ_BOTH,
    GPIO_IRQ_LEVEL_HIGH,
    GPIO_IRQ_LEVEL_LOW,
} gpio_irq_trigger_t;

/* ---- Pinctrl ---- */
int pinctrl_set_func(uint32_t pin, pin_func_t func);
int pinctrl_set_state(const char *state);   /* default/sleep */

/* ---- GPIO 控制 ---- */
int gpio_request(uint32_t pin);
int gpio_release(uint32_t pin);
int gpio_set_direction(uint32_t pin, gpio_dir_t dir);
int gpio_set_value(uint32_t pin, bool value);
int gpio_get_value(uint32_t pin, bool *value);

/* ---- 上拉/下拉/驱动强度 ---- */
int gpio_set_pull(uint32_t pin, uint32_t pull);   /* 0=none 1=up 2=down */
int gpio_set_drive_strength(uint32_t pin, uint32_t strength);

/* ---- GPIO 中断 ---- */
typedef void (*gpio_irq_handler_t)(void *arg);
int gpio_request_irq(uint32_t pin, gpio_irq_trigger_t trig, gpio_irq_handler_t h, void *arg);

#endif /* RK3588_GPIO_H */
