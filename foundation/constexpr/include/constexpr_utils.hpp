// constexpr —— constexpr 与编译期计算
// 对应文档: ../02-cpp/cpp-01-constexpr与编译期计算.md
// 职责: 编译期查表、CRC、位布局等纯函数工具，零运行时开销。
#pragma once

#include <cstdint>
#include <cstddef>

namespace cppu {

// ---- 编译期 CRC 查表 ----
constexpr std::uint32_t crc32_table_entry(std::size_t i);

// ---- 编译期位布局（寄存器字段） ----
struct reg_field {
    std::uint32_t offset;
    std::uint32_t width;
    constexpr std::uint32_t mask() const {
        return ((1u << width) - 1u) << offset;
    }
    constexpr std::uint32_t get(std::uint32_t reg) const {
        return (reg >> offset) & ((1u << width) - 1u);
    }
    constexpr std::uint32_t set(std::uint32_t reg, std::uint32_t val) const {
        return (reg & ~mask()) | ((val << offset) & mask());
    }
};

// ---- 编译期纯函数 ----
constexpr std::uint64_t factorial(std::uint64_t n);
constexpr std::uint32_t lut_generate(std::size_t idx);

// ---- if constexpr 分支裁剪 ----
template <typename T>
constexpr bool is_pow2(T v) { return v && !(v & (v - 1)); }

} // namespace cppu
