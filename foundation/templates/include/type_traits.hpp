// templates —— 模板元编程与类型萃取
// 对应文档: ../02-cpp/cpp-02-模板元编程.md
// 职责: 类型萃取、强类型单位、CRTP 静态多态基类。
#pragma once

#include <cstdint>
#include <type_traits>

namespace cppu {

// ---- 强类型单位系统（编译期杜绝量纲错误） ----
template <typename T, typename Tag>
struct strong_type {
    T value;
    constexpr explicit strong_type(T v) : value(v) {}
    constexpr T get() const { return value; }
};

struct length_mm_tag {};
struct freq_hz_tag {};
struct volt_tag {};
struct ampere_tag {};

using length_mm = strong_type<std::int32_t, length_mm_tag>;
using freq_hz   = strong_type<std::uint32_t, freq_hz_tag>;
using volt_t    = strong_type<std::int32_t, volt_tag>;
using ampere_t  = strong_type<std::int32_t, ampere_tag>;

// ---- CRTP 静态多态基类（零虚函数开销） ----
template <typename Derived>
struct crtp_base {
    Derived &derived() { return static_cast<Derived &>(*this); }
    const Derived &derived() const { return static_cast<const Derived &>(*this); }
};

// ---- 编译期类型工具 ----
template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <auto V>
using constant_t = std::integral_constant<decltype(V), V>;

} // namespace cppu
