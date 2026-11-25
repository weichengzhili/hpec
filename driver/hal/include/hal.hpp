// hal —— 硬件抽象层设计
// 对应文档: ../02-cpp/cpp-05-硬件抽象层设计.md
// 职责: 硬件访问抽象接口（依赖注入、可 Mock 测试、热路径零虚调用）。
#pragma once

#include <cstdint>
#include <cstddef>

namespace hal {

// ---- GPIO 抽象接口 ----
struct igpio {
    virtual ~igpio() = default;
    virtual void set_level(std::uint32_t pin, bool high) = 0;
    virtual bool get_level(std::uint32_t pin) = 0;
    virtual void set_direction(std::uint32_t pin, bool output) = 0;
};

// ---- UART 抽象接口 ----
struct iuart {
    virtual ~iuart() = default;
    virtual std::size_t write(const std::uint8_t *buf, std::size_t len) = 0;
    virtual std::size_t read(std::uint8_t *buf, std::size_t len) = 0;
    virtual void flush() = 0;
};

// ---- SPI 抽象接口 ----
struct ispi {
    virtual ~ispi() = default;
    virtual void transfer(const std::uint8_t *tx, std::uint8_t *rx, std::size_t len) = 0;
};

// ---- I2C 抽象接口 ----
struct ii2c {
    virtual ~ii2c() = default;
    virtual bool write(std::uint8_t addr, const std::uint8_t *buf, std::size_t len) = 0;
    virtual bool read(std::uint8_t addr, std::uint8_t *buf, std::size_t len) = 0;
};

// ---- ADC 抽象接口 ----
struct iadc {
    virtual ~iadc() = default;
    virtual std::uint32_t read_channel(std::uint32_t ch) = 0;
};

// ---- 硬件抽象聚合（依赖注入入口） ----
struct hal_iface {
    igpio *gpio = nullptr;
    iuart *uart = nullptr;
    ispi  *spi  = nullptr;
    ii2c  *i2c  = nullptr;
    iadc  *adc  = nullptr;
};

// ---- 热路径零开销抽象：CRTP 静态多态（无虚函数） ----
template <typename Derived>
struct static_gpio {
    void set_level(std::uint32_t pin, bool high) {
        static_cast<Derived *>(this)->set_level_impl(pin, high);
    }
    bool get_level(std::uint32_t pin) {
        return static_cast<Derived *>(this)->get_level_impl(pin);
    }
};

} // namespace hal
