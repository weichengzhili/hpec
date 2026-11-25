// result —— 无异常与错误处理
// 对应文档: ../02-cpp/cpp-06-无异常与错误处理.md
// 职责: -fno-exceptions 环境下的 Result<T,E> 类型，强制显式错误处理。
#pragma once

#include <cstdint>
#include <utility>
#include <type_traits>

namespace cppu {

// ---- 错误码基类型（分层编码） ----
using error_code = std::int32_t;

// ---- Result<T, E>（成功含 T，失败含 E；必须显式解包） ----
template <typename T, typename E>
class result {
public:
    // 成功构造
    constexpr result(T value) : ok_(true) { new (&storage_) T(std::move(value)); }
    // 失败构造（error tag）
    struct error_t {};
    static constexpr error_t error{};
    constexpr result(error_t, E err) : ok_(false) { new (&storage_) E(std::move(err)); }

    result(result &&o) noexcept : ok_(o.ok_) {
        if (ok_) new (&storage_) T(std::move(o.value()));
        else new (&storage_) E(std::move(o.error()));
    }
    result(const result &) = delete;
    result &operator=(const result &) = delete;
    ~result() { destroy(); }

    constexpr bool is_ok() const noexcept { return ok_; }
    constexpr explicit operator bool() const noexcept { return ok_; }

    T &value() { return *reinterpret_cast<T *>(&storage_); }
    const T &value() const { return *reinterpret_cast<const T *>(&storage_); }
    E &error() { return *reinterpret_cast<E *>(&storage_); }
    const E &error() const { return *reinterpret_cast<const E *>(&storage_); }

    // 链式处理
    template <typename Fn>
    auto and_then(Fn &&f) -> decltype(f(std::declval<T &>()));
    template <typename Fn>
    auto map(Fn &&f) -> result<decltype(f(std::declval<T &>())), E>;

private:
    void destroy() {
        if (ok_) value().~T();
        else error().~E();
    }
    bool ok_;
    alignas(T) alignas(E) unsigned char storage_[sizeof(T) > sizeof(E) ? sizeof(T) : sizeof(E)];
};

// ---- 便捷构造 ----
template <typename E, typename T>
result<T, E> ok(T value) { return result<T, E>(std::move(value)); }

template <typename T, typename E>
result<T, E> err(E error) { return result<T, E>(result<T, E>::error, std::move(error)); }

} // namespace cppu
